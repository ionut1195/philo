/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 13:59:24 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/21 11:51:45 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_data *d, long will_die)
{
	int		i;

	i = 0;
	while (i < d->total)
	{
		d->philos[i].id = i;
		d->philos[i].pt = d;
		d->philos[i].is_full = FALSE;
		d->philos[i].meals_eaten = 0;
		d->philos[i].will_die = will_die;
		if (i % 2 == 0)
		{
			d->philos[i].even = i;
			d->philos[i].odd = (i + 1) % d->total;
		}
		else
		{
			d->philos[i].odd = i;
			d->philos[i].even = (i + 1) % d->total;
		}
		i++;
	}
}

void	start_ph(t_data *d)
{
	int	i;

	i = 0;
	pthread_create(&d->time_tracker, NULL, &keep_track, d);
	pthread_mutex_lock(&d->start_mtx);
	while (i < d->total)
	{
		if (pthread_create(&(d->philos[i].thread),
				NULL, &do_shit, (void *)&d->philos[i]))
			printf("thread couldn't be created");
		i++;
	}
	pthread_mutex_unlock(&d->start_mtx);
	pthread_create(&d->death_tracker, NULL, &monitor_death, d);
	i = 0;
	while (i < d->total)
	{
		if (pthread_join(d->philos[i].thread, NULL))
			printf("thread couldn't be joined\n");
		i++;
	}
	pthread_join(d->time_tracker, NULL);
	pthread_join(d->death_tracker, NULL);
}

void	init_mutexes(t_data *d)
{
	int	i;

	i = 0;
	pthread_mutex_init(&d->print_mtx, NULL);
	pthread_mutex_init(&d->dead_mtx, NULL);
	pthread_mutex_init(&d->start_mtx, NULL);
	while (i < d->total)
	{
		if (pthread_mutex_init(&d->forks[i].mtx, NULL))
			printf("mutex init has failed\n");
		d->forks[i].last_user = -1;
		i++;
	}
}

void	init_data(t_data *d, int argc, char *argv[])
{
	d->meals_param = FALSE;
	d->total = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_sleep = ft_atoi(argv[4]);
	d->dead = FALSE;
	d->over = FALSE;
	d->start_time = get_time();
	d->current_time = get_time();
	d->full_philos = 0;
	if (argc == 6)
	{
		d->meals = ft_atoi(argv[5]);
		d->meals_param = TRUE;
	}
	d->philos = malloc(sizeof(t_philo) * d->total);
	d->forks = malloc(sizeof(t_mutex) * d->total);
}
