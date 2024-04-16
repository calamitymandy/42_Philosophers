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
	pthread_t			monitor; //store thread id of the monitor
	int					philo_id;
	long long			last_meal;
	int					meals_eaten;
	int					is_eating;
	int					forks;
	pthread_mutex_t		lock_philo;
	pthread_mutex_t		lock_meal;
}	t_philos;

typedef struct s_init_data
{
	int					nb_of_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					nb_of_meals;
	long long			start_time;
	int					is_dead;
	int					nb_of_full_bellies;
	char				*taken_fork;
	pthread_mutex_t		lock_dead;
	pthread_mutex_t		lock_full_bellies;
	pthread_mutex_t		lock;
	pthread_mutex_t		*forks;
	t_philos			*philos;
}	t_data;

/*UTILS*/
int		positive_atoi(const char *str);
int		get_time(void);
void	write_message(char *str, t_philos *philos);
void	wait_given_time(t_philos *philos, int given_time);
void	write_dead(char *str, t_data *data);
int	check_nb_of_full_bellies(t_data *data);

/*ACTIONS*/
int		philo_is_dead(t_philos *philos);
void	philo_is_sleeping(t_philos *philos);
void	philo_is_eating(t_philos *philos);

/*INIT*/
int		start_init(char **argv, t_data *data);
void	init_philos(t_data *data);
int		init_forks(t_data *data);

#endif
