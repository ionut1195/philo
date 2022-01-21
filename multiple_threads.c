/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:01:11 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/17 07:29:41 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_func(void *arg)
{
	int *inc;

	inc = (int *)arg;
	for (int i = 0; i < 5; i++)
		printf("Hello World! (thread %d)\n", *inc);
	return NULL;
}

int main(void)
{
	pthread_t	thread_id[10];
	void		*thread_result;
	pthread_mutex_t	mutx;
	int			i;
	int			v;
	char		*msg;

	i = 0;
	v = 42;
	while (i < 10)
	{
		pthread_create(&thread_id[i], NULL, thread_func, &thread_id[i]);
		i++;
	}
	thread_id[i] = NULL;
	i = 0;
	while (i++ < 10)
	{
		pthread_join(thread_id[i], &thread_result);
		msg = (char *)thread_result;
		printf("got %s back from the thread\n", msg);
		free(thread_result);
	}
	
	return (0);
}