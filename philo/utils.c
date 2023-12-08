/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:12:37 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/12/08 18:12:40 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Takes a string and returns an integer.
If param(str) is not a positive integer (ej. negative int or char):
return -2
*/
int	positive_atoi(const char *str)
{
	int				i;
	int				is_char;
	unsigned long	res;

	i = 0;
	is_char = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		is_char = 0;
		i++;
	}
	if (str[i] == '-' || is_char || res > 2147483647)
		return (-2);
	return (res);
}
