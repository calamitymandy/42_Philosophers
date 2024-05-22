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

#include "philo.h"

/* To check LEAKS use this command line:
		valgrind --leak-check=yes myprog arg1 arg2
*/

/**
 * The routine function manages the actions of a philosopher in a dining 
 * philosophers problem, such as eating, sleeping, and thinking, while 
 * checking for certain conditions like death or completion of meals.
 * 
 * if philo_id is even:
 * wait_given_time(philos, 15); -> to make even philo wait and avoid 
 * all philos to take the same fork
 */
void	*routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->philo_id % 2 == 0)
		wait_given_time(philos, 15);
	while (!philos->data->is_dead)
	{
		if (philo_is_dead(philos))
			return (0);
		if (lone_philo(philos))
			return (0);
		philo_is_eating(philos);
		if (have_eaten_all_his_meals(philos))
			return (0);
		if (philo_is_dead(philos))
			return (0);
		philo_is_sleeping(philos);
		if (philo_is_dead(philos))
			return (0);
		write_message("is thinking", philos);
	}
	return ((void *)arg);
}

/*
Check how many philosophers have eaten all their meals, 
once the nb of full bellies is the same than the nb of philos,
it means ALL philos have eaten ALL their meals.
*/
int	check_nb_of_full_bellies(t_data *data)
{
	int	nb_of_full_bellies;

	pthread_mutex_lock(&data->lock_full_bellies);
	nb_of_full_bellies = data->nb_of_full_bellies;
	pthread_mutex_unlock(&data->lock_full_bellies);
	if (nb_of_full_bellies == data->nb_of_philos)
		return (1);
	return (0);
}

/**
 * data->is_dead = 1; -> here to stop loop if one is dead
 * Continuously checks the time elapsed since the last meal of 
 * each philosopher and marks them as dead if they exceed 
 * the time allowed to die.
 */
void	*look_n_check(t_data *data)
{
	int			i;
	long long	last_meal;

	while (!data->is_dead)
	{
		if (check_nb_of_full_bellies(data))
			return (NULL);
		i = -1;
		while (++i < data->nb_of_philos)
		{
			pthread_mutex_lock(&data->philos[i].lock_meal);
			last_meal = data->philos[i].last_meal;
			pthread_mutex_unlock(&data->philos[i].lock_meal);
			if (get_time() - last_meal > data->time_to_die)
			{
				pthread_mutex_lock(&data->lock_dead);
				data->is_dead = 1;
				pthread_mutex_unlock(&data->lock_dead);
				write_message("died", data->philos);
				break ;
			}
		}
	}
	return (NULL);
}

/*
data->philos[i].data = data; -> To set the data field to the main data structure
*/
void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	init_philos(data);
	while (i < data->nb_of_philos)
	{
		data->philos[i].data = data;
		pthread_create(&data->philos[i].thread_id, NULL, routine,
			(void *)&data->philos[i]);
		i++;
	}
	look_n_check(data);
	i = 0;
	while (i < data->nb_of_philos)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		i++;
	}
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
	data.start_time = get_time();
	start_simulation(&data);
	free(data.philos);
	free(data.lock_forks);
	free(data.taken_fork);
	return (0);
}
