/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybel-maa <ybel-maa@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:22:32 by ybel-maa          #+#    #+#             */
/*   Updated: 2026/05/19 13:22:32 by ybel-maa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_coder	t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			last_used;
}	t_dongle;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	volatile int	stop;
	long			start_time;
	pthread_t		monitor;
	pthread_mutex_t	print_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

struct s_coder
{
	int				id;
	volatile long	last_compile;
	volatile int	compiles_done;
	volatile long	wait_start;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_data			*data;
};

int		init_data(t_data *data);
void	assign_dongles(t_data *data);
int		parse_args(t_data *data, int argc, char **argv);
void	cleanup(t_data *data);

int		start_threads(t_data *data);
void	*monitor(void *arg);
void	*routine(void *arg);

int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
void	wait_cooldown(t_dongle *dongle, t_data *data);
void	wait_scheduler(t_coder *coder);
void	interruptible_sleep(t_data *data, long ms);

long	get_time(void);
void	print_action(t_data *data, int id, char *msg);

#endif