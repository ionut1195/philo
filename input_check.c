/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itomescu <itomescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 13:52:41 by itomescu          #+#    #+#             */
/*   Updated: 2022/02/21 11:51:46 by itomescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_number(char *p)
{
	int	i;

	i = 0;
	while (p[i])
	{
		if (p[i] < '0' || p[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	invalid_input(int argc, char *argv[])
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_number(argv[i]))
			return (1);
		i++;
	}
	return (0);
}
