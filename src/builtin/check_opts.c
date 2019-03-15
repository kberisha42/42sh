/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_opts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 14:18:16 by khsadira          #+#    #+#             */
/*   Updated: 2019/03/15 14:34:33 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static void		print_flag_error(char *str, int i, char *cmd)
{
	char	*buf;
	char	*buf2;

	buf = ft_strdup("-");
	buf2 = ft_strsub(str, i, 1);
	buf = ft_strfjoin(buf, buf2, 2);
	put_error("sh: ", cmd, buf, "invalid option");
}

static t_opts	*empty_flag(char *arg, t_opts *flag, char *cmd)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]))
		{
			print_flag_error(arg, i, cmd);
			return (NULL);
		}
		else if (arg[i] == 'a')
			flag->a = 1;
		else if (arg[i] == 'p')
			flag->p = 1;
		else if (arg[i] == 'P')
			flag->pp = 1;
		else if (arg[i] == 'f')
			flag->f = 1;
		else if (arg[i] == 'l')
			flag->l = 1;
		else if (arg[i] == 't')
			flag->t = 1;
		else if (arg[i] == 'i')
			flag->i = 1;
		i++;
	}
	return (flag);
}

static t_opts	*init_flag(t_opts *flag)
{
	flag->a = 0;
	flag->p = 0;
	flag->pp = 0;
	flag->f = 0;
	flag->l = 0;
	flag->t = 0;
	flag->i = 0;
	return (flag);
}

int		check_opts(char **arg, t_opts *flag)
{
	int		i;
	
	i = 1;
	flag = init_flag(flag);
	while (arg[i])
	{
		if (arg[i][0] == '-')
		{
			if ((flag = empty_flag(arg[i], flag, arg[0])))
				return (-1);
		}
		else if (ft_strequ(arg[i], "--"))
			return (i + 1);
		else
			return (i);
	}
	return (i);
}
