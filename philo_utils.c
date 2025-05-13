/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbelaih <hbelaih@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:21 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/05/13 16:27:41 by hbelaih          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

void	set_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	log_state(t_philo *philo, char *state)
{
	long	timestamp;
	t_data	*data;
	char	*color;

	color = RESET;
	data = philo->data;
	pthread_mutex_lock(&data->log_mutex);
	if (strcmp(state, "died") == 0)
	{
		if (check_stop(data))
		{
			pthread_mutex_unlock(&data->log_mutex);
			return ;
		}
		set_stop(data);
		color = RED;
	}
	else if (check_stop(data))
	{
		pthread_mutex_unlock(&data->log_mutex);
		return ;
	}
	timestamp = get_time() - data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, state);
	pthread_mutex_unlock(&data->log_mutex);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}
