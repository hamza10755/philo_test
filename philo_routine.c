/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:50 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/26 16:07:19 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_sleep(long usec)
{
	long	start;
	long	end;

	start = get_time();
	end = start + (usec / 1000);
	while (get_time() < end)
		usleep(500);
}

void	check_left_right(t_philo *philo)
{
	if (philo->id % 2 == 1)
		precise_sleep(200);
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		log_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		log_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		log_state(philo, "has taken a fork");
	}
}

void	check_routine(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	log_state(philo, "is eating");
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(data->time_to_eat * 1000);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (!check_stop(data))
	{
		log_state(philo, "is thinking");
		if (check_stop(data))
			break ;
		if (data->num_philos == 1 && log_state(philo, "has taken a fork"))
		{
			precise_sleep(data->time_to_die * 1000);
			printf("died\n");
			return (NULL);
		}
		check_left_right(philo);
		check_routine(philo, data);
		if (data->meals_required > 0
			&& philo->meals_eaten >= data->meals_required)
			break ;
		log_state(philo, "is sleeping");
		precise_sleep(data->time_to_sleep * 1000);
	}
	return (NULL);
}

void	check_philo_death(t_data *data, int i, long now)
{
	long	time_since_last_meal;
	int		meals_complete;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	time_since_last_meal = now - data->philos[i].last_meal;
	meals_complete = (data->meals_required > 0
			&& data->philos[i].meals_eaten >= data->meals_required);
	if (time_since_last_meal > data->time_to_die && !meals_complete)
	{
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		set_stop(data);
		printf("died\n");
	}
	else
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
}
