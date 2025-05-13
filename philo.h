/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:10:10 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/10 15:36:25 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_philo t_philo;

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

typedef struct s_data {
    int             num_philos;
    long             time_to_die;
    long             time_to_eat;
    long             time_to_sleep;
    int             meals_required;
    long            start_time;
    int             stop;
    pthread_mutex_t stop_mutex;
    pthread_mutex_t *forks;
    pthread_mutex_t log_mutex;
    t_philo         *philos;
} t_data;

typedef struct s_philo {
    int             id;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    long            last_meal;
    pthread_mutex_t meal_mutex;
    int             meals_eaten;
    t_data          *data;
} t_philo;

int     parse_args(t_data *data, int ac, char **av);
int     init_forks_and_philos(t_data *data);
void    cleanup(t_data *data);
void    *philo_routine(void *arg);
void    *monitor_routine(void *arg);
long    get_time(void);
void    log_state(t_philo *philo, char *state);
void left_or_right(t_philo *philo);
int     ft_atoi(const char *str);
int     check_stop(t_data *data);
void    set_stop(t_data *data);

#endif