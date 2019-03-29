/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 12:00:25 by khsadira          #+#    #+#             */
/*   Updated: 2019/03/29 17:29:59 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		swap_var(t_envl **envl, t_envl **h_env, t_envl **tmp, int i)
{
	if (i)
		*envl = (*h_env)->next;
	else
		(*tmp)->next = (*h_env)->next;
	(*h_env)->next = NULL;
	free_envl(*h_env);
	return (0);
}

static int		unset_var(char *arg, t_envl **envl)
{
	t_envl	*h_env;
	t_envl	*tmp;

	h_env = *envl;
	if (ft_strequ(arg, h_env->name) && h_env->read_only == 0)
		return (swap_var(envl, &h_env, &tmp, 1));
	tmp = h_env;
	h_env = h_env->next;
	while (h_env)
	{
		if (ft_strequ(arg, h_env->name) && h_env->read_only == 0)
			return (swap_var(envl, &h_env, &tmp, 0));
		tmp = h_env;
		h_env = h_env->next;
	}
	return (0);
}

int				built_unset(char **arg, t_envl *envl)
{
	int	i;
	int	len;

	len = 0;
	i = 1;
	if (envl)
	{
		free_envl(g_shell.envl);
		g_shell.envl = dup_envl(envl);
	}
	while (arg[len])
		len++;
	if (len < 2)
	{
		ft_putendl_fd("unset: Too few argument", 2);
		return (1);
	}
	while (arg[i])
	{
		unset_var(arg[i], &g_shell.envl);
		i++;
	}
	return (0);
}
