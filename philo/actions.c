/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:06:29 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/03/13 17:06:34 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_dead(t_philos *philos)
{
	pthread_mutex_lock(&philos->data->lock_dead);
	if (philos->data->is_dead)
	{
		pthread_mutex_unlock(&philos->data->lock_dead);
		return (1);
	}
	pthread_mutex_unlock(&philos->data->lock_dead);
	return (0);
}

static void	is_fork_available(t_philos *philos, int which)
{
	pthread_mutex_lock(&philos->data->forks[which]);
	if (philos->data->taken_fork[which] == 0)
	{
		philos->data->taken_fork[which] = 1;
		philos->forks++;
		write_message("has taken a fork", philos);
	}
	pthread_mutex_unlock(&philos->data->forks[which]);
}

static void	drop_forks(t_philos *philos, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philos->data->forks[first_fork]);
	philos->data->taken_fork[first_fork] = 0;
	pthread_mutex_unlock(&philos->data->forks[first_fork]);
	pthread_mutex_lock(&philos->data->forks[second_fork]);
	philos->data->taken_fork[second_fork] = 0;
	pthread_mutex_unlock(&philos->data->forks[second_fork]);
	philos->forks = 0;
}

void	philo_is_eating(t_philos *philos)
{
	int	first_fork;
	int	second_fork;

	first_fork = philos->philo_id -1;
	second_fork = philos->philo_id;
	if (philos->philo_id == philos->data->nb_of_philos)
		second_fork = 0;
	if (philos->philo_id % 2 == 0)
	{
		first_fork = second_fork;
		second_fork = philos->philo_id -1;
	}
	while (philos->forks < 2)
	{
		is_fork_available(philos, first_fork);
		is_fork_available(philos, second_fork);
	}
	pthread_mutex_lock(&philos->lock_meal);
	philos->last_meal = get_time();
	pthread_mutex_unlock(&philos->lock_meal);
	write_message("is eating", philos);
	wait_given_time(philos, philos->data->time_to_eat);
	philos->meals_eaten++;
	drop_forks(philos, first_fork, second_fork);
}

void	philo_is_sleeping(t_philos *philos)
{
	write_message("is sleeping", philos);
	wait_given_time(philos, philos->data->time_to_sleep);
}
