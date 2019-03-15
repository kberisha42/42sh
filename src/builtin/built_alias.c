/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_alias.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 17:07:38 by khsadira          #+#    #+#             */
/*   Updated: 2019/03/15 11:48:08 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int	start_arg_alias(char **arg, int *opts)
{
	int	i;
	
	i = 1;
	*opts = 0;
	while (arg[i])
	{
		if (ft_strequ(arg[i], "-p"))
			*opts = 1;
		else if (ft_strequ(arg[i], "--"))
			return (i + 1);
		else if ((arg[i][0] == '-') && !ft_strequ(arg[i], "-p"))
		{
			put_error("sh: ", "alias", arg[i], "invalid option\n");
			return (-1);
		}
		else
			return (i);
		i++;
	}
	return (i);
}

int		alias_var_egual(char *arg, t_alias **alias)
{
	t_alias	*tmp;
	char	*name;
	char	*value;
	int		i;

	if (!(i = ft_strichr(arg, '=')))
		return (0);
	value = ft_strsub(arg, i + 1, ft_strlen(arg) - i);
	tmp = *alias;
	while (tmp)
	{
		if (ft_strequ(tmp->name, arg))
		{
			ft_strdel(&(tmp->value));
			tmp->value = value;
			return (1);
		}
		tmp = tmp->next;
	}
	name = ft_strsub(arg, 0, i);
	tmp = new_alias(name, value);
	*alias = addlast_alias(*alias, tmp);
	ft_strdel(&name);
	ft_strdel(&value);
	return (1);
}

int		alias_var(char *arg, t_alias **alias)
{
	t_alias	*tmp;

	tmp = *alias;
	while (tmp)
	{
		if (ft_strequ(arg, tmp->name))
		{
			ft_putstr("alias ");
			ft_putstr(tmp->name);
			ft_putchar('=');
			ft_putstr(tmp->value);
			return (0);
		}
		tmp = tmp->next;
	}
	ft_putstr_fd("alias: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not found\n", 2);
	return (1);
}

int		built_alias(char **arg, t_alias **alias)
{
	int	i;
	int	opts;

	if ((i = start_arg_alias(arg, &opts)) == -1)
	{
		ft_putstr_fd("alias: usage: alias [-p] [name[=value] ... ]\n", 2);
		return (0);
	}
	if (opts == 1)
		print_alias(*alias);
	while (arg[i])
	{
		if (ft_strchr(arg[i], '='))
			alias_var_egual(arg[i], alias);
		else
			alias_var(arg[i], alias);
		i++;
	}
	return (0);
}
