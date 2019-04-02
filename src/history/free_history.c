/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 12:09:09 by khsadira          #+#    #+#             */
/*   Updated: 2019/03/26 15:46:26 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void	free_history(t_history *hist)
{
	t_history *tmp;
	
	while (hist)
	{
		tmp = hist;
		if (hist->buf)
			free(hist->buf);
		hist = hist->bfr;
		free(tmp);
	}
}
