#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philos
{
    pthread_mutex_t     lock;
    int                 count;
    int                 philo_id;
    pthread_t   thread_id;
}t_philos;

typedef struct s_data
{
    int         nb_of_philos;
    t_philos    *philos;
}t_data;

void    *routine(void *arg)
{
    t_philos    *philos;

    philos = arg;
    pthread_mutex_lock(&philos->lock);
    int count = philos->count;

    while (count < 6)
    {
        printf("thread %d: count= %d\n", philos->philo_id, count);
        usleep(1);
        count++;
    }
    pthread_mutex_unlock(&philos->lock);
    return ((void *)arg);
}

void    start_simulation(t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->nb_of_philos)
    {
        data->philos[i].count = 0;
        data->philos[i].philo_id = i;
        pthread_mutex_init(&data->philos[i].lock, NULL);
        pthread_create(&data->philos[i].thread_id, NULL, routine, &data->philos[i]);
        i++;
    }
    i = 0;
    while (i < data->nb_of_philos)
    {
        pthread_join(data->philos[i].thread_id, NULL);
        pthread_mutex_destroy(&data->philos[i].lock);
        i++;
    }
}

int main(int argc, char **argv)
{
    t_data      *data;
    (void)argc;

    data = malloc(sizeof(t_data));
    data->nb_of_philos = atoi(argv[1]);
    data->philos = malloc(sizeof(t_philos) * data->nb_of_philos);
    
    start_simulation(data);
    return (0);
}