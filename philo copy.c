/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/27 11:25:36 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_philo *p, t_data *d, int forks)
{
	if (d->current_time > p->will_die)
	{
		printf("%10ld   %d has died!  X_x\n", d->current_time,p->id + 1);
		d->dead = true;
	}
	else if (d->current_time + d->t_eat > p->will_die)
	{
			if (forks == 2)
			{
				waiting(p->will_die - d->current_time);
				printf("%10ld   %d has died!  X_x\n", d->current_time,p->id + 1);
				d->dead = true;
			}
	}
}

int	dine(t_philo *ph, t_data *dt)
{
	printf("%10ld %d has to die at %10ld\n\n",dt->current_time, ph->id + 1, ph->will_die);
	check_death(ph,dt,0);
	pthread_mutex_lock(&(dt->forks[ph->left]));
	check_death(ph,dt,1);
	printf("%10ld   %d has taken a fork\n",dt->current_time,ph->id + 1);
	pthread_mutex_lock(&(dt->forks[ph->right % dt->total]));
	printf("%10ld   %d has taken another fork\n", dt->current_time, ph->id + 1);
	printf("%10ld   Philosopher %d starts eating\n", dt->current_time, ph->id + 1);
	printf("%d has to die at %10ld\n\n", ph->id + 1, ph->will_die);
	check_death(ph,dt, 2);
	waiting(dt->t_eat);
	ph->meals_eaten++;
	if (ph->meals_eaten == ph->pt->meals)
	{
		ph->is_full = true;
		printf("%d is full\n",ph->id + 1);
		dt->full_philos++;
	}
	ph->will_die = dt->current_time + dt->t_die;
	printf("%10ld   Philosopher %d finished eating\n", dt->current_time, ph->id + 1);
	printf("%10ld %d has to die at %10ld\n\n",dt->current_time, ph->id + 1, ph->will_die);
	pthread_mutex_unlock(&(dt->forks[ph->left]));
	pthread_mutex_unlock(&(dt->forks[ph->right % dt->total]));
	return (1);
}

void	sleep_think(t_philo *philo)
{
	printf("%10ld   %d starts sleeping\n", philo->pt->current_time, philo->id + 1);
	waiting(philo->pt->t_sleep);
	printf("%10ld   %d starts thinking\n", philo->pt->current_time, philo->id + 1);
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;
	philo = (t_philo *)shit;
	dt = philo->pt;
	while (philo->is_full == false && dt->dead == false)
	{
		dine(philo, dt);
		if (philo->is_full == false && !dt->dead)
			sleep_think(philo);
	}
	return (NULL);
}

void	init_philos(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->total)
	{
		d->philos[i].id = i;
		d->philos[i].pt = d;
		d->philos[i].is_full = false;
		d->philos[i].meals_eaten = 0;
		d->philos[i].birthdate = d->current_time;
		d->philos[i].will_die = get_time() + d->t_die;
		if (i % 3 == 0 && i != d->total - 1)
		{
			d->philos[i].left = i + 1;
			d->philos[i].right = i;
		}
		else
		{
			d->philos[i].left = i;
			d->philos[i].right = i + 1;
			if (i == d->total)
				d->philos[i].right = 0;	
		}
		i++;
	}
}

void	start_ph(t_data *d)
{
	int	i;

	i = 0;
	pthread_create(&d->time_tracker, NULL, &keep_track, d);
	while (i < d->total)
	{
		if (pthread_create(&(d->philos[i].thread), NULL, &do_shit, (void *)&d->philos[i]))
			printf("thread couldn't be created");
		i++;
	}
	i = 0;
	while (i < d->total)
	{
		if (pthread_join(d->philos[i].thread, NULL))
			printf("thread couldn't be joined\n");
		i++;
	}
	pthread_join(d->time_tracker, NULL);
	
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
	start_ph(&data);
	return 0;
}