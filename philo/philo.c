/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:08 by marvin            #+#    #+#             */
/*   Updated: 2025/02/13 12:10:39 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    data_init(t_philo *data, char **av, int var)
{
	int i = 0;
	
	data->nbr_philo = ft_atol(av[1]);
	data->die = ft_atol(av[2]) * 1000;
	data->eat = ft_atol(av[3]) * 1000;
	data->sleep = ft_atol(av[4]) * 1000;
	data->nbr_eat = 0;
	if (var)
		data->nbr_eat = ft_atol(av[5]);
	data->is_thinking = 0;

	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);

	if (data->nbr_philo <= 0) // ✅ Ensures valid philosopher count
	{
		printf("Error: Invalid philosopher count %d\n", data->nbr_philo);
		exit(1);
	}

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	if (!data->forks) // ✅ Ensures memory allocation success
	{
		printf("Error: Memory allocation for forks failed!\n");
		exit(1);
	}

	printf("Initializing %d forks...\n", data->nbr_philo);
	while (i < data->nbr_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Mutex initialization failed at index %d\n", i);
			exit(1);
		}
		i++;
	}
}

void	ft_printf(t_philo *data, int i)//ok
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
	else if  (i == 5)
		printf("Everyone has eaten enough 🍽️\n"); 
	else if (i == 6)
		printf("%d has taken another fork 🍴\n", data->x + 1);
	else if (i == 7)
		printf("%d is eating 🍝\n", data->x + 1);
	pthread_mutex_unlock(&data->print);
}

int	diff_time(struct timeval start, t_philo *data)
{
	long	elapsed_time;
	struct timeval	end;
	
	gettimeofday(&end, NULL);
	elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	if (elapsed_time > data->die)
		return (is_dead(data->x, 0, data), 0);
	return (1);
}

void *philosophers(void *arg)
{
	struct timeval start;
	t_philo *data = (t_philo *)arg;
	int 			tmp_eat;
	int				tmp_sleep;
	int				second_fork;
	
	second_fork = (data->x + 1) % data->nbr_philo;
	gettimeofday(&start, NULL);
	if (data->x % 2 == 0)
	{
        usleep(500);
		ft_printf(data, 3);
	}
	while (1)
	{
		if (diff_time(start, data) == 0)
			return (NULL);
		// 🍴 Grab First Fork
		pthread_mutex_lock(&data->forks[data->x]);
		ft_printf(data, 1);
		
		// 🍴 Grab Second Fork
		pthread_mutex_lock(&data->forks[second_fork]);
		ft_printf(data, 6);
		
		// 🛑 Check if philosopher died while waiting for second fork
		if (diff_time(start,data) == 0)
		{
			pthread_mutex_unlock(&data->forks[data->x]);
			pthread_mutex_unlock(&data->forks[second_fork]);
			return NULL;
		}
		
		gettimeofday(&start, NULL);
		ft_printf(data, 7);
		tmp_eat = data->eat;
		while (tmp_eat > 0)
		{
			usleep(500);
			if (diff_time(start, data) == 0)
				return (NULL);
			tmp_eat -= 500;
		}
		pthread_mutex_unlock(&data->forks[data->x]);
		pthread_mutex_unlock(&data->forks[second_fork]);
		ft_printf(data, 2);
		tmp_sleep = data->sleep;
		while (tmp_sleep > 0)
		{
			usleep(500);
			if (diff_time(start, data) == 0)
				return (NULL);
			tmp_sleep -= 500;
		}
        ft_printf(data, 3);
		usleep(500);
	}
}

int	number_of_meal(int nbr_eat, int read_only, t_philo *data, int var)//ok
{
	static int	*meals;
	int			i;

	i = data->nbr_philo - 1;
	pthread_mutex_lock(&data->meal);
	if (!meals)
	{
		meals = malloc(sizeof(int) * data->nbr_philo - 1);
		if (!meals)
			return (0);
	}
	if (read_only == 0)
		meals[data->x] = nbr_eat;
	else if (var)
	{
		while (i > 0)
		{
			if (meals[i] < data->nbr_eat)
				return (0);
			i--;
		}
		return (1);
	}
	pthread_mutex_unlock(&data->meal);
	return (0);
}

int	is_dead(int i, int read_only, t_philo *data)//ok
{
	static int	dead = 0;

	pthread_mutex_lock(&data->death);
	if (read_only == 0)
		dead = i + 1;
	pthread_mutex_unlock(&data->death);	
	return (dead);
}

void	*big_bro_is_watching(void *arg)//ok
{
	t_philo *data = (t_philo *)arg; 
	while (1)
	{
		if (is_dead(0, 1, data) != 0)
			return (ft_printf(data, 4), NULL);
		if (number_of_meal(0, 1, data, data->nbr_eat) == 1)
			return (ft_printf(data, 5), NULL);
	}
}

int	main(int ac, char **av)
{
	int			i;
	t_philo		*data;
	pthread_t	big_brother;

	i = ac -1;
	if (ac != 5 && ac != 6)
		return (printf("Error: Wrong number of arguments\n"), 0);
	while (i > 0)
	{
		if (!ft_isnum(av[i]))
			return (printf("Error: Arguments must be numbers\n"), 0);
		i--;
	}
	data = malloc(sizeof(t_philo));
	if (!data)
		return (0);
	data_init(data, av, (ac - 5));
	data->philo = malloc(sizeof(pthread_t) * data->nbr_philo);
	if (!data->philo)
		return (free(data), 0);
	for (i = 0; i < data->nbr_philo; i++)
	{
		data->x = i;

		pthread_create(&data->philo[i], NULL, philosophers, data);
	}
	pthread_create(&big_brother, NULL, big_bro_is_watching, data);
	pthread_join(big_brother, NULL);
	free(data->philo);
	free(data);
	return (0);
}
