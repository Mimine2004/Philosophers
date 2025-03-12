/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:05 by marvin            #+#    #+#             */
/*   Updated: 2025/02/14 13:40:25 by hhecquet         ###   ########.fr       */
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
			return (ft_printf(data, 5, -1), NULL);
		if (is_dead(0, 1, data) != 0)
			return (ft_printf(data, 6, is_dead(0, 1, data)), NULL);
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
		return (printf("Error : How are they supposed to eat ?\n"), 0);
	if (data->nbr_philo > 200)
		return (printf("Error : Not enough spaghetti for everyone\n"), 0);
	data->die = ft_atol(av[2]) * 1000;
	data->eat = ft_atol(av[3]);
	data->sleep = ft_atol(av[4]);
	data->nbr_eat = -1;
	if (var)
		data->nbr_eat = ft_atol(av[5]);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);
	pthread_mutex_init(&data->fork_state, NULL);
	while (i < data->nbr_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (1);
}

int	diff_time(struct timeval start, t_philo *data, int id)
{
	long long			elapsed_time;
	struct timeval		end;

	gettimeofday(&end, NULL);
	elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec
			- start.tv_usec);
	if (elapsed_time >= data->die)
		return (is_dead(id, 0, data), 0);
	if (number_of_meal(0, 1, data, data->nbr_eat) == 1)
		return (0);
	if (is_dead(0, 1, data) != 0)
		return (0);
	return (1);
}

int	fork_state(int id, int read_only, t_philo *data, int var)
{
	static int	forks[200];
	static int	state = 0;

	pthread_mutex_lock(&data->fork_state);
	if (state == 0)
	{
		ft_memset(forks, 0, sizeof(int) * (data->nbr_philo));
		state = 1;
	}
	if (read_only == 0)
		forks[id] = var;
	pthread_mutex_unlock(&data->fork_state);
	return (forks[id]);
}
