#include "codexion.h"

void *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;

    pthread_mutex_lock(&coder->left->mutex);
    print_action(coder->data, coder->id, "has taken a dongle");

    pthread_mutex_lock(&coder->right->mutex);
    print_action(coder->data, coder->id, "has taken a dongle");

    print_action(coder->data, coder->id, "is compiling");

    usleep(200000);

    pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);

    return (NULL);
}

int start_threads(t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {
        if(pthread_create(&data->coders[i].thread,
            NULL, routine, &data->coders[i]) != 0)
            return (1);
        i++;                
    }
    i = 0;
    while(i < data->number_of_coders)
    {
        pthread_join(data->coders[i].thread, NULL);
        i++;
    }
    return (0);
}