/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 19:33:25 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/12/07 19:33:27 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

typedef struct s_philos
{
	//TODO
}t_philos;

typedef struct s_init_data
{
	pthread_t			*threads_id;
	int					nb_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_of_meals;
	int					dead;
	int					finito;
	pthread_mutex_t		msg;
	pthread_mutex_t		lock;
	pthread_mutex_t		*forks;
	t_philos			*philos;
}t_data;



int	main(int argc, char **argv);
//int	start_init(char **argv, t_data data);

/*UTILS*/
int	positive_atoi(const char *str);

#endif
