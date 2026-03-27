#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

// ---------- STRUCTS ----------

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			last_used;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			last_compile;
	int				compiles_done;
	pthread_t		thread;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;

	int				stop;

	pthread_mutex_t	print_mutex;

	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;


long	get_time(void);

#endif
