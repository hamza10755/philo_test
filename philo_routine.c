/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:50 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/01 14:33:00 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *arg)
{
    t_philo *philo;
    t_data  *data;

    philo = (t_philo *)arg;
    data = philo->data;
    if (philo->id % 2 == 0)
        usleep(1000);
    
    while (!check_stop(data))
    {
        log_state(philo, "is thinking");
        pthread_mutex_lock(philo->left_fork);
        log_state(philo, "has taken a fork");
        if (data->num_philos == 1)
        {
            usleep(data->time_to_die * 1000);
            pthread_mutex_unlock(philo->left_fork);
            continue;
        }
        pthread_mutex_lock(philo->right_fork);
        log_state(philo, "has taken a fork");
        pthread_mutex_lock(&philo->meal_mutex);
        philo->last_meal = get_time();
        pthread_mutex_unlock(&philo->meal_mutex);
        log_state(philo, "is eating");
        usleep(data->time_to_eat * 1000);
        pthread_mutex_lock(&philo->meal_mutex);
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_mutex);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        if (data->meals_required > 0 && philo->meals_eaten >= data->meals_required)
            break;
        
        log_state(philo, "is sleeping");
        usleep(data->time_to_sleep * 1000);
    }
    return (NULL);
}

void *monitor_routine(void *arg)
{
    t_data *data;
    int    i;
    long   now;
    int    all_ate_enough;

    data = (t_data *)arg;
    
    while (!check_stop(data))
    {
        i = 0;
        all_ate_enough = (data->meals_required > 0);
        
        while (i < data->num_philos && !check_stop(data))
        {
            now = get_time();
            pthread_mutex_lock(&data->philos[i].meal_mutex);
            if (now - data->philos[i].last_meal > data->time_to_die)
            {
                pthread_mutex_unlock(&data->philos[i].meal_mutex);
                set_stop(data);
                log_state(&data->philos[i], "died");
                return (NULL);
            }
            if (data->meals_required > 0 && data->philos[i].meals_eaten < data->meals_required)
                all_ate_enough = 0;
            pthread_mutex_unlock(&data->philos[i].meal_mutex);
            
            i++;
        }
        if (all_ate_enough)
        {
            set_stop(data);
            pthread_mutex_lock(&data->log_mutex);
            printf("All philosophers have eaten enough meals\n");
            pthread_mutex_unlock(&data->log_mutex);
            return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}