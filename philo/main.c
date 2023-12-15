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

#include "philo.h"

int	mallocating(t_data *data)
{
	data->threads_id = malloc(sizeof(pthread_t) * data->nb_of_philos);
	if (!data->threads_id)
	{
		printf("Malloc error: threads ids");
		return (1);
	}
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
	if ((!data->threads_id || !data->philos || !data->forks) && data)
	{
		//TO DO exit & destroy function
		return (1);
	}
	return (0);
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
		|| data->time_to_sleep < 60 || data->nb_of_meals < 0)
	{
		printf("Incorrect arguments");
		return (1);
	}
	data->dead = 0;
	data->finito = 0;
	pthread_mutex_init(&data->msg, NULL);
	pthread_mutex_init(&data->lock, NULL);
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
	return (0);
}
