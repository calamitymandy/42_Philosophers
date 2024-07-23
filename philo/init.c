/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:10:42 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/07/23 19:10:57 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_init(char **argv, t_data *data)
{
	data->go_go_go = 0;
	data->nb_of_philos = positive_atoi(argv[1]);
	data->time_to_die = positive_atoi(argv[2]);
	data->time_to_eat = positive_atoi(argv[3]);
	data->time_to_sleep = positive_atoi(argv[4]);
	data->nb_of_full_bellies = 0;
	data->nb_of_meals = -1;
	data->is_dead = 0;
	if (argv[5])
		data->nb_of_meals = positive_atoi(argv[5]);
	if (data->nb_of_philos <= 0 || data->nb_of_philos > 200
		|| data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60 || data->nb_of_meals < -1)
	{
		printf("Incorrect arguments");
		return (1);
	}
	pthread_mutex_init(&data->lock_dead, NULL);
	pthread_mutex_init(&data->lock_write, NULL);
	pthread_mutex_init(&data->lock_full_bellies, NULL);
	pthread_mutex_init(&data->waiting_all_philos, NULL);
	return (0);
}

int	mallocating(t_data *data)
{
	data->philos = malloc(sizeof(t_philos) * data->nb_of_philos);
	data->lock_forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philos);
	data->taken_fork = malloc(sizeof(char) * data->nb_of_philos);
	if (!data->philos || !data->lock_forks || !data->taken_fork)
	{
		if (!data->philos)
			printf("Malloc error: philos");
		else if (!data->lock_forks || !data->taken_fork)
			printf("Malloc error: forks");
		return (1);
	}
	return (0);
}

/**
 * Initializes the data structure for philosophers and forks, 
 * setting initial values and initializing mutex locks.
 */
int	init_philos_n_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		data->philos[i].philo_id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = get_time();
		data->philos[i].forks = 0;
		if ((pthread_mutex_init(&data->lock_forks[i], NULL) != 0)
			|| (pthread_mutex_init(&data->philos[i].lock_meal, NULL) != 0))
		{
			printf("Mutex init error: forks");
			return (1);
		}
		data->taken_fork[i] = 0;
		i++;
	}
	return (0);
}

/**
 * Initiate a loop to wait until all philosophers are ready to start.
 */
void	waiting_for_everyone(t_philos *philos)
{
	int	ready_to_go;

	ready_to_go = 0;
	while (!ready_to_go)
	{
		pthread_mutex_lock(&philos->data->waiting_all_philos);
		ready_to_go = philos->data->go_go_go;
		pthread_mutex_unlock(&philos->data->waiting_all_philos);
	}
}

/**
 * Checks if there is only one philosopher 
 * if so, waits time_to_die and return (1) to stop routine
 */
int	lone_philo(t_philos *philos)
{
	if (philos->data->nb_of_philos == 1)
	{
		write_message("has taken a fork", philos);
		wait_given_time(philos, philos->data->time_to_die);
		return (1);
	}
	return (0);
}
