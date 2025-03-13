/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:26:38 by hhecquet          #+#    #+#             */
/*   Updated: 2025/03/13 16:59:26 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_printf(t_philo *data, int i, int id)
{
	static int	state = 0;

	pthread_mutex_lock(&data->print);
	if (is_dead(0, 1, data) != 0)
		state = 1;
	if (state == 1 && id > -1 && i != 6)
		return (pthread_mutex_unlock(&data->print));
	if (i == 1)
		printf("%lld \033[1;34m%d\033[00m has taken a fork 🍴\n", get_time(),
			id);
	else if (i == 2)
		printf("%lld \033[1;34m%d\033[00m is eating 🍝\n", get_time(), id);
	else if (i == 3)
		printf("%lld \033[1;34m%d\033[00m is sleeping 😴\n", get_time(), id);
	else if (i == 4)
		printf("%lld \033[1;34m%d\033[00m is thinking 🤔\n", get_time(), id);
	else if (i == 5)
		printf("%lld \033[1;34mEveryone\033[00m has eaten enough 🍽️\n",
			get_time());
	else if (i == 6)
		printf("%lld \033[1;34m%d\033[00m died 💀\n", get_time(), is_dead(0,
				1, data));
	pthread_mutex_unlock(&data->print);
	return (1);
}

int	is_dead(int i, int read_only, t_philo *data)
{
	static int	dead = 0;
	int			result;

	pthread_mutex_lock(&data->death);
	if (read_only == 0 && dead == 0)
		dead = i + 1;
	result = dead;
	pthread_mutex_unlock(&data->death);
	return (result);
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
		pthread_mutex_unlock(&data->meal);
		return (is_dead(1, 0, data), 1);
	}
	return (pthread_mutex_unlock(&data->meal), 0);
}

void	create_n_clean(t_philo *data, int i, int nbr_philo)
{
	pthread_t		big_brother;
	struct timeval	wait;
	t_thread_data	*thread_data;

	thread_data = malloc(sizeof(t_thread_data) * nbr_philo);
	if (!thread_data)
		return (mutexes_destroy(nbr_philo, data));
	while (i < nbr_philo)
	{
		thread_data[i].data = data;
		thread_data[i].id = i;
		pthread_create(&data->philo[i], NULL, philosophers, &thread_data[i]);
		i++;
		gettimeofday(&wait, NULL);
		while ((get_time() - ((wait.tv_sec * 1000) + (wait.tv_usec
						/ 1000))) < nbr_philo)
			usleep(10);
	}
	pthread_create(&big_brother, NULL, big_bro_is_watching, data);
	pthread_join(big_brother, NULL);
	i--;
	while (i >= 0)
		pthread_join(data->philo[i--], NULL);
	mutexes_destroy(nbr_philo, data);
	free(thread_data);
}

struct timeval	last_meal(int id, int read_only, t_philo *data)
{
	static struct timeval	time[200];
	static int				state = 0;
	struct timeval			result;

	pthread_mutex_lock(&data->fork_state);
	if (state == 0)
	{
		ft_memset(time, 0, sizeof(int) * (data->nbr_philo));
		state = 1;
	}
	if (read_only == 0)
		gettimeofday(&time[id], NULL);
	result = time[id];
	pthread_mutex_unlock(&data->fork_state);
	return (result);
}

/*
This function is like a bonus but, it print more information about the state
of each philosophers. It can help for degugging cause' it's say when
they let go forks, and when does he take his second one. it goes with
the comment in "philo.c" like this one.
*/
/***************************************************************************
void	ft_printf(t_philo *data, int i, int id)
{
	static int	state = 0;

	if (is_dead(0, 1, data) != 0)
		state = 1;
	if (state == 1 && id > -1 && i != 4)
		return ;
	pthread_mutex_lock(&data->print);
	if (i == 1)
		printf("%lld \033[1;34m%d\033[00m has taken a fork 🍴\n", get_time(),
			id);
	else if (i == 2)
		printf("%lld \033[1;34m%d\033[00m is sleeping 😴\n", get_time(), id);
	else if (i == 3)
		printf("%lld \033[1;34m%d\033[00m is thinking 🤔\n", get_time(), id);
	else if (i == 4)
		printf("%lld \033[1;34m%d\033[00m died 💀\n", get_time(), is_dead(0,
				1, data));
	else if (i == 5)
		printf("%lld \033[1;34mEveryone\033[00m has eaten enough 🍽️\n",
			get_time());
	else if (i == 6)
		printf("%lld \033[1;34m%d\033[00m has taken another fork 🍴\n",
			get_time(), id);
	else if (i == 7)
		printf("%lld \033[1;34m%d\033[00m is eating 🍝\n", get_time(), id);
	else if (i == 8)
		printf("%lld \033[1;34m%d\033[00m has finished eating 🍽️\n",
			get_time(), id);
	pthread_mutex_unlock(&data->print);
}***************************************************************************/