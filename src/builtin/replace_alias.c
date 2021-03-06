/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_alias.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 18:30:04 by khsadira          #+#    #+#             */
/*   Updated: 2019/05/07 04:15:53 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		word_separator(char buff)
{
	if (buff == ' ' || buff == '\n' || buff == '\t' || buff == '='
			|| buff == '[' || buff == ']' || buff == '?' || buff == '#'
			|| buff == '\\' || buff == '/' || buff == '~' || buff == '$'
			|| buff == '>' || buff == '<')
		return (1);
	return (0);
}

static int		cmd_separator(char buff)
{
	if (buff == '|' || buff == ';' || buff == '&')
		return (1);
	return (0);
}

static int		allowed_char(char buff)
{
	if (cmd_separator(buff) || word_separator(buff))
		return (0);
	return (1);
}

static char		*free_var_alias(char **dup, char **buff, int loop)
{
	if (loop >= 500)
	{
		ft_strdel(buff);
		return (*dup);
	}
	ft_strdel(dup);
	return (*buff);
}

char			*replace_alias(char *buff, t_alias *alias, int i, int n)
{
	char	*dup;
	int		loop;

	i = 0;
	loop = 0;
	dup = ft_strdup(buff);
	while (buff[i] && loop++ < 500)
	{
		if (word_separator(buff[i]))
			i++;
		else if (cmd_separator(buff[i]))
		{
			i++;
			n = 0;
		}
		else if (n == 0)
		{
			while (buff[i + n] && allowed_char(buff[i + n]))
				n++;
			buff = manage_alias_buff(buff, alias, &i, &n);
		}
		else
			i++;
	}
	return (free_var_alias(&dup, &buff, loop));
}
