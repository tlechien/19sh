/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlechien <tlechien@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/03 18:39:48 by tlechien          #+#    #+#             */
/*   Updated: 2019/10/04 19:30:44 by tlechien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "shell.h"

/*
** Adds a new node at the end of the t_pipe list.
*/
void push_pipe(t_ast *root, t_pipe **begin)
{
	t_pipe *new;
	t_pipe *pipe;

	pipe = *begin;
	if (!(new = (t_pipe *)ft_memalloc(sizeof(t_pipe))))
		shell_exit(MALLOC_ERR);
	new->cmd = root->token->content;
	if (pipe)
	{
		while (pipe->next)
			pipe = pipe->next;
		pipe->next = new;
	}
	else
		*begin = new;
}

/*
** Parses the ast and creates a list of the pipe to be executed.
*/
void parse_pipe (t_ast *root, t_ast *origin, t_pipe **pipe)
{
	if (root->left)
		parse_pipe(root->left, origin, pipe);
	if (root->right)
		parse_pipe(root->right, origin, pipe);
	if (root == origin)
		push_pipe(root, pipe);
	else if (root->token->type == TOKEN_WORD)
		push_pipe(root, pipe);
}

static int		ft_pipe_exec(char **cmd, int redir)
{
	t_builtin *builtin;

	g_shell->lastsignal = ft_pre_execution(&cmd, redir, &builtin);
	if (!g_shell->lastsignal && !builtin)
		execve(cmd[0], cmd, g_shell->env_tmp);
	if (!g_shell->lastsignal && builtin)
		g_shell->lastsignal = builtin->function(
										ft_split_count((const char**)cmd), cmd);
	return (g_shell->lastsignal);
}

/*
** Execve an unitary elem of the pipe and set its fds.
*/
void	launch_process(t_pipe *begin,t_pipe *prev, t_pipe *pipe, int is_bg)
{
		if (is_bg)
		{
			resetsign();
			setpgid(pipe->pid, begin->pid);
		}
		else
			setpgid(getpid(), getpgrp());
		if (pipe->next)
		{
			dup2(pipe->fd[1], STDOUT_FILENO);
			close(pipe->fd[0]);
		}
		if (prev)
		{
			dup2(prev->fd[0], STDIN_FILENO);
			close(prev->fd[1]);
		}
		ft_pipe_exec(pipe->cmd, 0);
		exit(1);
}

/*
** Launches each elem of a pipe and links them together.
*/
int launch_pipe (t_pipe **begin, t_pipe *elem, int is_bg)
{
	t_pipe  *prev;

	prev = NULL;
	elem = *begin;
	while (elem)
	{
		pipe(elem->fd);
		if (!(elem->pid = fork()))
			launch_process(*begin, prev, elem, is_bg);
		else if (elem->pid < 0)
			shell_exit(FORK_ERR);
		prev = elem;
		elem = elem->next;
	}
	elem = *begin;
	while (elem)
	{
		close(elem->fd[0]);
		close(elem->fd[1]);
		if (is_bg)
		{
				(elem == *begin) ? add_pid(3, elem->pid, elem->cmd, ID_RUN) :
				add_amperpipe((*begin)->pid, elem->pid, full_cmd(elem->cmd), ID_RUN);
				g_shell->dprompt = 0;
				g_shell->chld_check = 1;
		}
		elem = elem->next;
	}
	elem = *begin;
	while (!is_bg && elem)
	{
		waitpipe(begin, elem);
		prev = elem;
		elem = elem->next;
	}
	elem = *begin;
	while (!is_bg && elem)
	{
		prev = elem;
		elem = elem->next;
		free(prev);
	}
	setpgid(0, 0);
	tcsetpgrp(0, 0);
	return (0);
}*/
