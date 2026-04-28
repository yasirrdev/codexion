#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

// ---------- FORWARD DECLARATION ----------
typedef struct s_coder t_coder;

// ---------- STRUCTS ----------

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
	int				stop;

	pthread_mutex_t	print_mutex;

	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

struct s_coder
{
	int				id;
	long			last_compile;
	int				compiles_done;
	pthread_t		thread;

	t_dongle		*left;
	t_dongle		*right;

	t_data			*data;
};

// ---------- INIT ----------
int		init_data(t_data *data);
void	assign_dongles(t_data *data);

// ---------- THREADS ----------
int		start_threads(t_data *data);
void    take_dongles(t_coder *coder);
void    release_dongles(t_coder *coder);
// ---------- PARSE ----------
int		parse_args(t_data *data, int argc, char **argv);

// ---------- UTILS ----------
long	get_time(void);
void	print_action(t_data *data, int id, char *msg);

#endif