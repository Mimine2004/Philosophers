/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:31:08 by marvin            #+#    #+#             */
/*   Updated: 2025/02/12 14:31:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(char *str)
{
    int i;
    int res;

    i = 0;
    res = 0;
    while (str[i])
    {
        res = res * 10 + str[i] - '0';
        i++;
    }
    return (res);
}

void data_init(t_philo *data, char **av, int var)
{
    data->nbr_philo = ft_atoi(av[1]);
    data->eat = ft_atoi(av[2]);
    data->sleep = ft_atoi(av[3]);
    data->die = ft_atoi(av[4]);
    if (var)
        data->nbr_eat = ft_atoi(av[5]);
    data->is_thinking = 0;
}

int ft_isnum(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int main(int ac, char **av)
{
    int i;
    t_philo *data;

    i = ac -1;
    if (ac != 5 && ac != 6)
    {
        printf("Error: Wrong number of arguments\n");
        return (0);
    }
    while (i != 0)
    {
        if (!ft_isnum(av[i]))
        {
            printf("Error: Arguments must be numbers\n");
            return (0);
        }
        i--;
    }
    data = malloc(sizeof(t_philo));
    if (ac == 6)
        data_init(data, av, 1);
    else
        data_init(data, av, 0);
}