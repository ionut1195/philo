/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/25 08:07:40 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	grab_fork(t_philo *ph, t_data *dt)
{
	pthread_mutex_lock(&(dt->forks[ph->id + ph->left]));
	printf("%10ld   %d has taken a fork\n",dt->current_time,ph->id + 1);
	pthread_mutex_lock(&(dt->forks[(ph->id + ph->right) % dt->total]));
	printf("%10ld   %d has taken another fork\n", dt->current_time, ph->id + 1);
	printf("%10ld   Philosopher %d starts eating\n", dt->current_time, ph->id + 1);
	ph->meals_eaten++;
	ph->birthdate = dt->current_time;
	delay(dt->t_eat);
	pthread_mutex_unlock(&(dt->forks[ph->id + ph->left]));
	pthread_mutex_unlock(&(dt->forks[(ph->id + ph->right) % dt->total]));

	if (dt->current_time - ph->birthdate > dt->t_die) // to modify
	{
		printf("%10ld   %d has died!  X_x\n", dt->current_time,ph->id + 1);
		dt->dead = true;
	}
	if (dt->meals_param)
	{
		if (ph->meals_eaten == ph->pt->meals)
		{
			ph->is_full = true;
			printf("%d is full\n",ph->id + 1);
			dt->full_philos++;
		}
	}
	//ph->can_eat = false;
	return (1);
}

void	sleep_think(t_philo *philo)
{
	printf("%10ld   %d starts sleeping\n",philo->pt->current_time, philo->id + 1);
	delay(philo->pt->t_sleep);
	printf("%10ld   %d starts thinking\n",philo->pt->current_time, philo->id + 1);
	//philo->can_eat = true;
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;
	philo = (t_philo *)shit;
	dt = philo->pt;
	while (philo->is_full == false && dt->dead == false)
	{
		if (philo->can_eat == true)
			grab_fork(philo, dt);
		if (philo->is_full == false)
			sleep_think(philo);
	}
	return (NULL);
}

int	init_philos(t_data *d)
{
	int	i;

	i = 0;
	pthread_create(&d->time_tracker, NULL, &keep_track, d);
	while (i < d->total)
	{
		d->philos[i].id = i;
		d->philos[i].pt = d;
		d->philos[i].can_eat = true;
		d->philos[i].is_full = false;
		d->philos[i].meals_eaten = 0;
		d->philos[i].birthdate = d->current_time;
		d->philos[i].last_meal = d->current_time;
		if (i % 3 == 0)
		{
			d->philos[i].left = 1;
			d->philos[i].right = 0;
		}
		else
		{
			d->philos[i].left = 0;
			d->philos[i].right = 1;
			if (i == d->total)
				d->philos[i].right = 0;	
		}
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
		i++;
	}
	pthread_join(d->time_tracker, NULL);

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
	data.meals_param = false;
	data.total = ft_atoi(argv[1]);
	data.t_die = ft_atoi(argv[2]);
	data.t_eat = ft_atoi(argv[3]);
	data.t_sleep = ft_atoi(argv[4]);
	data.dead = false;
	data.full_philos = 0;
	if (argc == 6)
	{
		data.meals = ft_atoi(argv[5]);
		data.meals_param = true;
	}
	
	i = 0;
	data.philos = malloc(sizeof(t_philo) * data.total);
	data.forks = malloc(sizeof(pthread_mutex_t) * data.total);

	if (init_mutexes(&data))
		return 1;
	init_philos(&data);
	return 0;
}