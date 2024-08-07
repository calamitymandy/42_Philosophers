/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:44:07 by amdemuyn          #+#    #+#             */
/*   Updated: 2024/07/23 19:12:33 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * The routine function manages the actions of a philosopher in a dining 
 * philosophers problem, such as eating, sleeping, and thinking, while 
 * checking for certain conditions like death or completion of meals.
 * 
 * waiting_for_everyone: used to wait for all philosophers' threads to be 
 * created before they start their actions and so all philosophers are 
 * ready to begin their routine simultaneously to avoid any race conditions 
 * or inconsistencies in the behavior of the philosophers.
 * 
 * if philo_id is even:
 * wait_given_time(philos, 15); -> to make even philo wait and avoid 
 * all philos to take the same fork
 */
void	*routine(void *arg)
{
	t_philos	*philos;
	int			dead;

	dead = 0;
	philos = (t_philos *)arg;
	waiting_for_everyone(philos);
	if (philos->philo_id % 2 == 0)
		wait_given_time(philos, 15);
	while (!dead)
	{
		if (philo_is_dead(philos))
			return (0);
		if (lone_philo(philos))
			return (0);
		philo_is_eating(philos);
		if (have_eaten_all_his_meals(philos))
			return (0);
		philo_is_sleeping(philos);
		write_message("is thinking", philos);
		pthread_mutex_lock(&philos->data->lock_dead);
		dead = philos->data->is_dead;
		pthread_mutex_unlock(&philos->data->lock_dead);
	}
	return ((void *)arg);
}

/**
 * Checks if a philosopher has exceeded the time to die and updates
 * the status accordingly.
 * 
 * If the philosopher has exceeded the time allowed (`data->time_to_die`)
 * without eating, the function will return 1 indicating that the 
 * philosopher has died. 
 * Otherwise, it will return 0 indicating that the philosopher is still alive.
 * 
 * get_time() - last_meal > data->time_to_die: Checks if the time since 
 * the last meal is greater than the allowed time to die. If so:
 * - data->is_dead = 1; -> to stop loop if one is dead
 * - write message of death
 * - return (1)
 */
int	rip_stop_and_write(t_data *data, int i)
{
	long long	last_meal;

	pthread_mutex_lock(&data->philos[i].lock_meal);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].lock_meal);
	if (get_time() - last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->lock_dead);
		data->is_dead = 1;
		pthread_mutex_unlock(&data->lock_dead);
		write_message("died", data->philos);
		return (1);
	}
	return (0);
}

/**
 * Continuously checks:
 * 1- how many philosophers have eaten all their meals, 
 * once the nb of full bellies is the same than the nb of philos,
 * it means ALL philos have eaten ALL their meals.
 * 
 * 2- with rip_stop_and_write: the time elapsed since the last 
 * meal of each philosopher  and marks them as dead if they 
 * exceed the time allowed to die.
 */
void	*look_n_check(t_data *data)
{
	int			i;
	int			dead;
	int			nb_of_full_bellies;

	pthread_mutex_lock(&data->lock_dead);
	dead = data->is_dead;
	pthread_mutex_unlock(&data->lock_dead);
	while (!dead)
	{
		pthread_mutex_lock(&data->lock_full_bellies);
		nb_of_full_bellies = data->nb_of_full_bellies;
		pthread_mutex_unlock(&data->lock_full_bellies);
		if (nb_of_full_bellies == data->nb_of_philos)
			return (NULL);
		i = -1;
		while (++i < data->nb_of_philos)
		{
			if (rip_stop_and_write(data, i))
				break ;
		}
		pthread_mutex_lock(&data->lock_dead);
		dead = data->is_dead;
		pthread_mutex_unlock(&data->lock_dead);
	}
	return (NULL);
}

/*
* Starts the simulation by creating and managing threads for each philosopher.
* - data->philos[i].data = data -> Set the data field to the main data struct
* - pthread_create: Creates a thread for each philosopher to run the routine.
* - pthread_mutex_lock(&data->waiting_all_philos): Synchronizes to ensure all 
* philosophers start at the same time.
* - look_n_check(data): Monitors the simulation.
* - pthread_join(data->philos[i].thread_id, NULL): ensures that the main thread 
* waits for the philosopher thread i to finish its execution. It blocks the main 
* thread until the specified thread (in this case, the philosopher's thread) 
* terminates.
*/
void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_of_philos)
	{
		data->philos[i].data = data;
		pthread_create(&data->philos[i].thread_id, NULL, routine,
			(void *)&data->philos[i]);
		i++;
	}
	pthread_mutex_lock(&data->waiting_all_philos);
	data->go_go_go = 1;
	pthread_mutex_unlock(&data->waiting_all_philos);
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
	data.start_time = get_time();
	if (mallocating(&data))
		return (1);
	if (init_philos_n_forks(&data))
		return (1);
	start_simulation(&data);
	free(data.philos);
	free(data.lock_forks);
	free(data.taken_fork);
	return (0);
}
