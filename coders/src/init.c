/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/05/19 12:29:15 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 12:29:15 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	free(data->coders);
	free(data->dongles);
	free(data->sched_heap);
}

int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc != 9)
		return (printf("Error: wrong number of arguments\n"), 1);
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->number_of_compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	if (data->number_of_coders < 1 || data->time_to_burnout < 0
		|| data->time_to_compile < 0
		|| data->time_to_debug < 0 || data->time_to_refactor < 0
		|| data->number_of_compiles_required < 1 || data->dongle_cooldown < 0)
		return (printf("Error: invalid argument values\n"), 1);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 0;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 1;
	else
		return (printf("Error: invalid scheduler\n"), 1);
	if (data->number_of_coders <= 0)
		return (printf("Error: invalid number_of_coders\n"), 1);
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	data->sched_heap = malloc(sizeof(t_coder *) * data->number_of_coders);
	if (!data->sched_heap || !data->coders || !data->dongles)
		return (1);
	data->stop = 0;
	data->heap_size = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->heap_mutex, NULL);
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		data->dongles[i].last_used = 0;
		data->coders[i].id = i + 1;
		data->coders[i].last_compile = 0;
		data->coders[i].compiles_done = 0;
		data->coders[i].data = data;
		i++;
	}
	return (0);
}

void	assign_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].left = &data->dongles[i];
		data->coders[i].right = &data->dongles[(i + 1)
			% data->number_of_coders];
		i++;
	}
}
