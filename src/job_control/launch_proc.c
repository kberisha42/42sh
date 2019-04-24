/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_proc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khsadira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 18:58:21 by khsadira          #+#    #+#             */
/*   Updated: 2019/04/24 15:29:27 by khsadira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void	set_signal_dfl(void)
{
	//signal(SIGINT, SIG_DFL);
	//signal(SIGQUIT, SIG_DFL);
	//signal(SIGTSTP, SIG_DFL);
	//signal(SIGTTIN, SIG_DFL);
	//signal(SIGTTOU, SIG_DFL);
	//signal(SIGCHLD, SIG_DFL);
} //a rajouter

int			get_return_status(int status)
{
	if (WIFEXITED(status) || WIFSIGNALED(status) || WIFSTOPPED(status))
		return (WEXITSTATUS(status));
	return (0); //TODO
}

void		launch_proc(t_proc *proc)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (proc->next || !proc->is_builtin)
	{
		//set_signal_dfl();
		pid = fork();
		if (pid < 0)
			return ; //TODO fork error
		else if (pid > 0)
		{
			signal(SIGCHLD, SIG_IGN);
			if (proc->prev)
				close(proc->prev->tunnel[0]);
			if (proc->next)
				close(proc->tunnel[1]);
			if (!proc->next)
			{
				waitpid(pid, &status, WUNTRACED | WCONTINUED);
				g_shell.exit_code = get_return_status(status);
			}
			return ;
		}
		if (proc->prev)
		{
			dup2(proc->prev->tunnel[0], STDIN_FILENO);
			close(proc->prev->tunnel[0]);
		}
		if (proc->next)
		{
			dup2(proc->tunnel[1], STDOUT_FILENO);
			close(proc->tunnel[1]);
		}
		if (command_redir(proc->cmd) < 0 || execute_assign_list(proc->cmd, proc) < 0)
		{
			//exit_cmd(125); // TODO leaks + return quoi??
		}
		if (proc->is_builtin)
		{
			g_shell.exit_code = start_builtin(proc->arg, proc->envl);
			exit(0); // TODO leaks
		}
		else
		{
			if (!proc->path)
				exit(1);
			proc->env = envl_to_envarr(proc->envl);
			execve(proc->path, proc->arg, proc->env);
			command_redir_restore(proc->cmd);
			fatal_exit(7); // TODO leaks
		}
	}
	else
	{
		if (command_redir(proc->cmd) < 0)
		{
			g_shell.exit_code = 125;
			return ;
		}
		if (execute_assign_list(proc->cmd, proc) < 0)
			g_shell.exit_code = 125;
		else
			g_shell.exit_code = start_builtin(proc->arg, proc->envl);
		command_redir_restore(proc->cmd);
	}
}
