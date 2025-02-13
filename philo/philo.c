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

void	data_init(t_philo *data, char **av, int var)//ok
{
	data->nbr_philo = ft_atoi(av[1]);
	data->eat = ft_atoi(av[2]) * 1000;
	data->sleep = ft_atoi(av[3]) * 1000;
	data->die = ft_atoi(av[4]) * 1000;
	if (var)
		data->nbr_eat = ft_atoi(av[5]);
	data->is_thinking = 0;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->meal, NULL);
	var = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_philo);
	while (var < data->nbr_philo)
	{
		pthread_mutex_init(&data->forks[var], NULL);//segfault ici 🚨 malloc size of philo
		var++;
	}
}

void	ft_printf(t_philo *data, int i)//ok
{
	pthread_mutex_lock(&data->print);
	if (i == 1)
		printf("%d has taken forks 🍴\n%d is eating 🍝\n", data->x + 1, data->x + 1);
	else if (i == 2)
		printf("%d is sleeping 😴\n", data->x + 1);
	else if (i == 3)
	{
		printf("%d is thinking 🤔\n", data->x + 1);
		data->is_thinking = 1;
	}
	else if (i == 4)
		printf("%d died 💀\n", data->x + 1);
	else if  (i == 5)
		printf("Everyone has eaten enough 🍽️\n");
	pthread_mutex_unlock(&data->print);
}

int	diff_time(struct timeval	start, t_philo *data)
{
	long	elapsed_time;
	struct timeval	end;
	
	gettimeofday(&end, NULL);
	elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	if (elapsed_time > data->die)
        return (is_dead(data->x, 0, data), 0);
	return (1);
}

void	*philosophers(void *arg)
{
	struct timeval	start;
	int 			tmp_eat;
	int				tmp_sleep;
	int				second_fork;
	t_philo	*data = *(t_philo **)arg;
	
	if (data->x == data->nbr_philo - 1)
		second_fork = 0;
	else
		second_fork = data->x + 1;
	while (1)
	{
		tmp_eat = data->eat;
		tmp_sleep = data->sleep;
		if (pthread_mutex_trylock(&data->forks[data->x]) == 0 && pthread_mutex_trylock(&data->forks[second_fork]) == 0)
		{
			ft_printf(data, 1);
			gettimeofday(&start, NULL); 
			while (tmp_eat > 0)
			{
				usleep(data->eat / 1000);
				if (diff_time(start, data) == 0)
				return (NULL);
				tmp_eat -= data->eat / 1000;
			}
			pthread_mutex_unlock(&data->forks[data->x]);
			pthread_mutex_unlock(&data->forks[second_fork]);
			ft_printf(data, 2);
			while (tmp_sleep > 0)
			{
				usleep(data->sleep / 1000);
				if (diff_time(start, data) == 0)
				return (NULL);
				tmp_sleep -= data->sleep / 1000;
			}
			ft_printf(data, 3);
		}
		usleep(2);
		if (diff_time(start, data) == 0)
				return (NULL);
		if (data->is_thinking == 0)
			ft_printf(data, 3);
	}
}

int	number_of_meal(int nbr_eat, int read_only, t_philo *data)//ok
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
	else
	{
		while (i > 0)
		{
			if (meals[i] < data->nbr_eat)
				return (0);
			i--;
		}
	}
	pthread_mutex_unlock(&data->meal);
	return (1);
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
	t_philo	*data = *(t_philo **)arg;
	while (1)
	{
		if (is_dead(0, 1, data) != 0)
			return (ft_printf(data, 4), NULL);
		if (number_of_meal(0, 1, data) == 1)
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
	while (i != 0)
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
	while (i < ac)
	{
		data->x = i;
		pthread_create(&data->philo[i], NULL, philosophers, data);
		i++;
	}
	pthread_create(&big_brother, NULL, big_bro_is_watching, data);
	pthread_join(big_brother, NULL);
	free(data->philo);
	free(data);
	return (0);
}
