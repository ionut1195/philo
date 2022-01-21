/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mauled.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 11:58:30 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/21 13:17:59 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void	sleeping()
{
	usleep(100);
}

int	grab_fork(t_data *data)
{
	char *message[2][30];
	*message[0] = "thread took left fork\n";
	*message[1] = "thread took right fork\n";
	pthread_mutex_lock(&data->mutexes[data->id]);
	printf("%d %s",data->id, *message[0]);
	pthread_mutex_lock(&data->mutexes[(data->id + 1) % data->total]);
	printf("%d %s",data->id, *message[1]);
	printf("Philosopher %d starts eating\n\n", data->id);
	pthread_mutex_unlock(&data->mutexes[data->id]);
	pthread_mutex_unlock(&data->mutexes[(data->id + 1) % data->total]);
	return (1);
}

void	*do_shit(void *shit)
{
	t_data *data;

	data = (t_data *)shit;
	grab_fork(data);
	return (NULL);
}

int	init_philos(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->total)
	{
		if (i % 2 == 0)
		{
			if (pthread_create(&d->threads[i], NULL, &do_shit, (void *)d))
			{
				printf("thread couldn't be created");
				return (1);
			}
			else
			{
				d->philos[i].id = i;
				d->id = i;
				printf("thread created\n");
			}
		}
		i++;
	}
	i = 0;
	usleep(10000);
	while (i < d->total)
	{
		if (i % 2 != 0)
		{
			if (pthread_create(&d->threads[i], NULL, &do_shit, (void *)d))
			{
				printf("thread couldn't be created");
				return (1);
			}
			else
			{
				d->philos[i].id = i;
				d->id = i;
				printf("thread created\n");
			}
		}
		i++;
	}
	return (0);
}

// void	*funky(void *p)
// {
// 	t_data *d;

// 	d = (t_data *)p;
// 	if (!grab_fork(&d->mtx[d->id - 1], &d->philos[d->id],0, d->id))
// 		printf("couldn't get the fork");
// 	if (!grab_fork(&d->mtx[d->id], &d->philos[d->id],1, d->id))
// 		printf("couldn't get the fork");
	
// 	return NULL;
// }


int	init_mutexes(t_data *d)
{
	int i;

	i = 0;
	while (i < d->total)
	{
		if (pthread_mutex_init(&d->mutexes[i], NULL))
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
	data.total = ft_atoi(argv[1]);
	i = 0;
	data.threads = malloc(sizeof(pthread_t *) * data.total);
	data.philos = malloc(sizeof(t_philo *) * data.total);
	data.mutexes = malloc(sizeof(pthread_mutex_t) * data.total);
	if (init_mutexes(&data))
		return 1;
	init_philos(&data);
	usleep(1000);
	i = 0;
	while (i < data.total)
	{
		if (pthread_join(data.threads[i], NULL))
		{
			printf("thread couldn't be joined\n");
			return 2;
		}
		else
			printf("thread joined\n");
		i++;
	}
	// i = 0;
	// while (i++ < ft_atoi(argv[1]))
	// 	free(threads[i]);
	
	
	return 0;
}