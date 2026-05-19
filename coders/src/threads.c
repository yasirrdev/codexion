/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:18:04 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 13:18:04 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_compile_times(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
		data->coders[i++].last_compile = data->start_time;
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

int	start_threads(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	init_compile_times(data);
	pthread_create(&data->monitor, NULL, monitor, data);
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL, routine,
				&data->coders[i]) != 0)
			return (1);
		i++;
	}
	join_threads(data);
	return (0);
}
