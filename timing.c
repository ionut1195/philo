/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 08:03:47 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/21 20:51:21 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval		current_time;
	long				timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	return (timestamp);
}

void	*keep_track(void *p)
{
	t_data	*dt;

	dt = (t_data *)p;
	while (dt->over == FALSE && dt->full_philos < dt->total)
	{
		dt->current_time = get_time();
	}
	return (NULL);
}

void	waiting(int milisecs, t_data *d)
{
	long	curr;

	curr = get_time();
	while ((d->current_time < milisecs + curr) && !d->over)
		usleep(400);
}

void	*monitor_death(void *v)
{
	int		id;
	t_data	*d;

	d = (t_data *)v;
	while (!d->over)
	{
		id = -1;
		while (++id < d->total)
		{
			if ((d->current_time > d->philos[id].will_die)
				&& !d->philos[id].is_full)
				print_action(&d->philos[id], d, Dead);
			if (d->full_philos == d->total)
				d->over = TRUE;
		}
		usleep(200);
		if (d->over)
			break ;
	}
	return (NULL);
}
