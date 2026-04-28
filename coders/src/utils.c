#include "codexion.h"


void print_action(t_data *data, int id, char *msg)
{
    pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d %s\n", get_time(), id, msg);
    pthread_mutex_unlock(&data->print_mutex);
}

long	get_time(void)
{
# ifdef _WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);
	return (st.wHour * 3600000 + st.wMinute * 60000 + st.wSecond * 1000 + st.wMilliseconds);
# else
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
# endif
}