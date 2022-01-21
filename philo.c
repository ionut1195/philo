/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/21 16:03:44 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	grab_fork(t_philo *ph, t_data *dt)
{
	pthread_mutex_lock(&(dt->forks[ph->id]));
	printf("%llu   %d has taken a fork\n",get_current_timestamp(),ph->id);
	pthread_mutex_lock(&(dt->forks[(ph->id + 1) % dt->total]));
	printf("%llu   %d has taken another fork\n", get_current_timestamp(), ph->id);
	printf("%llu   Philosopher %d starts eating\n", get_current_timestamp(), ph->id);
	pthread_mutex_unlock(&(dt->forks[ph->id]));
	pthread_mutex_unlock(&(dt->forks[(ph->id + 1) % dt->total]));
	ph->can_eat = false;
	return (1);
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;
	int		meals;
	//loop
	philo = (t_philo *)shit;
	dt = philo->pt;
	meals = 0;
	while (meals < dt->meals)
	{
		if (philo->can_eat == true)
		{
			pthread_mutex_lock(&dt->function_access);
			grab_fork(philo, dt);
			pthread_mutex_unlock(&dt->function_access);
		}
		if (philo->can_eat == false)
		{
			usleep(500);
			philo->can_eat = true;
		}
		meals++;
	}
	return (NULL);
}

int	init_philos(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->total)
	{
		d->philos[i].id = i;
		d->philos[i].pt = d;
		d->philos[i].can_eat = true;
		i++;
	}
	i = 0;
	while (i < d->total)
	{
		if (pthread_create(&(d->philos[i].thread), NULL, &do_shit, (void *)&d->philos[i]))
		{
			printf("thread couldn't be created");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < d->total)
	{
		if (pthread_join(d->philos[i].thread, NULL))
		{
			printf("thread couldn't be joined\n");
			return 2;
		}
		else
			printf("thread joined\n");
		i++;
	}

	return (0);
}

int	init_mutexes(t_data *d)
{
	int i;

	i = 0;
	pthread_mutex_init(&d->function_access, NULL);
	while (i < d->total)
	{
		if (pthread_mutex_init(&d->forks[i], NULL))
		{
			printf("mutex init has failed\n");
			return (1);
		}
		i++;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	t_data	data;
	int	i;
	if (argc <= 1)
		return 1;
	data.total = ft_atoi(argv[1]);
	data.meals = ft_atoi(argv[2]);
	i = 0;
	data.philos = malloc(sizeof(t_philo) * data.total);
	data.forks = malloc(sizeof(pthread_mutex_t) * data.total);

	if (init_mutexes(&data))
		return 1;
	init_philos(&data);
	return 0;
}