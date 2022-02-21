/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 10:30:07 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/21 21:23:05 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum s_bool { FALSE, TRUE }	t_bool;
typedef enum s_action {Think , Sleep, Eat, Full, Dead, Fork}	t_action;
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
}	t_philo;

typedef struct s_mutex
{
	int				last_user;
	pthread_mutex_t	mtx;
}	t_mutex;

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
	t_philo			*philos;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	start_mtx;
	t_mutex			*forks;
	pthread_mutex_t	dead_mtx;
}	t_data;

void	*do_shit(void *shit);
int		ft_atoi(const char *nptr);
void	print_action(t_philo *p, t_data *d, t_action act);
// timing.c
void	*keep_track(void *p);
void	waiting(int milisecs, t_data *d);
long	get_time(void);
void	*monitor_death(void *v);
// input_check.c
int		invalid_input(int argc, char *argv[]);
// utils.c
void	print_action(t_philo *p, t_data *d, t_action act);
void	cleanup(t_data *d);
// inits.c
void	init_philos(t_data *d, long will_die);
void	start_ph(t_data *d);
void	init_mutexes(t_data *d);
void	init_data(t_data *d, int argc, char *argv[]);
#endif