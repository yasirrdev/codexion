/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:47:19 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 12:53:56 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_cooldown(t_dongle *dongle, t_data *data)
{
	while (get_time() - dongle->last_used < data->dongle_cooldown)
	{
		if (data->stop)
			return ;
		usleep(1000);
	}
}

static void	take_even_dongles(t_coder *coder)
{
	wait_cooldown(coder->left, coder->data);
	if (coder->data->stop)
		return ;
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	wait_cooldown(coder->right, coder->data);
	if (coder->data->stop)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		return ;
	}
	pthread_mutex_lock(&coder->right->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
}

static void	take_odd_dongles(t_coder *coder)
{
	wait_cooldown(coder->right, coder->data);
	if (coder->data->stop)
		return ;
	pthread_mutex_lock(&coder->right->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	wait_cooldown(coder->left, coder->data);
	if (coder->data->stop)
	{
		pthread_mutex_unlock(&coder->right->mutex);
		return ;
	}
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
}

void	take_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
		return ;
	coder->wait_start = get_time();
	wait_for_scheduler(coder);
	if (coder->data->stop)
		return ;
	if (coder->id % 2 == 0)
		take_even_dongles(coder);
	else
		take_odd_dongles(coder);
	coder->wait_start = 0;
}

void	release_dongles(t_coder *coder)
{
	coder->left->last_used = get_time();
	coder->right->last_used = get_time();
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}
