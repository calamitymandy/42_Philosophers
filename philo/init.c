/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:10:18 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/03/13 17:10:21 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		|| data->time_to_sleep < 60 || data->nb_of_meals < -1)
	{
		printf("Incorrect arguments");
		return (1);
	}
	data->is_dead = 0;
	pthread_mutex_init(&data->lock_dead, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
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
		data->philos[i].last_meal = get_time();
		i++;
	}
}

/*
 * The function initializes the forks for a dining philosophers problem:
 * - 1st while loop to initialize the mutex for each fork.
 * - Then assign left[0] and right[nb_of_philos -1] forks for the 1st philo
 * - 2nd loop to initialize left and right forks for every other philosopher.
 * It assigns the left fork to the current philosopher as the fork at index `i`,
 * and the right fork as the fork at index `i - 1`.
 */
int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_of_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->philos[i].lock_meal, NULL);
		pthread_mutex_init(&data->philos[i].lock_philo, NULL);
	}
	data->philos[0].left_fork = &data->forks[0];
	data->philos[0].right_fork = &data->forks[data->nb_of_philos -1];
	i = 0;
	while (++i < data->nb_of_philos)
	{
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[i - 1];
	}
	return (0);
}
