/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/21 21:36:49 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	dine(t_philo *ph, t_data *dt)
{
	pthread_mutex_lock(&(dt->forks[ph->even].mtx));
	print_action(ph, dt, Fork);
	pthread_mutex_lock(&(dt->forks[ph->odd].mtx));
	print_action(ph, dt, Fork);
	print_action(ph, dt, Eat);
	ph->will_die = dt->current_time + dt->t_die;
	waiting(dt->t_eat, dt);
	ph->meals_eaten++;
	if (ph->meals_eaten >= ph->pt->meals && dt->meals_param)
	{
		ph->is_full = TRUE;
		print_action(ph, dt, Full);
		dt->full_philos++;
	}
	pthread_mutex_unlock(&(dt->forks[ph->even].mtx));
	pthread_mutex_unlock(&(dt->forks[ph->odd].mtx));
}

static void	sleep_think(t_philo *philo)
{
	if (philo->is_full)
		return ;
	print_action(philo, philo->pt, Sleep);
	waiting(philo->pt->t_sleep, philo->pt);
	print_action(philo, philo->pt, Think);
}

void	*do_shit(void *shit)
{
	t_philo	*philo;
	t_data	*dt;

	philo = (t_philo *)shit;
	dt = philo->pt;
	pthread_mutex_lock(&dt->start_mtx);
	pthread_mutex_unlock(&dt->start_mtx);
	if (philo->id % 2 == 0 && dt->total > 1)
		waiting(dt->t_eat, dt);
	while (!philo->is_full && !dt->over)
	{
		dine(philo, dt);
		sleep_think(philo);
	}
	return (NULL);
}

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
	cleanup(&data);
	return (0);
}
