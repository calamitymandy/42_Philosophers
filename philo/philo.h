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

/*
	pthread_t			thread_id; //used to store the identifier of a thread
*/
typedef struct s_philos
{
	struct s_init_data	*data;
	pthread_t			thread_id;
	int					philo_id;
	long long			last_meal;
	int					meals_eaten;
	int					forks;
	pthread_mutex_t		lock_meal;
}t_philos;

typedef struct s_init_data
{
	int					go_go_go;
	int					nb_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_of_meals;
	long long			start_time;
	int					is_dead;
	char				*taken_fork;
	int					nb_of_full_bellies;
	pthread_mutex_t		waiting_all_philos;
	pthread_mutex_t		lock_full_bellies;
	pthread_mutex_t		lock_dead;
	pthread_mutex_t		lock_write;
	pthread_mutex_t		*lock_forks;
	t_philos			*philos;
}t_data;

/*UTILS*/
int		positive_atoi(const char *str);
int		get_time(void);
void	wait_given_time(t_philos *philos, int given_time);
void	write_message(char *str, t_philos *philos);
int		philo_is_dead(t_philos *philos);

/*ACTIONS*/
void	philo_is_eating(t_philos *philos);
int		have_eaten_all_his_meals(t_philos *philos);
int		philo_is_sleeping(t_philos *philos);

/*INIT*/
int		start_init(char **argv, t_data *data);
int		mallocating(t_data *data);
int		init_philos_n_forks(t_data *data);
void	waiting_for_everyone(t_philos *philos);
int		lone_philo(t_philos *philos);

#endif
