#include "codexion.h"


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

void take_dongles(t_coder *coder)
{
        if (coder->left == coder->right)
        return ;

    if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->left->mutex);
        print_action(coder->data, coder->id, "has taken a dongle");
        pthread_mutex_lock(&coder->right->mutex);
        print_action(coder->data, coder->id, "has taken a dongle");
    }
    else
    {
        pthread_mutex_lock(&coder->right->mutex);
        print_action(coder->data, coder->id, "has taken a dongle");
        pthread_mutex_lock(&coder->left->mutex);
        print_action(coder->data, coder->id, "has taken a dongle");        
    }
}

void release_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
}

void *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;

    take_dongles(coder);
    print_action(coder->data, coder->id, "is compiling");
# ifdef _WIN32
    Sleep(200);
# else
    usleep(200000);
# endif
    release_dongles(coder);
    return (NULL);
}
