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

/* Problemas o Mejoras:

Manejo de Memoria:
Liberar la memoria reservada.

Manejo de Errores:
En caso de que se detecte un error durante la inicialización o ejecución del
programa, sería útil implementar un mecanismo de manejo de errores más robusto, 
como la liberación adecuada de la memoria reservada y una salida clara indicando 
el motivo del error.
*/

#include "philo.h"

int	mallocating(t_data *data)
{
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

void	*routine(void *arg)
{
	t_philos	*philos;

	philos = (t_philos *)arg;
	if (philos->philo_id % 2 == 0) // MOVE IT ELSEWHERE???
		wait_given_time(philos, 1); // to avoid all philos to take same fork
	while (!philos->data->is_dead)
	{
		if (philo_is_dead(philos))
			return (0);
		if (philos->data->nb_of_philos == 1)
		{
			write_message("has taken the one and only fork", philos);
			wait_given_time(philos, philos->data->time_to_die);
			return (0);
		}
		philo_is_eating(philos);
		if (philos->meals_eaten == philos->data->nb_of_meals)
		{
			pthread_mutex_lock(&philos->data->lock_full_bellies);
			philos->data->nb_of_full_bellies++;
			printf("philo %d meals eaten: %d\n", philos->philo_id, philos->meals_eaten);
			pthread_mutex_lock(&philos->data->lock_full_bellies);
			return (0);
		}
		if (philo_is_dead(philos))
			return (0);
		philo_is_sleeping(philos);
		if (philo_is_dead(philos))
			return (0);
		write_message("is thinking", philos);
	}
	return ((void *)arg);
}

void	*look_n_check(t_data *data) 
{
	int			i;
	int			someone_died;
	int			nb_of_full_bellies;
	long long	last_meal;
	
	pthread_mutex_lock(&data->lock_dead);
	someone_died = data->is_dead;
	pthread_mutex_unlock(&data->lock_dead);
	while (!someone_died)
	{
		i = 0;
		while (i < data->nb_of_philos)
		{
			pthread_mutex_lock(&data->lock_full_bellies);
			nb_of_full_bellies = data->nb_of_full_bellies;
			pthread_mutex_unlock(&data->lock_full_bellies);
			if (nb_of_full_bellies == data->nb_of_philos)
			{
				printf("MAKE IT STOP WHEN nb_of_full_bellies: %d == data->nb_of_philos %d bc ALL PHILOS have eaten ALL THEIR MEALS\n", nb_of_full_bellies, data->nb_of_philos);
				return NULL;
			}
			pthread_mutex_lock(&data->philos[i].lock_meal);
			last_meal = data->philos[i].last_meal;
			pthread_mutex_unlock(&data->philos[i].lock_meal);
			if (get_time() - last_meal > data->time_to_die)
			{
				pthread_mutex_lock(&data->lock_dead);
				data->is_dead = 1; //here to stop loop if one is dead!!!!
				someone_died = 1;
				pthread_mutex_lock(&data->lock_dead);
				printf("%lld philosopher %d %s\n", get_time() 
					- data->start_time, data->philos->philo_id, "died");
				break ;
			}
			i++;
		}
	}
	return NULL;
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	init_philos(data);
	while (i < data->nb_of_philos)
	{
		data->philos[i].data = data; // Set the data field to the main data structure
		pthread_create(&data->philos[i].thread_id, NULL, routine, (void *)&data->philos[i]);
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
	return (0);
}
