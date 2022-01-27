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
	while (dt->dead == false && dt->full_philos < dt->total)
		dt->current_time = get_time();	
	return (NULL);
}

void	waiting(int milisecs)
{
	long curr;

	curr = get_time();
	while (get_time() < milisecs + curr)
		;
}
