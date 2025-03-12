/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:23:43 by marvin            #+#    #+#             */
/*   Updated: 2025/03/10 16:53:43 by hhecquet         ###   ########.fr       */
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
	long long		eat;
	long long		sleep;
	long long		die;
	int				nbr_philo;
	int				nbr_eat;
	int				x;
	pthread_t		philo[200];
	pthread_mutex_t	forks[200];
	pthread_mutex_t	print;
	pthread_mutex_t	death;
	pthread_mutex_t	meal;
	pthread_mutex_t	fork_state;
	pthread_mutex_t	last_meal;
}		t_philo;

//philo.c
void			*philosophers(void *arg);
int				think_n_forks(t_philo *data, int id,
					int second_fork);
int				eat_n_sleep(t_philo *data, int id,
					int second_fork);
int				initialize(t_philo *data, int id);
//philo_utils.c
int				return_to_death(t_philo *data, int id, int second_fork);
int				data_init(t_philo *data, char **av, int var);
int				diff_time(struct timeval start, t_philo *data, int id);
int				fork_state(int id, int read_only, t_philo *data, int var);
void			*big_bro_is_watching(void *arg);
//philo_utils2.c
void			ft_printf(t_philo *data, int i, int id);
int				is_dead(int i, int read_only, t_philo *data);
int				number_of_meal(int id, int read_only, t_philo *data, int av);
void			create_n_clean(t_philo *data, int i, int nbr_philo);
struct timeval	last_meal(int id, int read_only, t_philo *data);
//philo_basics.c
long long		ft_atol(char *str);
int				ft_isnum(char *str);
void			*ft_memset(void *s, int c, size_t n);
long long		get_time(void);

#endif 