/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:50 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/13 17:38:39 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_sleep(long usec)
{
	long	start;
	long	elapsed;
	long	remaining;
	start = get_time();
	while (1)
	{
		elapsed = get_time() - start;
		remaining = (usec / 1000) - elapsed;
		if (remaining <= 0)
			break ;
		if (remaining > 1)
			usleep((remaining - 1) * 1000);
		while (get_time() - start < (usec / 1000))
			;
	}
}

void precise_usleep(long usec)
{
    long start;
    long elapsed;
    long remaining;
    
    start = get_time();  // Using your existing get_time function
    while (1)
    {
        elapsed = get_time() - start;
        // Convert from ms to μs for comparison with usec
        remaining = usec / 1000 - elapsed;
        
        if (remaining <= 0)
            break;
            
        if (remaining > 1)
            usleep((remaining - 1) * 1000);
            
        // Busy wait for the final precision
        while ((get_time() - start) < (usec / 1000))
            ;
    }
}

void	check_left_right(t_philo *philo)
{
	if (philo->id % 2 == 1)
		precise_sleep(500);
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
	log_state(philo, "has released a fork");
	pthread_mutex_unlock(philo->right_fork);
	log_state(philo, "has released a fork");
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
		if (data->num_philos == 1)
		{
			log_state(philo, "has taken a fork");
			precise_sleep(data->time_to_die * 1000);
			log_state(philo, "died");
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

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	time_since_last_meal = now - data->philos[i].last_meal;
	if (time_since_last_meal > data->time_to_die + 15)
	{
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		set_stop(data);
		log_state(&data->philos[i], "died");
	}
	else
	{
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
	}
}

int	check_all_ate_enough(t_data *data)
{
	int	i;
	int	all_ate_enough;

	i = 0;
	all_ate_enough = (data->meals_required > 0);
	while (i < data->num_philos && !check_stop(data))
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->meals_required > 0
			&& data->philos[i].meals_eaten < data->meals_required)
			all_ate_enough = 0;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	return (all_ate_enough);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;

	data = (t_data *)arg;
	while (!check_stop(data))
	{
		i = -1;
		now = get_time();
		while (++i < data->num_philos && !check_stop(data))
		{
			check_philo_death(data, i, now);
			if (check_stop(data))
				return (NULL);
		}
		if (check_all_ate_enough(data))
		{
			set_stop(data);
			pthread_mutex_lock(&data->log_mutex);
			printf("All philosophers have eaten enough meals\n");
			pthread_mutex_unlock(&data->log_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
