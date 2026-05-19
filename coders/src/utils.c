/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:23:41 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 13:23:41 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_action(t_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d %s\n", get_time(), id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
