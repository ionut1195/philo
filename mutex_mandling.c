/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_mandling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 16:00:05 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/16 16:29:37 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t	lock;
int	shared_data = 42;

void *thread_func(void *arg)
{
	int i;

	for (i = 0; i < 6999999; i++)
	{
		pthread_mutex_lock(&lock);
		shared_data++;
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main(void)
{
	pthread_t	thread_id;
	void		*thread_res;
	int			i;

	pthread_mutex_init(&lock, NULL);
	pthread_create(&thread_id, NULL, thread_func, NULL);
	for (i = 0; i < 10;++i)
	{
		pthread_mutex_lock(&lock);
		printf("\rShared integer's value = %d \n", shared_data);
		pthread_mutex_unlock(&lock);
	}
	printf("\n");
	pthread_join(thread_id, &thread_res);
	pthread_mutex_destroy(&lock);
	return (0);
}