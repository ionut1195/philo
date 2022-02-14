/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 13:56:21 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/14 13:39:15 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static char	*get_action(t_action action)
{
	if (action == Fork)
		return ("has taken a fork");
	else if (action == Dead)
		return ("has died");
	else if (action == Think)
		return ("is thinking");
	else if (action == Full)
		return ("is full");
	else if (action == Sleep)
		return ("is sleeping");
	else if (action == Eat)
		return ("is eating");
	return (NULL);
}

void	print_action(t_philo *p, t_data *d, t_action act)
{
	pthread_mutex_lock(&d->print_mtx);
	if (!d->over)
		printf("%010ld	Philosopher %d %s\n",
			d->current_time - d->start_time, p->id + 1, get_action(act));
	if (act == Dead)
	{
		pthread_mutex_lock(&d->dead_mtx);
		d->over = true;
		pthread_mutex_unlock(&d->dead_mtx);
	}
	pthread_mutex_unlock(&d->print_mtx);
}

void	cleanup(t_data *d)
{
	free(d->forks);
	free(d->philos);
}
