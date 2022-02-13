/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 10:30:07 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/27 16:30:48 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
typedef enum s_bool { false, true } t_bool;
typedef struct s_philo
{
	int				id;
	int				even;
	int				odd;
	int				meals_eaten;
	long			will_die;
	struct s_data	*pt;
	pthread_t		thread;
	t_bool			is_full;
} t_philo;
typedef struct s_mutex
{
	int				last_user;
	pthread_mutex_t	mtx;
} t_mutex;

typedef struct s_data
{
	int				meals;
	int				total;
	int				full_philos;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			current_time;
	long			start_time;
	t_bool			meals_param;
	t_bool			dead;
	t_bool			over;
	pthread_t		time_tracker;
	pthread_t		death_tracker;
	t_philo 		*philos;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	start_mtx;
	t_mutex			*forks;
	pthread_mutex_t dead_mtx;
} t_data;

long	get_time(void);
int		ft_atoi(const char *nptr);
void	*keep_track(void *p);
void	waiting(int milisecs, t_data *d);
void	print_action(t_philo *p,t_data *d, char *st);
void	*monitor_death(void *v);
#endif