/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:54:15 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/06/05 14:05:54 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	interruptible_sleep(t_data *data, long ms)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
	{
		if (data->stop)
			return ;
		usleep(1000);
	}
}

static int	do_idle_cycle(t_coder *coder, t_data *data)
{
	print_action(data, coder->id, "is debugging");
	interruptible_sleep(data, data->time_to_debug);
	print_action(data, coder->id, "is refactoring");
	interruptible_sleep(data, data->time_to_refactor);
}

static int	do_compile_cycle(t_coder *coder, t_data *data)
{
	int	acquired;

	if (data->stop)
		return (1);
	acquired = take_dongles(coder);
	if (!acquired || data->stop)
	{
		if (acquired)
			release_dongles(coder);
		return (1);
	}
	coder->last_compile = get_time();
	print_action(data, coder->id, "is compiling");
	interruptible_sleep(data, data->time_to_compile);
	coder->compiles_done++;
	release_dongles(coder);
	if (data->stop || coder->compiles_done == data->number_of_compiles_required)
		return (1);
	do_idle_cycle(coder, data);
	return (0);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->data;
	while (coder->compiles_done < data->number_of_compiles_required)
	{
		if (do_compile_cycle(coder, data))
			return (NULL);
	}
	return (NULL);
}
