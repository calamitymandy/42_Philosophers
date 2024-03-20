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

void	philo_is_eating(t_philos *philos)
{
	pthread_mutex_lock(philos->right_fork);
	write_message("has taken right fork", philos);
	pthread_mutex_lock(philos->left_fork);
	write_message("has taken left fork", philos);
	pthread_mutex_lock(&philos->lock_philo);

	pthread_mutex_lock(&philos->lock_meal);
	philos->last_meal = get_time();
	pthread_mutex_unlock(&philos->lock_meal);
	write_message("is eating", philos);
	philos->meals_eaten++; 
	wait_given_time(philos, philos->data->time_to_eat);
	pthread_mutex_unlock(&philos->lock_philo); 
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}

void	philo_is_sleeping(t_philos *philos)
{
	write_message("is sleeping", philos);
	wait_given_time(philos, philos->data->time_to_sleep);
}