/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/28 11:48:29 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	check_death(t_philo *p, t_data *d)
// {
// 	if (d->current_time > p->will_die)
// 		d->dead = true;
// 	else if (d->current_time + d->t_eat > p->will_die)
// 	{
// 		waiting(p->will_die - d->current_time + 1);
// 		d->dead = true;
// 	}
// }

void	print_action(t_philo *p, t_data *d, char *st)
{
	pthread_mutex_lock(&d->print_mtx);
	if (!d->over)
		printf("%010ld	Philosopher %d %s\n", /*d->current_time*/get_time() - d->start_time, p->id + 1, st);
	pthread_mutex_unlock(&d->print_mtx);
}

int	dine(t_philo *ph, t_data *dt)
{
	pthread_mutex_lock(&(dt->forks[ph->even].mtx));
	// pthread_mutex_lock(&dt->dead_mtx);
	print_action(ph, dt, "has taken a fork");
	// pthread_mutex_unlock(&dt->dead_mtx);
	pthread_mutex_lock(&(dt->forks[ph->odd].mtx));
	// pthread_mutex_lock(&dt->dead_mtx);
	print_action(ph, dt, "has taken another fork");
	print_action(ph, dt, "starts eating");
	ph->will_die = dt->current_time + dt->t_die;
	// pthread_mutex_unlock(&dt->dead_mtx);
	waiting(dt->t_eat, dt);
	ph->meals_eaten++;
	if (ph->meals_eaten >= ph->pt->meals && dt->meals_param)
	{
		ph->is_full = true;
		print_action(ph, dt, "is full");
		dt->full_philos++;
	}

	pthread_mutex_unlock(&(dt->forks[ph->even].mtx));
	pthread_mutex_unlock(&(dt->forks[ph->odd].mtx));
	dt->forks[ph->even].last_user = ph->id;
	dt->forks[ph->odd].last_user = ph->id;
	return (1);
}

void	sleep_think(t_philo *philo)
{
	print_action(philo, philo->pt, "starts sleeping");
	waiting(philo->pt->t_sleep, philo->pt);
	print_action(philo, philo->pt, "starts thinking");
}

void	kill_one(t_philo *philo, t_data *dt)
{
	pthread_mutex_lock(&dt->forks[philo->id].mtx);
	print_action(philo, dt, "has taken a fork");
	waiting(dt->t_die, dt);
	print_action(philo, dt, "has died!");
	pthread_mutex_unlock(&dt->forks[philo->id].mtx);
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;

	philo = (t_philo *)shit;
	dt = philo->pt;
	// pthread_mutex_lock(&dt->start_mtx);
	// pthread_mutex_unlock(&dt->start_mtx);
	if (philo->id % 2 == 0 && dt->total > 1)
		waiting(dt->t_eat, dt);
	if (dt->total == 1)
		kill_one(philo, dt);
	else
	{
		while (!philo->is_full && !dt->over)
		{
			// while (dt->forks[philo->even].last_user == philo->id
			// 	&& dt->forks[philo->odd].last_user == philo->id)
			// 		;
			dine(philo, dt);
			if (!philo->is_full && !dt->over)
				sleep_think(philo);
		}
	}
	return (NULL);
}

void	init_philos(t_data *d, long will_die)
{
	int		i;

	i = 0;
	while (i < d->total)
	{
		d->philos[i].id = i;
		d->philos[i].pt = d;
		d->philos[i].is_full = false;
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
		// d->philos[i].even = i;
		// d->philos[i].odd = (i + 1) % d->total;
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
	d->meals_param = false;
	d->total = ft_atoi(argv[1]);
	d->t_die = ft_atoi(argv[2]);
	d->t_eat = ft_atoi(argv[3]);
	d->t_sleep = ft_atoi(argv[4]);
	d->dead = false;
	d->over = false;
	d->start_time = get_time();
	d->full_philos = 0;
	if (argc == 6)
	{
		d->meals = ft_atoi(argv[5]);
		d->meals_param = true;
	}
	d->philos = malloc(sizeof(t_philo) * d->total);
	d->forks = malloc(sizeof(t_mutex) * d->total);
}

int	is_number(char *p)
{
	int	i;

	i = 0;
	while (p[i])
	{
		if (p[i] < '0' && p[i] > '9')
			return (0);
		i++;		
	}
	return (1);
}

int	invalid_input(int argc, char *argv[])
{
	int i;

	i = 0;
	while (i < argc)
	{
		if (!is_number(argv[i]))
			return(1);
		i++;
	}
	return (0);	
}

void	cleanup(t_data *d)
{
	// int	i;

	// i = 0;
	// while (i++ < d->total)
	// 	pthread_mutex_destroy(&d->forks[i].mtx);
	// pthread_mutex_destroy(&d->print_mtx);
	// pthread_mutex_destroy(&d->dead_mtx);
	// pthread_mutex_destroy(&d->start_mtx);
	free(d->forks);
	free(d->philos);
	
}
// void	monitor_death(t_data *d)
// {
// 	int id;

// 	while (!d->over)
// 	{
// 		id = -1;
// 		while (++id < d->total)
// 		{
// 			pthread_mutex_lock(&d->dead_mtx);
// 			if (/*d->current_time*/(get_time() > d->philos[id].will_die) && !d->philos[id].is_full)
// 			{
// 				print_action(&d->philos[id], d, "has died ------------------------------");
				
// 				d->over = true;
// 			}
// 			if (d->full_philos == d->total)
// 				d->over = true;
// 			pthread_mutex_unlock(&d->dead_mtx);
// 		}
// 		usleep(200);
// 		if (d->over)
// 			break ;
// 	}
// }

int	main(int argc, char *argv[])
{
	t_data	data;

	if ((argc < 5 || argc > 6) || invalid_input(argc, argv))
	{
		printf("Wrong arguments!\n");
		return (EXIT_FAILURE);
	}
	init_data(&data, argc, argv);
	init_mutexes(&data);
	init_philos(&data, get_time() + data.t_die);
	start_ph(&data);
	// monitor_death(&data);
	cleanup(&data);
	return (0);
}
