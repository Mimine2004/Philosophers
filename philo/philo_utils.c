/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:05 by marvin            #+#    #+#             */
/*   Updated: 2025/02/14 11:44:49 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_printf(t_philo *data, int i)
{
	pthread_mutex_lock(&data->print);
	if (i == 1)
		printf("%d has taken a fork 🍴\n", data->x + 1);
	else if (i == 2)
		printf("%d is sleeping 😴\n", data->x + 1);
	else if (i == 3)
	{
		printf("%d is thinking 🤔\n", data->x + 1);
		data->is_thinking = 1;
	}
	else if (i == 4)
		printf("%d died 💀\n", is_dead(0, 1, data));
	else if (i == 5)
		printf("Everyone has eaten enough 🍽️\n");
	else if (i == 6)
		printf("%d has taken another fork 🍴\n", data->x + 1);
	else if (i == 7)
		printf("%d is eating 🍝\n", data->x + 1);
	pthread_mutex_unlock(&data->print);
}

void	data_init(t_philo *data, char **av, int var)
{
	int	i;

	i = 0;
	data->nbr_philo = ft_atol(av[1]);
	data->die = ft_atol(av[2]) * 1000;
	data->eat = ft_atol(av[3]) * 1000;
	data->sleep = ft_atol(av[4]) * 1000;
	data->nbr_eat = -1;
	if (var)
		data->nbr_eat = ft_atol(av[5]);
	data->is_thinking = 0;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);
	if (data->nbr_philo <= 0)
		exit(1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks)
		exit(1);
	while (i < data->nbr_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			exit(1);
		i++;
	}
}

int	diff_time(struct timeval start, t_philo *data)
{
	long			elapsed_time;
	struct timeval	end;

	gettimeofday(&end, NULL);
	elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec
			- start.tv_usec);
	if (elapsed_time > data->die)
		return (is_dead(data->x, 0, data), 0);
	return (1);
}
