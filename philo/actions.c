/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 19:08:04 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/07/23 19:08:38 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	is_fork_available(t_philos *philos, int which)
{
	pthread_mutex_lock(&philos->data->lock_forks[which]);
	if (philos->data->taken_fork[which] == 0)
	{
		philos->data->taken_fork[which] = 1;
		philos->forks++;
		write_message("has taken a fork", philos);
	}
	pthread_mutex_unlock(&philos->data->lock_forks[which]);
}

static void	drop_forks(t_philos *philos, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philos->data->lock_forks[first_fork]);
	philos->data->taken_fork[first_fork] = 0;
	pthread_mutex_unlock(&philos->data->lock_forks[first_fork]);
	pthread_mutex_lock(&philos->data->lock_forks[second_fork]);
	philos->data->taken_fork[second_fork] = 0;
	pthread_mutex_unlock(&philos->data->lock_forks[second_fork]);
	philos->forks = 0;
}

/**
 * 1. Determines the correct forks for the philosopher to pick up.
 * 		- If the philosopher is the last one (ID == total nB of philos), 
 * 		second_fork is set to 0 to handle the circular arrangement of forks.
 * 		- To avoid deadlock, the order in which forks are picked up is swapped 
 * 		for even-numbered philosophers. This ensures that not all philosophers
 *		try to pick up the same fork first, reducing contention.
 * 2. Ensures the philosopher picks up both forks before eating.
 * 		The loop goes on until the philo has both forks (philos->forks < 2).
 * 3. Updates the last meal time in a thread-safe manner.
 * 4. Logs the eating action and simulates the eating time.
 * 5. Increments the count of meals eaten.
 * 6. Releases the forks after eating.
 */
void	philo_is_eating(t_philos *philos)
{
	int	first_fork;
	int	second_fork;

	first_fork = philos->philo_id - 1;
	second_fork = philos->philo_id;
	if (philos->philo_id == philos->data->nb_of_philos)
		second_fork = 0;
	if (philos->philo_id % 2 == 0)
	{
		first_fork = second_fork;
		second_fork = philos->philo_id - 1;
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

int	have_eaten_all_his_meals(t_philos *philos)
{
	if (philos->meals_eaten == philos->data->nb_of_meals)
	{
		pthread_mutex_lock(&philos->data->lock_full_bellies);
		philos->data->nb_of_full_bellies++;
		pthread_mutex_unlock(&philos->data->lock_full_bellies);
		return (1);
	}
	return (0);
}

int	philo_is_sleeping(t_philos *philos)
{
	if (philo_is_dead(philos))
		return (0);
	write_message("is sleeping", philos);
	wait_given_time(philos, philos->data->time_to_sleep);
	return (1);
}
