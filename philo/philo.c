/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:08 by marvin            #+#    #+#             */
/*   Updated: 2025/03/10 16:54:56 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosophers(void *arg)
{
	struct timeval	start;
	t_philo			*data;
	long long		tmp;
	int				second_fork;
	int				id;

	data = (t_philo *)arg;
	id = data->x;
	second_fork = (id + 1) % data->nbr_philo;
	gettimeofday(&start, NULL);
	if (data->nbr_philo == 1)
	{
		ft_printf(data, 1, id + 1);
		while (1)
		{
			usleep(100);
			if (diff_time(start, data, id) == 0)
				return (NULL);
		}
	}
	if ((id) % 2 != 0)
	{
		ft_printf(data, 3, id + 1);
		usleep(100);
	}
	while (1)
	{
		while (pthread_mutex_lock(&data->forks[id]) != 0)
		{
			if (diff_time(start, data, id) == 0)
				return (NULL);
			if (is_dead(0, 1, data) != 0)
				return (NULL);
			usleep(100);
		}
		ft_printf(data, 1, id + 1);
		while (pthread_mutex_lock(&data->forks[second_fork]) != 0)
		{
			if (diff_time(start, data, id) == 0)
				return (pthread_mutex_unlock(&data->forks[id]), NULL);
			if (is_dead(0, 1, data) != 0)
				return (pthread_mutex_unlock(&data->forks[id]), NULL);
			usleep(100);
		}
		ft_printf(data, 6, id + 1);
		if (is_dead(0, 1, data) != 0)
			return (return_to_death(data, id, second_fork));
		ft_printf(data, 7, id + 1);
		gettimeofday(&start, NULL);
		tmp = data->eat;
		while (tmp > 0)
		{
			if (diff_time(start, data, id) == 0)
				return (return_to_death(data, id, second_fork));
			if (is_dead(0, 1, data) != 0)
				return (return_to_death(data, id, second_fork));
			usleep(100);
			tmp -= 100;
		}
		number_of_meal(id, 0, data, -1);
		if (is_dead(0, 1, data) != 0)
			return (return_to_death(data, id, second_fork));
		ft_printf(data, 8, id + 1);
		pthread_mutex_unlock(&data->forks[id]);
		pthread_mutex_unlock(&data->forks[second_fork]);
		if (is_dead(0, 1, data) != 0)
			return (return_to_death(data, id, second_fork));
		ft_printf(data, 2, id + 1);
		tmp = data->sleep;
		while (tmp > 0)
		{
			if (diff_time(start, data, id) == 0)
				return (return_to_death(data, id, second_fork));
			if (is_dead(0, 1, data) != 0)
				return (return_to_death(data, id, second_fork));
			usleep(100);
			tmp -= 100;
		}
		ft_printf(data, 3, id + 1);
		usleep(100);
	}
}

int	number_of_meal(int id, int read_only, t_philo *data, int av)
{
	static int	meals[200];
	static int	state = 0;
	int			i;

	i = data->nbr_philo - 1;
	pthread_mutex_lock(&data->meal);
	if (state == 0)
	{
		ft_memset(meals, 0, sizeof(int) * (data->nbr_philo));
		state = 1;
	}
	if (read_only == 0)
		meals[id] += 1;
	else if (av >= 0)
	{
		while (i >= 0)
		{
			if (meals[i] < data->nbr_eat)
				return (pthread_mutex_unlock(&data->meal), 0);
			i--;
		}
		return (pthread_mutex_unlock(&data->meal), is_dead(1, 0, data), 1);
	}
	return (pthread_mutex_unlock(&data->meal), 0);
}

void	*big_bro_is_watching(void *arg)
{
	t_philo	*data;

	data = (t_philo *)arg;
	while (1)
	{
		if (is_dead(0, 1, data) != 0)
			return (ft_printf(data, 4, is_dead(0, 1, data)), NULL);
		if (number_of_meal(0, 1, data, data->nbr_eat) == 1)
			return (ft_printf(data, 5, -1), NULL);
	}
}

void	create_n_clean(t_philo *data, int i, int nbr_philo)
{
	pthread_t	big_brother;

	while (i < nbr_philo)
	{
		data->x = i;
		pthread_create(&data->philo[i], NULL, philosophers, data);
		usleep(250);
		i++;
	}
	pthread_create(&big_brother, NULL, big_bro_is_watching, data);
	pthread_join(big_brother, NULL);
	i--;
	while (i >= 0)
	{
		pthread_join(data->philo[i], NULL);
		pthread_mutex_destroy(&data->forks[i--]);
	}
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->meal);
	free(data);
}

int	main(int ac, char **av)
{
	int			i;
	t_philo		*data;
	int			nbr_philo;

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
	data = malloc(sizeof(t_philo));
	if (!data)
		return (0);
	if (!data_init(data, av, ac == 6))
		return (free(data), 0);
	i = 0;
	create_n_clean(data, i, nbr_philo);
	return (0);
}
