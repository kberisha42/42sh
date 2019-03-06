/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 16:06:31 by emartine          #+#    #+#             */
/*   Updated: 2019/03/06 15:59:46 by schakor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

#include <stdio.h> // XXX
#include <unistd.h> // XXX

static int			(*g_lexer_func[])(t_lexer *) = {
	unquoted_backslash_newline,
	heredoc,
	operator_append,
	operator_end,
	quoting,
	expansion,
	operator_new,
	unquoted_blank,
	word_append,
	comment,
	word_new,
};

static void			lexer_init(t_lexer *lex, uint8_t *line, size_t line_size)
{
	lex->line = line;
	lex->line_size = line_size;
	lex->i = 0;
	if (lex->init)
	{
		lex->line_y++;
		lex->i = 0;
		lex->backslash_newline = 0;
		// TODO add newline if quoted
	}
	else
		lex->init = 1;
}

int					lexer_read(t_lexer *lex, uint8_t *line, size_t line_size)
{
	size_t	f;
	int		r;

	lexer_init(lex, line, line_size);
	while (lex->i < lex->line_size)
	{
		f = 0;
		while (f < sizeof(g_lexer_func) / sizeof(g_lexer_func[0]))
		{
			if ((r = g_lexer_func[f](lex)) < 0)
				return (-1);
			if (r == 0)
				break ;
			f++;
		}
		if (f == sizeof(g_lexer_func) / sizeof(g_lexer_func[0]))
		{
			lex->impl_error = 1;
			return (-1);
		}
		lex->i++;
	}
	if (line_end(lex) < 0)
		return (-1);
	//return (parser(lex));
	return (0);
}

void				lexer_destroy(t_lexer *lex)
{
	t_lexer_token	*current;
	t_lexer_token	*previous;

	current = lex->head;
	while (current)
	{
		previous = current;
		current = current->next;
		if (previous->type == TYPE_WORD || previous->type == TYPE_OPERATOR)
			free(previous->buffer);
		// TODO free heredoc
		free(previous);
	}
	lex->head = NULL;
	lex->foot = NULL;
	// TODO destroy parser?
}

int					token(t_lexer *lex, enum e_lexer_type type)
{
	t_lexer_token	*t;

	if (NULL == (t = malloc(sizeof(*t))))
		return (-1);
	ft_memset(t, 0, sizeof(*t));
	if (type == TYPE_WORD || type == TYPE_OPERATOR)
	{
		if (NULL == (t->buffer = malloc(1)))
		{
			free(t);
			return (-1);
		}
		t->buffer[0] = lex->line[lex->i];
		t->buffer_size = 1;
	}
	else if (type == TYPE_HEREDOC) // FIXME called only when heredoc buffer == NULL?
	{
		//if (t->heredoc_queue[0].buffer == NULL)
		//if (NULL == (t->heredoc_queue[0].buffer = malloc(heredoc_queue[0].buffer_size + heredoc_queue[0].i - lex->i)))
		//if (NULL == (t->heredoc_queue[0].buffer = malloc(heredoc_queue[0].i - lex->i)))
	}
	t->line_x = lex->i;
	t->type = type;
	t->previous = lex->foot;
	t->next = NULL;
	if (lex->head)
		lex->foot->next = t;
	else
		lex->head = t;
	lex->foot = t;
	return (0);
}

/*
** assumes a token exists (tests anyway)
*/

int					append(t_lexer *lex)
{
	uint8_t	*t;

	if (!lex->foot || lex->foot->buffer_size == 0
			|| lex->foot->type == TYPE_HEREDOC)
	{
		lex->impl_error = 1;
		return (-1);
	}
	if (NULL == (t = malloc(lex->foot->buffer_size + 1)))
		return (-1);
	ft_memmove(t, lex->foot->buffer, lex->foot->buffer_size);
	free(lex->foot->buffer);
	t[lex->foot->buffer_size] = lex->line[lex->i];
	lex->foot->buffer = t;
	lex->foot->buffer_size++;
	return (0);
}
