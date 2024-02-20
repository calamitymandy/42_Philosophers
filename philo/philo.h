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
# include <sys/time.h>

typedef struct s_philos
{
	struct s_init_data	*data;
	pthread_t			thread_id; //used to store the identifier of a thread
	int					philo_id;
	//pthread_mutex_t		lock; //fake simulation
	//int					count; //fake simulation
	int					meals_eaten;
	int					is_eating;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*lock_philo; //segfault
}t_philos;

typedef struct s_init_data
{
	int					nb_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_of_meals;
	long long			start_time;
	int					is_dead;
	//int					finito;
	//pthread_mutex_t		msg;
	pthread_mutex_t		lock;
	pthread_mutex_t		*forks;
	t_philos			*philos;
}t_data;

/*MAIN*/
int		philo_is_dead(t_philos *philos);

/*UTILS*/
int		positive_atoi(const char *str);
int		get_time(void);
void	write_message(char *str, t_philos *philos);
void	wait_given_time(t_philos *philos, int given_time);

#endif
