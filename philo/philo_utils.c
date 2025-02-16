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

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_printf(t_philo *data, int i, int id)
{
	static int	state = 0;

	if (is_dead(0, 1, data) != 0)
		state = 1;
	if (state == 1 && id > -1 && i != 4)
		return ;
	pthread_mutex_lock(&data->print);
	if (i == 1)
		printf("%lld \033[34;01m%d\033[00m has taken a fork 🍴\n", get_time(), id);
	else if (i == 2)
		printf("%lld \033[34;01m%d\033[00m is sleeping 😴\n", get_time(), id);
	else if (i == 3)
		printf("%lld \033[34;01m%d\033[00m is thinking 🤔\n", get_time(), id);
	else if (i == 4)
		printf("%lld \033[34;01m%d\033[00m died 💀\n", get_time(), is_dead(0, 1, data));
	else if (i == 5)
		printf("%lld \033[34;01mEveryone\033[00m has eaten enough 🍽️\n", get_time());
	else if (i == 6)
		printf("%lld \033[34;01m%d\033[00m has taken another fork 🍴\n", get_time(), id);
	else if (i == 7)
		printf("%lld \033[34;01m%d\033[00m is eating 🍝\n", get_time(), id);
	else if (i == 8)
		printf("%lld \033[34;01m%d\033[00m has finished eating 🍽️\n", get_time(), id);
	pthread_mutex_unlock(&data->print);
}

int	data_init(t_philo *data, char **av, int var)
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
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);
	if (data->nbr_philo <= 0)
		return (0);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks)
		return (0);
	while (i < data->nbr_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (1);
}

int	diff_time(struct timeval start, t_philo *data, int id)
{
	long			elapsed_time;
	struct timeval	end;

	gettimeofday(&end, NULL);
	elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec
			- start.tv_usec);
	if (elapsed_time > data->die)
		return (is_dead(id, 0, data), 0);
	if (is_dead(0, 1, data) != 0)
		return (0);
	return (1);
}
