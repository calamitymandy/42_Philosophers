/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:12:37 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/12/08 18:12:40 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Takes a string and returns an integer.
If param(str) is not a positive integer (ej. negative int or char):
return -2
*/
int	positive_atoi(const char *str)
{
	int				i;
	int				is_char;
	unsigned long	res;

	i = 0;
	is_char = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		is_char = 0;
		i++;
	}
	if (str[i] == '-' || is_char || res > 2147483647)
		return (-2);
	return (res);
}

/* Get the current time in milliseconds since 1970-01-01 00:00:00*/
int	get_time(void)
{
	struct timeval	time_value;

	gettimeofday(&time_value, NULL);
	return ((time_value.tv_sec * 1000) + (time_value.tv_usec / 1000));
}

/**
 * The function "wait_given_time" waits for a given amount of time or 
 * until a philosopher is dead.
 * 
 * @param philos A pointer to a struct that contains information about
 * the philosophers.
 * @param given_time The given_time parameter is the amount of time in
 * milliseconds that the function
 * should wait for.
 */
void	wait_given_time(t_philos *philos, int given_time)
{
	int	start_time;

	(void)philos;
	start_time = get_time();
	while ((get_time() - start_time) < given_time
		&& !philo_is_dead(philos))
		usleep(50);
}

void	write_message(char *str, t_philos *philos)
{
	if (*str == 'd' && *(str + 1) == 'i')
	{
		pthread_mutex_lock(&philos->data->lock_write);
		printf("%lld %d %s\n", get_time()
			- philos->data->start_time, philos->philo_id, str);
		pthread_mutex_unlock(&philos->data->lock_write);
		return ;
	}
	pthread_mutex_lock(&philos->data->lock_dead);
	if (philos->data->is_dead == 1)
	{
		pthread_mutex_unlock(&philos->data->lock_dead);
		return ;
	}
	pthread_mutex_lock(&philos->data->lock_write);
	printf("%lld %d %s\n", get_time()
		- philos->data->start_time, philos->philo_id, str);
	pthread_mutex_unlock(&philos->data->lock_write);
	pthread_mutex_unlock(&philos->data->lock_dead);
}

int	philo_is_dead(t_philos *philos)
{
	pthread_mutex_lock(&philos->data->lock_dead);
	if (philos->data->is_dead == 1)
	{
		pthread_mutex_unlock(&philos->data->lock_dead);
		return (1);
	}
	pthread_mutex_unlock(&philos->data->lock_dead);
	return (0);
}
