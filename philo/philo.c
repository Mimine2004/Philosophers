/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:08 by marvin            #+#    #+#             */
/*   Updated: 2025/03/17 12:16:07 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosophers(void *arg)
{
	t_thread_data	*thread_data;
	t_philo			*data;
	struct timeval	tmp;
	int				second_fork;

	thread_data = (t_thread_data *)arg;
	data = thread_data->data;
	second_fork = (thread_data->id + 1) % data->nbr_philo;
	if (initialize(data, thread_data->id) == 0)
		return (NULL);
	while (1)
	{
		if (eat_n_sleep(data, thread_data->id, second_fork) == 0)
			return (NULL);
		gettimeofday(&tmp, NULL);
		while (get_time() - ((tmp.tv_sec * 1000) + (tmp.tv_usec / 1000))
			< data->sleep)
		{
			if (diff_time(last_meal(thread_data->id, 1, data, 0), data,
					thread_data->id) == 0)
				return (NULL);
			usleep(100);
		}
		ft_printf(data, 4, thread_data->id + 1);
	}
}

int	initialize(t_philo *data, int id)
{
	last_meal(id, 0, data, 0);
	if (data->nbr_philo == 1)
	{
		ft_printf(data, 1, id + 1);
		while (1)
		{
			if (diff_time(last_meal(id, 1, data, 0), data, id) == 0)
				return (0);
			usleep(100);
		}
	}
	if ((id) % 2 != 0)
		ft_printf(data, 4, id + 1);
	if ((id) % 2 != 0)
		usleep(100);
	return (1);
}

int	eat_n_sleep(t_philo *data, int id, int second_fork)
{
	long long	start;

	if (think_n_forks(data, id, second_fork) == 0)
		return (0);
	ft_printf(data, 2, id + 1);
	start = last_meal(id, 0, data, 0);
	while (get_time() - start < data->eat)
	{
		if (diff_time(start, data, id) == 0)
			return (return_to_death(data, id, second_fork));
		usleep(100);
	}
	if (diff_time(start, data, id) == 0)
		return (return_to_death(data, id, second_fork));
	ft_printf(data, 3, id + 1);
	number_of_meal(id, 0, data, -1);
	pthread_mutex_unlock(&data->forks[id]);
	pthread_mutex_unlock(&data->forks[second_fork]);
	return (1);
}

int	think_n_forks(t_philo *data, int id, int second_fork)
{
	int	first;
	int	second;

	if (id % 2 == 0)
	{
		first = id;
		second = second_fork;
	}
	else
	{
		first = second_fork;
		second = id;
	}
	pthread_mutex_lock(&data->forks[first]);
	if (diff_time(last_meal(id, 1, data, 0), data, id) == 0)
		return (pthread_mutex_unlock(&data->forks[first]), 0);
	ft_printf(data, 1, id + 1);
	pthread_mutex_lock(&data->forks[second]);
	if (diff_time(last_meal(id, 1, data, 0), data, id) == 0)
		return (return_to_death(data, id, second_fork));
	ft_printf(data, 1, id + 1);
	if (diff_time(last_meal(id, 1, data, 0), data, id) == 0)
		return (return_to_death(data, id, second_fork));
	return (1);
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
	free(data);
	return (0);
}
