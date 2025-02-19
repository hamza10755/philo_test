/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamzabillah <hamzabillah@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 02:10:51 by hamzabillah       #+#    #+#             */
/*   Updated: 2025/02/20 02:10:59 by hamzabillah      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor;
	int			i;

	if (ac < 5 || ac > 6 || !parse_args(&data, ac, av))
		return (write(2, "Invalid arguments\n", 18), 1);
	data.start_time = get_time();
	data.stop = 0;
	if (!init_forks_and_philos(&data))
		return (1);
	pthread_create(&monitor, NULL, monitor_routine, &data);
	i = 0;
	while (i < data.num_philos)
		pthread_create(&data.philos[i++].thread, NULL, philo_routine,
			&data.philos[i]);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data.num_philos)
		pthread_join(data.philos[i++].thread, NULL);
	cleanup(&data);
	return (0);
}
