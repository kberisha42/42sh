/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vi_delete_motion_c.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schakor <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:16:41 by schakor           #+#    #+#             */
/*   Updated: 2019/04/05 14:55:51 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static t_keymap		g_motion_keymap[MOTION_KEYMAP_SIZE] = {
	{" ", 1, vi_move_next_char},
	{"l", 1, vi_move_next_char},
	{"h", 1, vi_move_prev_char},
	{"w", 1, vi_forward_word},
	{"W", 1, vi_forward_bigword},
	{"e", 1, vi_end_word},
	{"E", 1, vi_end_bigword},
	{"b", 1, vi_backward_word},
	{"B", 1, vi_backward_bigword},
	{"^", 1, vi_move_first_nonblank},
	{"$", 1, vi_go_to_end},
	{"0", 1, go_to_home},
	{"|", 1, vi_move_counth_char},
	{"f", 1, vi_search_char_after},
	{"F", 1, vi_search_char_before},
	{"T", 1, vi_search_char_after_before},
	{"t", 1, vi_search_char_before_after},
	{";", 1, repeat_last_search_char},
	{",", 1, repeat_last_search_char_reverse},
	{"c", 1, vi_clear_line_insert}
};

static int		begin_is_before_end(t_char *begin, t_char *end)
{
	t_char		*tmp;

	tmp = begin;
	while (tmp)
	{
		if (tmp == end)
			return (TRUE);
		tmp = tmp->next;
	}
	return (FALSE);
}

static void		delete_from_to(t_char *begin, t_char *end)
{
	t_char		*tmp;
	int			begin_before_end;

	begin_before_end = begin_is_before_end(begin, end);
	if (begin_before_end)
	{
		while (begin && begin != end)
		{
			tmp = begin;
			begin = begin->next;
			delete_char_from_list(tmp);
		}
	}
	else
	{
		begin = begin->prev;
		while (begin && begin != end)
		{
			tmp = begin;
			begin = begin->prev;
			delete_char_from_list(tmp);
		}
	}
	if (begin_before_end && end && !end->is_prompt)
		delete_char_from_list(end);
}

void			vi_delete_c(void)
{
	vi_delete_motion();
	vi_insert_mode();
}

void			vi_delete_motion(void)
{
	char	c;
	int		i;
	t_char	*beg;

	if (read(STDIN_FILENO, &c, 1) < 0 || !ft_ismotionchar(c))
		return ;
	if (!g_shell.edit.point_char || !(beg = g_shell.edit.point_char->next))
		return ;
	i = 0;
	if (c == '0' || c == '^' || c == '$' || c == 'c')
		g_shell.edit.count = 1;
	while (g_motion_keymap[i].seq && i < MOTION_KEYMAP_SIZE)
	{
		if (g_motion_keymap[i].seq[0] == c)
			g_motion_keymap[i].funckey();
		i++;
	}
	if (beg == g_shell.edit.point_char->next || c == 'c')
		return ;
	delete_from_to(beg, g_shell.edit.point_char->next);
	update_all_pos();
	clean_and_print();
}

void			vi_yank_motion(void)
{
	char	c;
	int		i;
	t_char	*cpy_beg;
	t_char	*cpy_end;

	if (read(STDIN_FILENO, &c, 1) < 0 || !ft_ismotionchar(c))
		return ;
	if (!g_shell.edit.point_char || !(cpy_beg = g_shell.edit.point_char->next))
		return ;
	i = -1;
	while (g_motion_keymap[i].seq && ++i < MOTION_KEYMAP_SIZE)
	{
		if (g_motion_keymap[i].seq[0] == c)
			g_motion_keymap[i].funckey();
	}
	if (cpy_beg == (cpy_end = g_shell.edit.point_char->next))
		return ;
	if (g_shell.edit.cpy_buff)
		free(g_shell.edit.cpy_buff);
	if (begin_is_before_end(cpy_beg, cpy_end))
		g_shell.edit.cpy_buff = build_cpy_buff(cpy_beg, cpy_end);
	else
		g_shell.edit.cpy_buff = build_cpy_buff(cpy_end, cpy_beg->prev);
	g_shell.edit.point_char = cpy_beg->prev;
	clean_and_print();
}
