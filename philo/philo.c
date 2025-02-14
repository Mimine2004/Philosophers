/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:08 by marvin            #+#    #+#             */
/*   Updated: 2025/02/14 13:47:18 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	forks_lock(t_philo *data, int fork, int state)
{
	pthread_mutex_lock(&data->forks[fork]); // Lock the fork's mutex

	if (state == 1) // Trying to take the fork
	{
		if (data->fork_state[fork] == 1) // If fork is available
		{
			data->fork_state[fork] = 0; // Mark fork as taken
			pthread_mutex_unlock(&data->forks[fork]);
			return (1);
		}
	}

	else if (state == 0) // Trying to put the fork back
	{
		data->fork_state[fork] = 1; // Mark fork as available
	}

	pthread_mutex_unlock(&data->forks[fork]); // Unlock the fork's mutex
	return (0);
}



void	*philosophers(void *arg)
{
	struct timeval	start;
	t_philo			*data;
	int				tmp_eat;
	int				tmp_sleep;
	int				second_fork;

	data = (t_philo *)arg;
	second_fork = (data->x + 1) % data->nbr_philo;
	gettimeofday(&start, NULL);
	if (data->nbr_philo == 1)
	{
		ft_printf(data, 1);
		while (1)
		{
			usleep(500);
			if (diff_time(start, data) == 0)
				return (NULL);
		}
	}
	if ((data->x + 1) % 2 == 0)
	{
		ft_printf(data, 3);
		usleep(500);
	}
	while (1)
	{
		while (!forks_lock(data, data->x, 1))
			usleep(500);
		ft_printf(data, 1);
		while (!forks_lock(data, second_fork, 1))
		{
			forks_lock(data, data->x, 0);
			usleep(500);
		}
		ft_printf(data, 6);
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
		number_of_meal(1, 0, data, 0);
		forks_lock(data, data->x, 0);
		forks_lock(data, second_fork, 0);
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
		ft_memset(meals, 0, sizeof(int) * (data->nbr_philo - 1));
	}
	if (read_only == 0)
		meals[data->x] += nbr_eat;
	else if (var >= 0)
	{
		while (i > 0)
		{
			if (meals[i] < data->nbr_eat)
				return (pthread_mutex_unlock(&data->meal), 0);
			i--;
		}
		return (pthread_mutex_unlock(&data->meal), 1);
	}
	return (pthread_mutex_unlock(&data->meal), 0);
}

int	is_dead(int i, int read_only, t_philo *data)
{
	static int	dead = 0;

	pthread_mutex_lock(&data->death);
	if (read_only == 0)
		dead = i + 1;
	pthread_mutex_unlock(&data->death);
	return (dead);
}

void	*big_bro_is_watching(void *arg)
{
	t_philo	*data;

	data = (t_philo *)arg;
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
	int			nbr_philo;
	pthread_t	*philo;

	if (ac != 5 && ac != 6)
		return (printf("Error: Wrong number of arguments\n"), 0);
	i = 1;
	while (i < ac)
	{
		if (!ft_isnum(av[i]))
			return (printf("Error: Arguments must be positive numbers\n"), 0);
		i++;
	}
	nbr_philo = ft_atol(av[1]);
	data = malloc(sizeof(t_philo) * nbr_philo);
	if (!data)
		return (0);
	i = 0;
	while (i < nbr_philo)
	{
		data[i].x = i;
		data_init(&data[i], av, (ac - 5));
		i++;
	}
	philo = malloc(sizeof(pthread_t) * nbr_philo);
	if (!philo)
		return (free(data), 0);
	i = 0;
	while (i < nbr_philo)
	{
		pthread_create(&philo[i], NULL, philosophers, &data[i]);
		i++;
	}
	pthread_create(&big_brother, NULL, big_bro_is_watching, data);
	pthread_join(big_brother, NULL);
	free(philo);
	free(data);
	return (0);
}
