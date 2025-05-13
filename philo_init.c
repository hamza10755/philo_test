/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:13:20 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/10 15:19:05 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int ac, char **av)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = atol(av[2]);
	data->time_to_eat = atol(av[3]);
	data->time_to_sleep = atol(av[4]);
	if (ac == 6)
		data->meals_required = ft_atoi(av[5]);
	else
		data->meals_required = -1;
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (ac == 6
			&& data->meals_required <= 0))
		return (0);
	return (1);
}

int	init_forks_and_philos(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].last_meal = get_time();
		data->philos[i].meals_eaten = 0;
		pthread_mutex_init(&data->philos[i].meal_mutex, NULL);
		i++;
	}
	pthread_mutex_init(&data->log_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->forks);
	free(data->philos);
}
