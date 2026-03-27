#include "../include/codexion.h"


int parse_args(t_data *data, int argc, char **argv)
{
    if(argc != 9)
        return (printf("Error: wrong number of arguments\n"), 1);
    
    data->number_of_coders = atoi(argv[1]);
    data->time_to_burnout = atoi(argv[2]);
    data->time_to_compile = atoi(argv[3]);
    data->time_to_debug = atoi(argv[4]);
    data->time_to_refactor = atoi(argv[5]);
    data->number_of_compiles_required = atoi(argv[6]);
    data->dongle_cooldown= atoi(argv[7]);

    if(strcmp(argv[8], "fifo") == 0)
        data->scheduler = 0;
    else if(strcmp(argv[8], "edf") == 0)
        data->scheduler = 1;
    else
        return (printf("Error: invalid scheduler\n"), 1);
    
    if(data->number_of_coders <= 0)
        return (printf("Error: invalid number_of_coders\n"), 1);
    
    return (0);
}

int init_data(t_data *data)
{
    int i;

    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    if(!data->coders || !data->dongles)
        return(1);
    
    pthread_mutex_init(&data->print_mutex, NULL);

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_init(&data->dongles[i].mutex, NULL);
        data->dongles[i].last_used = 0;

        data->coders[i].id = i + 1;
        data->coders[i].last_compile = 0;
        data->coders[i].compiles_done = 0;

        i++;
    }
    
    return (0);
}
