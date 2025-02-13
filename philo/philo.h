/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:23:43 by marvin            #+#    #+#             */
/*   Updated: 2025/02/13 12:01:29 by hhecquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				eat;
	int				sleep;
	int				die;
	int				nbr_philo;
	int				nbr_eat;
	int				is_thinking;
	pthread_t		*philo;
	int				x;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t meal;
}		t_philo;

//philo.c
void	data_init(t_philo *data, char **av, int var);
void	ft_printf(t_philo *data, int i);
int	diff_time(struct timeval	start, t_philo *data);
void	*philosophers(void *arg);
int	number_of_meal(int nbr_eat, int read_only, t_philo *data);
int	is_dead(int i, int read_only, t_philo *data);
void	*big_bro_is_watching(void *arg);
//philo_utils.c
int	ft_isnum(char *str);
int	ft_atoi(char *str);
//philo_utils2.c

#endif 