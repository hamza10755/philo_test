/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:50 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/02/20 02:12:15 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!data->stop)
	{
		log_state(philo, "is thinking");
		pthread_mutex_lock(philo->left_fork);
		log_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		log_state(philo, "has taken a fork");
		philo->last_meal = get_time();
		log_state(philo, "is eating");
		usleep(data->time_to_eat * 1000);
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		if (data->meals_required && philo->meals_eaten >= data->meals_required)
			break ;
		log_state(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;

	data = (t_data *)arg;
	while (!data->stop)
	{
		i = 0;
		while (i < data->num_philos)
		{
			now = get_time();
			if (now - data->philos[i].last_meal >= data->time_to_die)
			{
				data->stop = 1;
				log_state(&data->philos[i], "died");
				return (NULL);
			}
			i++;
		}
		usleep(5000);
	}
	return (NULL);
}
