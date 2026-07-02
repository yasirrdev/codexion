/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 16:08:08 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/07/02 16:08:27 by ybel-maa         ###   ########.fr       */
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

static int	take_single_dongle(t_coder *coder)
{
	wait_cooldown(coder->left, coder->data);
	if (coder->data->stop)
		return (0);
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	return (1);
}

static int	take_even_dongles(t_coder *coder)
{
	wait_cooldown(coder->left, coder->data);
	if (coder->data->stop)
		return (0);
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	wait_cooldown(coder->right, coder->data);
	if (coder->data->stop)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->right->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	return (1);
}

static int	take_odd_dongles(t_coder *coder)
{
	wait_cooldown(coder->right, coder->data);
	if (coder->data->stop)
		return (0);
	pthread_mutex_lock(&coder->right->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	wait_cooldown(coder->left, coder->data);
	if (coder->data->stop)
	{
		pthread_mutex_unlock(&coder->right->mutex);
		return (0);
	}
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->data, coder->id, "has taken a dongle");
	return (1);
}

int	take_dongles(t_coder *coder)
{
	coder->wait_start = get_time();
	wait_scheduler(coder);
	if (coder->data->stop)
		return (0);
	if (coder->left == coder->right)
		return (take_single_dongle(coder));
	if (coder->id % 2 == 0)
		return (take_even_dongles(coder));
	return (take_odd_dongles(coder));
}

void	release_dongles(t_coder *coder)
{
	coder->left->last_used = get_time();
	if (coder->left == coder->right)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		return ;
	}
	coder->right->last_used = get_time();
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}