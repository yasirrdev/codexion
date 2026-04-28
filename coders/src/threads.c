#include "codexion.h"

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
    t_data *data;

    coder = (t_coder *)arg;
    data = coder->data;
    while(coder->compiles_done < data->number_of_compiles_required)
    {
        if (data->stop)
            return (NULL);
        take_dongles(coder);
        if (data->stop)
        {
            release_dongles(coder);
            return (NULL);
        }
        coder->last_compile = get_time();
        print_action(data, coder->id, "is compiling");
        usleep(data->time_to_compile * 1000);
        coder->compiles_done++;
        coder->last_compile = get_time();
        release_dongles(coder);
        if (data->stop || coder->compiles_done == data->number_of_compiles_required)
            return (NULL);
        print_action(data, coder->id, "is debugging");
        usleep(data->time_to_debug * 1000);
        print_action(data, coder->id, "is refactoring");
        usleep(data->time_to_refactor * 1000);
    }
    return (NULL);

}
int start_threads(t_data *data)
{
    int i;
    data->start_time = get_time();
    i = 0;

    while (i < data->number_of_coders)
        data->coders[i++].last_compile = data->start_time;

    i = 0;
    pthread_create(&data->monitor, NULL, monitor, data);

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
    pthread_join(data->monitor, NULL);
    return (0);
}

void *monitor(void *arg)
{
    t_data *data;
    int i;
    int finished;

    data = (t_data *)arg;
    while(1)
    {
        i = 0;
        finished = 0;
        while(i < data->number_of_coders)
        {
            if (data->coders[i].compiles_done >= data->number_of_compiles_required)
            {
                finished++;
                i++;
                continue;
            }
            if (get_time() - data->coders[i].last_compile > data->time_to_burnout)
            {
                print_action(data, data->coders[i].id, "burned out");
                data->stop = 1;
                return (NULL);
            }
            i++;
        }
        if(finished == data->number_of_coders)
        {
            print_action(data, 0, "all coders finished successfully");
            data->stop = 1;
            return (NULL);
        }
        if (data->stop)
            return (NULL);
        usleep(1000);
    }
}