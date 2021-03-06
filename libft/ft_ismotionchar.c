/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ismotionchar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:01:43 by emartine          #+#    #+#             */
/*   Updated: 2019/05/06 14:25:26 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_ismotionchar(int c)
{
	return (c == ' ' || c == '0' || c == 'b' || c == 'F' || c == 'l'
			|| c == 'W' || c == '^' || c == '$' || c == ';' || c == 'E'
			|| c == 'f' || c == 'T' || c == 'w' || c == '|' || c == ','
			|| c == 'B' || c == 'e' || c == 'h' || c == 't' || c == 'c');
}
