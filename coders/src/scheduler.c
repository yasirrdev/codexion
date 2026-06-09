/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:40:49 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/06/09 12:30:38 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	heap_is_top(t_data *data, t_coder *coder)
{
	int	result;

	pthread_mutex_lock(&data->heap_mutex);
	result = (data->heap_size > 0 && data->sched_heap[0]->id == coder->id);
	pthread_mutex_unlock(&data->heap_mutex);
	return (result);
}

void	wait_scheduler(t_coder *coder)
{
	t_data	*data;

	data = coder->data;
	heap_push(data, coder);
	while (!heap_is_top(data, coder))
	{
		if (data->stop)
		{
			heap_remove(data, coder);
			coder->wait_start = 0;
			return ;
		}
		usleep(1000);
	}
	pthread_mutex_lock(&data->heap_mutex);
	data->heap_size--;
	data->sched_heap[0] = data->sched_heap[data->heap_size];
	heap_sift_down(data, 0);
	coder->wait_start = 0;
	pthread_mutex_unlock(&data->heap_mutex);
}
