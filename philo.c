/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/27 17:03:18 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_philo *p, t_data *d)
{
	if (d->current_time > p->will_die)
		d->dead = true;
	else if (d->current_time + d->t_eat > p->will_die)
	{
				waiting(p->will_die - d->current_time + 1);
				d->dead = true;
	}
}

void	print_action(t_philo *p,t_data *d, char *st, pthread_mutex_t *m)
{
	pthread_mutex_lock(&d->print_mtx);
	check_death(p, d);
	if (!d->dead && st)
		printf("%10ld	Philosopher %d %s\n",d->current_time,p->id + 1, st);
	if (d->dead && !d->over)
	{
		printf("%10ld	Philosopher %d has died\n",d->current_time,p->id + 1);
		d->over = true;
		if (m)
			pthread_mutex_unlock(m);
	}
	pthread_mutex_unlock(&d->print_mtx);
}

int	dine(t_philo *ph, t_data *dt)
{
	pthread_mutex_lock(&(dt->forks[ph->left].mtx));
	print_action(ph,dt, "has taken a fork", &dt->forks[ph->left].mtx);
	pthread_mutex_lock(&(dt->forks[ph->right % dt->total].mtx));
	print_action(ph,dt,"has taken another fork",&dt->forks[ph->right % dt->total].mtx);
	print_action(ph, dt, "starts eating", NULL);
	waiting(dt->t_eat);
	ph->meals_eaten++;
	if (ph->meals_eaten >= ph->pt->meals)
	{
		ph->is_full = true;
		print_action(ph, dt , "is full", NULL);
		dt->full_philos++;
	}
	ph->will_die = dt->current_time + dt->t_die;
	pthread_mutex_unlock(&(dt->forks[ph->left].mtx));
	pthread_mutex_unlock(&(dt->forks[ph->right % dt->total].mtx));
	dt->forks[ph->left].last_user = ph->id;
	dt->forks[ph->right % dt->total].last_user = ph->id;
	return (1);
}

void	sleep_think(t_philo *philo)
{
	print_action(philo, philo->pt, "starts sleeping", NULL);
	waiting(philo->pt->t_sleep);
	print_action(philo, philo->pt, "starts thinking", NULL);
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;
	philo = (t_philo *)shit;
	dt = philo->pt;
	if (dt->total == 1)
	{
		pthread_mutex_lock(&dt->forks[philo->id].mtx);
		print_action(philo, dt, "has taken a fork", NULL);
		waiting(dt->t_die);
		print_action(philo, dt, "has died!", NULL);
		pthread_mutex_unlock(&dt->forks[philo->id].mtx);
		
	}
	else
	{
	while (philo->is_full == false && dt->dead == false)
	{
		while (dt->forks[philo->id].last_user == philo->id || dt->forks[philo->right % dt->total].last_user == philo->id)
			usleep(1);
		dine(philo, dt);
		if (philo->is_full == false && !dt->dead)
			sleep_think(philo);
	}
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
	pthread_mutex_init(&d->print_mtx, NULL);
	while (i < d->total)
	{
		if (pthread_mutex_init(&d->forks[i].mtx, NULL))
		{
			printf("mutex init has failed\n");
			return (1);
		}
		d->forks[i].last_user = -1;
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
	data.over = false;
	data.full_philos = 0;
	if (argc == 6)
	{
		data.meals = ft_atoi(argv[5]);
		data.meals_param = true;
	}
	
	i = 0;
	data.philos = malloc(sizeof(t_philo) * data.total);
	data.forks = malloc(sizeof(t_mutex) * data.total);

	if (init_mutexes(&data))
		return 1;
	init_philos(&data);
	start_ph(&data);
	return 0;
}