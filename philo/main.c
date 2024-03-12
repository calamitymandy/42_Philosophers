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

/*
Problemas o Mejoras:

INIT of mutexes:
check if all pthread_mutex_init are initialized!!

Manejo de Memoria:
Aunque se verifica si la asignación de memoria es exitosa, no se libera la 
memoria reservada en caso de un error posterior.

Manejo de Errores:
En caso de que se detecte un error durante la inicialización o ejecución del
programa, sería útil implementar un mecanismo de manejo de errores más robusto, 
como la liberación adecuada de la memoria reservada y una salida clara indicando 
el motivo del error.

Optimización del Ciclo Principal:
El ciclo principal (look_n_check()) puede optimizarse para salir tan pronto 
como se detecte que un filósofo ha completado todas sus comidas o ha muerto. 
Esto puede mejorar la eficiencia del programa y reducir el tiempo de ejecución.*/

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
		pthread_mutex_init(&data->forks[i], NULL);
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

void	philo_is_sleeping(t_philos *philos)
{
	write_message("is sleeping", philos);
	wait_given_time(philos, philos->data->time_to_sleep);
}

void	philo_is_eating(t_philos *philos)
{
	pthread_mutex_lock(philos->right_fork);
	write_message("has taken right fork", philos);
	if (philos->data->nb_of_philos == 1)
	{
		wait_given_time(philos, philos->data->time_to_die);
		pthread_mutex_unlock(philos->right_fork);
		return ;
	}
	pthread_mutex_lock(philos->left_fork);
	write_message("has taken left fork", philos);
	pthread_mutex_lock(&philos->lock_philo);
	philos->last_meal = get_time();
	write_message("is eating", philos);
	philos->meals_eaten++; 
	wait_given_time(philos, philos->data->time_to_eat);
	pthread_mutex_unlock(&philos->lock_philo); 
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
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
		philo_is_eating(philos);
		if (philos->meals_eaten == philos->data->nb_of_meals)
		{
			printf("philo %d meals eaten: %d\n", philos->philo_id, philos->meals_eaten);
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

void	look_n_check(t_data *data) 
{
	int	i;
	int	stop;
	
	stop = 0;
	while (1)
	{
		i = 0;
		while (i < data->nb_of_philos)
		{
			pthread_mutex_lock(&data->philos[i].lock_philo);
			if (data->philos->meals_eaten == data->nb_of_meals)
			{
				printf("STOP SIMULATION / philo %d / i = %d data->philos->meals_eaten: %d\n", data->philos->philo_id, i, data->philos->meals_eaten);
				stop = 1;
				pthread_mutex_unlock(&data->philos[i].lock_philo);
				break ;
			}
			else if ((get_time() - (data->philos[i].last_meal) > data->time_to_die))
			{
				write_message("died", data->philos);
				pthread_mutex_lock(&data->lock_dead);
				data->is_dead = 1; //here to stop loop if one is dead!!!!
				pthread_mutex_unlock(&data->lock_dead);
				stop = 1;
				break ;
			}
			pthread_mutex_unlock(&data->philos[i].lock_philo);
			i++;
		}
		if (stop)
			break ;
	}
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
	data.start_time = get_time();
	start_simulation(&data);
	return (0);
}
