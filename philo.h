/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:10:10 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/02/20 02:10:41 by hamzabillah      ###   ########.fr       */
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

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long			last_meal;
	int				meals_eaten;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_required;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	log_mutex;
	int				stop;
	t_philo			*philos;
}					t_data;

int		main(int ac, char **av);

long	get_time(void);
void	log_state(t_philo *philo, char *state);
int		ft_atoi(const char *str);

void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

int		parse_args(t_data *data, int ac, char **av);
int		init_forks_and_philos(t_data *data);
void	cleanup(t_data *data);

#endif