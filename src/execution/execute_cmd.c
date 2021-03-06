/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kberisha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:47:32 by kberisha          #+#    #+#             */
/*   Updated: 2019/04/26 13:45:14 by aguillot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "twenty_one_sh.h"
#include "execution.h"
#include "expansions.h"

int			execute_simple_command(t_lexer_token *cmd)
{
	t_job			*new_job;

	new_job = NULL;
	if (!cmd || cmd->ptype != PARSER_COMMAND || command_expansions(cmd) < 0)
		return (-1);
	create_proc(&new_job, cmd);
	launch_job(&new_job);
	return (0);
}

int			execute_pipe(t_lexer_token *pipe_seq)
{
	t_lexer_token	*cur;
	t_job			*new_job;

	new_job = NULL;
	if (!(cur = pipe_seq))
		return (-1);
	if (pipe_seq->ptype != PARSER_PIPE)
		return (execute_simple_command(pipe_seq));
	while (cur && cur->ptype == PARSER_PIPE)
	{
		if (!cur->left || command_expansions(cur->left) < 0)
			return (-1);
		cur = cur->right;
	}
	if (command_expansions(cur) < 0)
		return (-1);
	cur = pipe_seq;
	while (cur && cur->ptype == PARSER_PIPE)
	{
		create_proc(&new_job, cur->left);
		cur = cur->right;
	}
	create_proc(&new_job, cur);
	launch_job(&new_job);
	return (0);
}

int			execute_and_or(t_lexer_token *and_or)
{
	t_lexer_token	*cur;

	if (!(cur = and_or))
		return (-1);
	if (and_or->ptype != PARSER_AND_OR)
		return (execute_pipe(and_or));
	if (execute_pipe(cur->left) < 0)
		return (-1);
	while (cur && cur->ptype == PARSER_AND_OR)
	{
		if ((cur->buffer[0] == '&' && g_shell.exit_code == 0)
				|| (cur->buffer[0] == '|' && g_shell.exit_code != 0))
		{
			if (execute_pipe(cur->right->ptype
						== PARSER_AND_OR ? cur->right->left : cur->right) < 0)
				return (-1);
		}
		cur = cur->right;
	}
	return (0);
}

static void	execute_amp(t_lexer_token *cur)
{
	if (create_background_job(cur) == 0)
	{
		if (execute_and_or(cur->left) < 0)
		{
			clean_shell();
			exit(125);
		}
		if (g_shell.background_signal)
		{
			clean_shell();
			signal(g_shell.background_signal, SIG_DFL);
			kill(getpid(), g_shell.background_signal);
			exit(125);
		}
		clean_shell();
		exit(g_shell.exit_code);
	}
}

int			execute(t_lexer *lex)
{
	t_lexer_token	*cmd;
	t_lexer_token	*cur;

	cmd = lex->root;
	if (!cmd)
		return (-1);
	if (cmd->ptype != PARSER_SEPARATOR)
		return (execute_and_or(cmd));
	cur = cmd;
	while (cur && cur->ptype == PARSER_SEPARATOR)
	{
		if (cur->buffer[0] == '&')
			execute_amp(cur);
		else if (execute_and_or(cur->left) < 0)
			return (-1);
		cur = cur->right;
	}
	if (cur)
		return (execute_and_or(cur));
	return (0);
}
