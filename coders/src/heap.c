/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:09:19 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/06/05 15:08:09 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int heap_compare(t_coder *a, t_coder *b, t_data *data)
{
	if(data->scheduler == 0)
	{
		if(a->wait_start != b->wait_start)
			return (a->wait_start < b->wait_start);
		
		return (a->id < b->id);
	}
	if(a->last_compile != b->last_compile)
		return (a->last_compile < b->last_compile);
	return (a->id > b->id);
}

void	heap_swap(t_data *data, int i, int j)
{
	t_coder *temp;
	
	temp = data->sched_heap[i];
	data->sched_heap[i] = data->sched_heap[j];
	data->sched_heap[j] = temp;
}

void heap_sift_up(t_data *data, int i)
{
	int parent;

	while(i > 0)
	{
		parent = (i - 1) / 2;

		if (heap_compare(data->sched_heap[i], data->sched_heap[parent], data))
		{
			heap_swap(data, i, parent);
			i = parent;
		}
		else
			break;
	}
}

void heap_sift_down(t_data *data, int i)
{
	int left;
	int right;
	int best;

	while(1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		best = i;

		if (left < data->heap_size
			&& heap_compare(data->sched_heap[left], data->sched_heap[best], data))
			best = left;
		if (right < data->heap_size
			&& heap_compare(data->sched_heap[right], data->sched_heap[best], data
			))
			best = right;
		if (best == i)
			break;
		heap_swap(data, i, best);
		i = best;
	}
}

void heap_push(t_data *data, t_coder *coder)
{
	pthread_mutex_lock(&data->heap_mutex);
	data->sched_heap[data->heap_size] = coder;
	data->heap_size++;
	heap_sift_up(data, data->heap_size - 1);
	pthread_mutex_unlock(&data->heap_mutex);
}

void heap_remove(t_data *data, t_coder *coder)
{
	int i;

	pthread_mutex_lock(&data->heap_mutex);
	i = 0;

	while(i < data->heap_size && data->sched_heap[i] != coder->id)
		 i++;

	if (i>= data->heap_size)
		return(pthread_mutex_unlock(&data->heap_mutex));
	if (i < data->heap_size)
	{
		data->sched_heap[i] = data->sched_heap[data->heap_size - 1];
		data->heap_size--;
		heap_sift_down(data, i);
	}
	data->heap_size--;
	if (i < data->heap_size)
	{
		data->sched_heap[i] = data->sched_heap[data->heap_size];
		heap_sift_down(data, i);
		heap_sift_up(data, i);
	}
	pthread_mutex_unlock(&data->heap_mutex);
}

