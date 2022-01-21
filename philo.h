/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 10:30:07 by itomescu          #+#    #+#             */
/*   Updated: 2022/01/21 16:01:46 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
typedef enum s_bool { false, true } t_bool;
typedef struct s_philo
{
	int				id;
	struct s_data	*pt;
	pthread_t		thread;
	t_bool			can_eat;
	t_bool			is_full;
	
} t_philo;

typedef struct s_data
{
	int				meals;
	int				total;
	pthread_mutex_t	*forks;
	t_philo 		*philos;
	pthread_mutex_t	function_access;
} t_data;

long long unsigned int	get_current_timestamp(void);
int						ft_atoi(const char *nptr);
#endif