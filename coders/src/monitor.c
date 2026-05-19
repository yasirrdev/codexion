/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:06:24 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 13:11:58 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].compiles_done >= data->number_of_compiles_required)
		{
			i++;
			continue ;
		}
		if (get_time() - data->coders[i].last_compile > data->time_to_burnout)
		{
			print_action(data, data->coders[i].id, "burned out");
			data->stop = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	count_finished(t_data *data)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].compiles_done >= data->number_of_compiles_required)
			finished++;
		i++;
	}
	return (finished);
}

void	*monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_burnout(data))
			return (NULL);
		if (count_finished(data) >= data->number_of_coders)
		{
			print_action(data, 0, "All coders finished successfully");
			data->stop = 1;
			return (NULL);
		}
		if (data->stop)
			return (NULL);
		usleep(1000);
	}
}
