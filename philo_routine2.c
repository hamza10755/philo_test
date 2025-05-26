/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:12:15 by hbelaih           #+#    #+#             */
/*   Updated: 2025/05/26 16:12:16 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_philo(t_data *data, int i, int *finished_eating)
{
	if (data->meals_required > 0)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->philos[i].meal_mutex);
			if (data->philos[i].meals_eaten < data->meals_required)
				finished_eating = 0;
			pthread_mutex_unlock(&data->philos[i].meal_mutex);
			i++;
		}
		if (finished_eating)
		{
			set_stop(data);
			return ;
		}
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;
	int		finished_eating;

	data = (t_data *)arg;
	while (!check_stop(data))
	{
		finished_eating = 1;
		now = get_time();
		check_philo(data, i, &finished_eating);
		i = 0;
		while (i < data->num_philos)
		{
			check_philo_death(data, i, now);
			if (check_stop(data))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
