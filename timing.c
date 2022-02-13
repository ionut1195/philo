/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/24 08:03:47 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/25 09:21:58 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval		current_time;
	long	timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
	return (timestamp);
}

void	*keep_track(void *p)
{
	t_data *dt;

	dt = (t_data *)p;
	while (dt->over == false && dt->full_philos < dt->total)
		dt->current_time = get_time();
	return (NULL);
}

void	waiting(int milisecs, t_data *d)
{
	long curr;

	curr = get_time();
	// while (d->current_time < milisecs + curr && !d->over)
	while (get_time() < milisecs + curr && !d->over)
		;
}
void	*monitor_death(void *v)
{
	int id;
	t_data *d;

	d = (t_data *)v;
	while (!d->over)
	{
		id = -1;
		while (++id < d->total)
		{
			pthread_mutex_lock(&d->dead_mtx);
			if (/*d->current_time*/(get_time() > d->philos[id].will_die) && !d->philos[id].is_full)
			{
				print_action(&d->philos[id], d, "has died ------------------------------");
				
				d->over = true;
			}
			if (d->full_philos == d->total)
				d->over = true;
			pthread_mutex_unlock(&d->dead_mtx);
		}
		usleep(200);
		if (d->over)
			break ;
	}
	return (NULL);
}