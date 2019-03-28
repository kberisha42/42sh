/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emartine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:16:41 by emartine          #+#    #+#             */
/*   Updated: 2019/03/27 17:16:42 by emartine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>//XXX
#include "execution.h"

static int	save_dup(t_lexer_token *cur)
{
	if ((cur->fd_dup = dup(cur->fd_saved)) < 0)
		return (-1);
	printf("<%d=dup(%d)>\n", cur->fd_dup, cur->fd_saved);
	if (cur->fd_new < 0)
		close(cur->fd_saved);
	else if (dup2(cur->fd_new, cur->fd_saved) < 0)
		return (-1);
	printf("<dup2(%d,%d)>\n", cur->fd_new, cur->fd_saved);
	return (0);
}

static int	copy_path(t_lexer_token *cur, char *path)
{
	if (cur->redir_target->size > PATH_MAX)
		return (-1);
	ft_memmove(path, cur->redir_target->buffer, cur->redir_target->size);
	path[cur->redir_target->size] = '\0';
	return (0);
}

static int	error_restore(t_lexer_token *cmd, t_lexer_token *cur)
{
	cur->fd_saved = -1;
	command_redir_restore(cmd);
	return (-1);
}

int			command_redir(t_lexer_token *cmd)
{
	t_lexer_token	*cur;
	char			path[PATH_MAX + 1];
	int				oflag;

	cur = cmd->redir_head;
	while (cur)
	{
		if (cur->rtype == LESS || cur->rtype == DLESS || cur->rtype == DLESSDASH
				|| cur->rtype == LESSAND || cur->rtype == LESSGREAT)
			cur->fd_saved = cur->redir_input < 0 ? 0 : cur->redir_input;
		else
			cur->fd_saved = cur->redir_input < 0 ? 1 : cur->redir_input;
		if (cur->rtype == LESS || cur->rtype == GREAT || cur->rtype == DGREAT)
		{
			if (copy_path(cur, path) < 0)
				return (error_restore(cmd, cur));
			oflag = (cur->rtype == LESS) ? O_RDONLY : O_WRONLY | O_CREAT;
			if (cur->rtype == DGREAT)
				oflag |= O_APPEND;
			if ((cur->fd_new = open(path, oflag)) < 0)
				return (error_restore(cmd, cur));
		}
		else if (cur->rtype == LESSAND || cur->rtype == GREATAND)
		{
			if (cur->redir_target->buffer[0] == '-')
				cur->fd_new = -1;
			else
				cur->fd_new = ft_memtoi(cur->redir_target->buffer,
						cur->redir_target->size);
		}
		else
		{
			// TODO heredoc
			return (error_restore(cmd, cur));
		}
		if (save_dup(cur) < 0)
		{
			if (cur->rtype == LESS || cur->rtype == GREAT
					|| cur->rtype == DGREAT)
				close(cur->fd_new);
			return (error_restore(cmd, cur));
		}
		cur = cur->redir_next;
	}
	return (0);
}

void		command_redir_restore(t_lexer_token *cmd)
{
	t_lexer_token	*cur;

	cur = cmd->redir_head;
	while (cur && cur->fd_saved >= 0)
	{
		dup2(cur->fd_dup, cur->fd_saved);
		printf("<*dup2(%d,%d)>\n", cur->fd_dup, cur->fd_saved);
		close(cur->fd_dup);
		cur = cur->redir_next;
	}
}