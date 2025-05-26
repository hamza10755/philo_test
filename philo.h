/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:10:10 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/26 16:08:12 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int					num_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					meals_required;
	long				start_time;
	int					stop;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		*forks;
	pthread_mutex_t		log_mutex;
	t_philo				*philos;
}						t_data;

typedef struct s_philo
{
	int					id;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	long				last_meal;
	pthread_mutex_t		meal_mutex;
	int					meals_eaten;
	t_data				*data;
}						t_philo;

int						parse_args(t_data *data, int ac, char **av);
int						init_forks_and_philos(t_data *data);
void					cleanup(t_data *data);
void					*philo_routine(void *arg);
void					*monitor_routine(void *arg);
long					get_time(void);
int						log_state(t_philo *philo, char *state);
void					left_or_right(t_philo *philo);
int						ft_atoi(const char *str);
int						check_stop(t_data *data);
void					set_stop(t_data *data);
int						check_all_ate_enough(t_data *data);
void					check_philo_death(t_data *data, int i, long now);
void					check_routine(t_philo *philo, t_data *data);
void					check_left_right(t_philo *philo);
void					precise_sleep(long usec);
void					check_philo(t_data *data, int i, int *finished_eating);

#endif