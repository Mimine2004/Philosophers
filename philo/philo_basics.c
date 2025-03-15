/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_basics.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 20:44:25 by marvin            #+#    #+#             */
/*   Updated: 2025/03/14 11:32:45 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atol(char *str)
{
	int			i;
	long long	res;

	i = 0;
	res = 0;
	while (str[i])
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	if (res > 2147483647 || res < 0)
		res = -1;
	return (res);
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;

	str = s;
	while (n != 0)
	{
		*str = c;
		str++;
		n--;
	}
	return (1);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	mutexes_destroy(int nbr_philo, t_philo *data)
{
	int	i;

	i = nbr_philo - 1;
	while (i >= 0)
		pthread_mutex_destroy(&data->forks[i--]);
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->meal);
	pthread_mutex_destroy(&data->last_meal);
	pthread_mutex_destroy(&data->end);
}
/*
void	ft_usleep(int time)
{
	long long	start;

	start = get_time();
	while (get_time() * 1000 - start * 1000 < time)
		usleep(100);
	return ;
}*/
