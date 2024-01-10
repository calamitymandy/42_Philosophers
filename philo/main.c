/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 19:14:57 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/12/07 19:19:36 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* system("leaks philo"); */

#include "philo.h"

int	mallocating(t_data *data)
{
	// data->thread_id = malloc(sizeof(pthread_t) * data->nb_of_philos);
	// if (!data->thread_id)
	// {
	// 	printf("Malloc error: threads ids");
	// 	return (1);
	// }
	data->philos = malloc(sizeof(t_philos) * data->nb_of_philos);
	if (!data->philos)
	{
		printf("Malloc error: philos");
		return (1);
	}
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philos);
	if (!data->forks)
	{
		printf("Malloc error: forks");
		return (1);
	}
	if ((!data->philos || !data->forks) && data)
	{
		//TO DO exit & destroy function
		return (1);
	}
	return (0);
}

/*
 * The function initializes the forks for a dining philosophers problem:
 * - 1st while loop to to initialize the mutex for each fork.
 * - Then assign left [0] and right [nb_of_philos -1] forks for the 1st philo
 * - 2nd loop to initialize left and right forks for every other philosopher.
 * It assigns the left fork to the current philosopher as the fork at index `i`,
 * and the right fork as the fork at index `i - 1`.
 */
int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_of_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 1;
	data->philos[0].left_fork = &data->forks[0];
	data->philos[0].right_fork = &data->forks[data->nb_of_philos -1];
	while (i < data->nb_of_philos)
	{
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philos	*philos;

	philos = arg;
	pthread_mutex_lock(&philos->lock);
	while (philos->count < 6)
	{
		printf("thread_id: %d | philo_id: %d | count = %d\n", (int)philos->thread_id, philos->philo_id, philos->count);
		usleep(1);
		philos->count++;
	}
	pthread_mutex_unlock(&philos->lock);
	return ((void *)arg);
}

void	start_simulation(t_data *data)
{
	int	i;

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

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].is_eating = 0;
		//pthread_mutex_init(&data->philos[i].lock, NULL);
		i++;
	}
}

int	start_init(char **argv, t_data *data)
{
	data->nb_of_philos = positive_atoi(argv[1]);
	data->time_to_die = positive_atoi(argv[2]);
	data->time_to_eat = positive_atoi(argv[3]);
	data->time_to_sleep = positive_atoi(argv[4]);
	data->nb_of_meals = -1;
	if (argv[5])
		data->nb_of_meals = positive_atoi(argv[5]);
	if (data->nb_of_philos <= 0 || data->nb_of_philos > 200
		|| data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60 || data->nb_of_meals < 0)
	{
		printf("Incorrect arguments");
		return (1);
	}
	//data->dead = 0;
	//data->finito = 0;
	//pthread_mutex_init(&data->msg, NULL);
	//pthread_mutex_init(&data->lock, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	if (start_init(argv, &data))
		return (1);
	if (mallocating(&data))
		return (1);
	if (init_forks(&data))
		return (1);
	start_simulation(&data);
	data.start_time = get_time();
	printf("%lld", data.start_time);
	return (0);
}
