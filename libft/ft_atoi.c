/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schakor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 15:52:35 by schakor           #+#    #+#             */
/*   Updated: 2018/11/22 17:45:15 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int		ft_atoi(const char *s)
{
	int		ret;
	int		sign;

	while (*s == ' ' || (*s >= '\t' && *s <= '\r'))
		++s;
	sign = 1;
	if (*s == '+')
		++s;
	else if (*s == '-')
	{
		++s;
		sign = -1;
	}
	ret = 0;
	while (*s >= '0' && *s <= '9')
	{
		ret = ret * 10 + *s - '0';
		++s;
	}
	return (ret * sign);
}