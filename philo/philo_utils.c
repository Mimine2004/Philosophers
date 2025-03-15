/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:05 by marvin            #+#    #+#             */
/*   Updated: 2025/03/14 11:42:03 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*big_bro_is_watching(void *arg)
{
	t_philo	*data;

	data = (t_philo *)arg;
	while (1)
	{
		if (number_of_meal(0, 1, data, data->nbr_eat) == 1)
			return (end(0, 1, data), ft_printf(data, 5, -1), NULL);
		if (is_dead(0, 1, data) != 0)
			return (end(0, 1, data), ft_printf(data, 6, is_dead(0, 1, data)),
				NULL);
		usleep(10);
	}
}

int	return_to_death(t_philo *data, int id, int second_fork)
{
	pthread_mutex_unlock(&data->forks[id]);
	pthread_mutex_unlock(&data->forks[second_fork]);
	return (0);
}

int	data_init(t_philo *data, char **av, int var)
{
	int	i;

	i = 0;
	data->nbr_philo = ft_atol(av[1]);
	if (data->nbr_philo <= 0)
		return (printf("Error : Who gonna eat ?\n"), 0);
	if (data->nbr_philo > 200)
		return (printf("Error : Not enough spaghetti for everyone\n"), 0);
	data->die = ft_atol(av[2]);
	data->eat = ft_atol(av[3]);
	data->sleep = ft_atol(av[4]);
	data->nbr_eat = -2;
	if (var)
		data->nbr_eat = ft_atol(av[5]);
	if (data->die == -1 || data->eat == -1 || data->sleep == -1
		|| data->nbr_eat == -1)
		return (printf("Error : incorrect values\n"), 0);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);
	pthread_mutex_init(&data->last_meal, NULL);
	pthread_mutex_init(&data->end, NULL);
	while (i < data->nbr_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
	return (1);
}

int	diff_time(struct timeval start, t_philo *data, int id)
{
	long long			elapsed_time;
	struct timeval		next;

	gettimeofday(&next, NULL);
	elapsed_time = (next.tv_sec - start.tv_sec) * 1000 + (next.tv_usec
			- start.tv_usec) / 1000;
	if (elapsed_time >= data->die)
		return (is_dead(id, 0, data), 0);
	if (end(0, 0, data) == 1)
		return (0);
	return (1);
}

int	end(int read_only, int var, t_philo *data)
{
	static int	end = 0;
	int			result;

	pthread_mutex_lock(&data->end);
	if (read_only == 0)
		end = var;
	result = end;
	pthread_mutex_unlock(&data->end);
	return (result);
}
