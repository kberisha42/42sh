/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi_insert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schakor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:16:41 by schakor           #+#    #+#             */
/*   Updated: 2019/04/05 14:55:51 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void 	vi_append_mode(void)
{
	move_right();
	g_shell.edit.edit_mode = MODE_VI_INSERT;
}

void 		vi_append_eol(void)
{
	go_to_end();
	g_shell.edit.edit_mode = MODE_VI_INSERT;
}

void 	vi_insert_mode(void)
{
	g_shell.edit.edit_mode = MODE_VI_INSERT;
}

void 	vi_insert_bol(void)
{
	go_to_home();
	g_shell.edit.edit_mode = MODE_VI_INSERT;
}

void 	vi_insert_replace(void)
{

}

void 		vi_replace_char(void)
{
	int		count;
	char	c;

	if (read(STDIN_FILENO, &c, 1) < 0)
		fatal_exit(SH_EINVAL);
	count = g_shell.edit.count;
	while (count--)
	{
		if (read(STDIN_FILENO, &c, 1) < 0)
			fatal_exit(SH_EINVAL);
	}
}