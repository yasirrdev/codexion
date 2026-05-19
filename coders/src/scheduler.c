/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:40:49 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 12:46:54 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_first_fifo(t_coder *coder)
{
	int		i;
	t_data	*data;

	i = 0;
	data = coder->data;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].id != coder->id && data->coders[i].wait_start != 0
			&& data->coders[i].wait_start < coder->wait_start)
			return (0);
		i++;
	}
	return (1);
}

int	is_most_urgent_edf(t_coder *coder)
{
	int		i;
	t_data	*data;
	long	my_remaining;
	long	their_remaining;

	i = 0;
	data = coder->data;
	my_remaining = data->time_to_burnout - (get_time() - coder->last_compile);
	while (i < data->number_of_coders)
	{
		if (data->coders[i].id != coder->id && data->coders[i].wait_start != 0)
		{
			their_remaining = data->time_to_burnout - (get_time()
					- data->coders[i].last_compile);
			if (their_remaining < my_remaining)
				return (0);
		}
		i++;
	}
	return (1);
}

static void	wait_fifo(t_coder *coder)
{
	while (!is_first_fifo(coder))
	{
		if (coder->data->stop)
		{
			coder->wait_start = 0;
			return ;
		}
		usleep(1000);
	}
}

static void	wait_edf(t_coder *coder)
{
	while (!is_most_urgent_edf(coder))
	{
		if (coder->data->stop)
		{
			coder->wait_start = 0;
			return ;
		}
		usleep(1000);
	}
}

void	wait_scheduler(t_coder *coder)
{
	if (coder->data->scheduler == 0)
		wait_fifo(coder);
	else
		wait_edf(coder);
}
