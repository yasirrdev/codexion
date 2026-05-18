#include "codexion.h"

void interruptible_sleep(t_data *data, long ms)
{
    long start;

    start = get_time();
    
    while (get_time() - start < ms)
    {
        if (data->stop)
            return ;
        usleep(1000);
    }
}

void wait_cooldown(t_dongle *dongle, t_data *data)
{
    while(get_time() - dongle->last_used < data->dongle_cooldown)
    {
        if (data->stop)
            return ;
        usleep(1000);
    }
}


void take_dongles(t_coder *coder)
{
        if (coder->left == coder->right)
        return ;

    if (coder->id % 2 == 0)
    {
        wait_cooldown(coder->left, coder->data);
        if(coder->data->stop) return ;
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
    else
    {
        wait_cooldown(coder->right, coder->data);
        if(coder->data->stop) return ;
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
}

void release_dongles(t_coder *coder)
{
    coder->left->last_used = get_time();
    coder->right->last_used = get_time();
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
        interruptible_sleep(data, data->time_to_compile);
        coder->compiles_done++;
        coder->last_compile = get_time();
        release_dongles(coder);
        if (data->stop || coder->compiles_done == data->number_of_compiles_required)
            return (NULL);
        print_action(data, coder->id, "is debugging");
        interruptible_sleep(data, data->time_to_debug);
        print_action(data, coder->id, "is refactoring");
        interruptible_sleep(data, data->time_to_refactor);
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
