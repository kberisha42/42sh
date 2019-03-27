/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_controler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguillot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 16:58:34 by aguillot          #+#    #+#             */
/*   Updated: 2019/03/25 16:58:36 by aguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void	init_prompt(int prompt_id)
{
	size_t	i;

	i = 0;
	if (prompt_id == BASIC_PROMPT)
		while (BASIC_PRMPT[i])
			add_char_to_list((uint8_t *)&(BASIC_PRMPT[i++]), 1, 1);
	else if (prompt_id == QUOTE_PROMPT)
		while (QUOTE_PRMPT[i])
			add_char_to_list((uint8_t *)&(QUOTE_PRMPT[i++]), 1, 1);
	else if (prompt_id == BACKSLASH_PROMPT)
		while (BACKSLASH_PRMPT[i])
			add_char_to_list((uint8_t *)&(BACKSLASH_PRMPT[i++]), 1, 1);
	else if (prompt_id == HEREDOC_PROMPT)
		while (HEREDOC_PRMPT[i])
			add_char_to_list((uint8_t *)&(HEREDOC_PRMPT[i++]), 1, 1);
	g_shell.edit.mark = g_shell.edit.point_char;
}

uint8_t *prompt_to_buff(t_char_list *list)
{
	uint8_t *prompt;
	size_t	len;
	int		i;
	t_char 	*curr;

	len = 0;
	i = 0;
	curr = list->head;
	while (curr && curr->is_prompt)
	{
		len++;
		curr = curr->next;
	}
	if (!(prompt = (uint8_t*)malloc(sizeof(uint8_t) * (len + 1))))
		fatal_exit(SH_ENOMEM);
	prompt[len] = '\0';
	curr = list->head;
	while (curr && curr->is_prompt)
	{
		ft_memmove(prompt + i, curr->charac, curr->len);
		i += curr->len;
		curr = curr->next;
	}
	return (prompt);
}