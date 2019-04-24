/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_line_to_hist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 11:59:38 by aguillot          #+#    #+#             */
/*   Updated: 2019/04/23 19:18:41 by aguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static uint8_t	*check_buff_end_for_b(uint8_t *buff, size_t buff_size)
{
	if (buff_size <= 0)
		return (buff);
	if (buff[buff_size - 1] == '\\')
		buff[buff_size - 1] = '\0';
	return (buff);
}

static void		append_line_to_hist(int is_multi, uint8_t *buff,\
	size_t buff_size)
{
	uint8_t *hist_buff;
	uint8_t	*tmp;

	if (!(tmp = (uint8_t*)ft_strdup((char*)buff)))
		fatal_exit(SH_ENOMEM);
	tmp = check_buff_end_for_b(tmp, buff_size);
	hist_buff = g_shell.hist.history->buf;
	if (is_multi == MULTI)
		hist_buff = (uint8_t*)ft_strfjoin((char*)hist_buff, "\n", 0);
	hist_buff = (uint8_t*)ft_strfjoin((char*)hist_buff, (char*)tmp, 0);
	g_shell.hist.history->buf = hist_buff;
	free(tmp);
}

static void		create_new_hist_line(uint8_t *buff, size_t buff_size)
{
	uint8_t		*tmp;
	t_history	*curr;

	if (!(curr = find_last_hist_line()))
		curr = g_shell.hist.history;
	if (!(tmp = (uint8_t*)ft_strdup((char*)buff)))
		fatal_exit(SH_ENOMEM);
	tmp = check_buff_end_for_b(tmp, buff_size);
	g_shell.hist.history = rl_add_hist(curr, rl_new_hist(tmp));
	g_shell.hist.history_size++;
	resize_history(g_shell.hist.history);
	g_shell.hist.history_save = -1;
	free(tmp);
}

void			rl_line_to_hist(int add_to_hist, uint8_t *buff, size_t b_size)
{
	if (g_shell.edit.prompt_id != HEREDOC_PROMPT && add_to_hist == 1)
	{
		if (g_shell.edit.prompt_id == QUOTE_PROMPT)
			append_line_to_hist(MULTI, buff, b_size);
		else if (g_shell.edit.prompt_id == BACKSLASH_PROMPT)
			append_line_to_hist(1, buff, b_size);
		else
			create_new_hist_line(buff, b_size);
	}
}