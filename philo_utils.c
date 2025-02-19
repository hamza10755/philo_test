/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:11:21 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/02/20 02:11:26 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_state(t_philo *philo, char *state)
{
	long	timestamp;
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->log_mutex);
	if (data->stop && strcmp(state, "died") != 0)
	{
		pthread_mutex_unlock(&data->log_mutex);
		return ;
	}
	timestamp = get_time() - data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, state);
	pthread_mutex_unlock(&data->log_mutex);
}

int		ft_atoi(const char *str)
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