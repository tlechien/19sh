/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlechien <tlechien@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 03:30:06 by tlechien          #+#    #+#             */
/*   Updated: 2019/11/06 15:35:25 by tlechien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Inits the global pid_table.
*/

int	init_pid(void)
{
	if (!(g_pid_table = (t_child*)ft_memalloc(sizeof(t_child))))
		shell_exit(MALLOC_ERR);
	g_shell->dprompt = 1;
	g_shell->chld_check = 1;
	return (0);
}

/*
** Adds a new node to the pid_table.
*/

int	add_pid(int is_pipe, int pid, char **command, int status)
{
	t_child *new;

	while (ID_PREV)
		g_pid_table = ID_PREV;
	while (ID_LEFT && ID_LEFT->index == ID_INDEX + 1)
		g_pid_table = ID_LEFT;
	if (!(new = (t_child*)malloc(sizeof(t_child))))
		shell_exit(MALLOC_ERR);
	new->is_pipe = is_pipe;
	new->index = ID_INDEX + 1;
	new->pid = pid;
	new->status = status;
	new->priority = 0;
	new->left = ID_LEFT;
	new->right = NULL;
	new->prev = g_pid_table;
	(new->left) ? new->left->prev = new : 0;
	ID_LEFT = new;
	(!(new->exec = full_cmd(command))) ? shell_exit(MALLOC_ERR) : 0;
	while (ID_LEFT)
		g_pid_table = ID_LEFT;
	setpgid(pid, 0);
	update_priority(new->index, NULL, NULL);
	ft_printf("[%d] %d\n", new->index, new->pid);
	return (0);
}

/*
** Adds a new node to right pointer of a pig node.
*/

int	add_amperpipe(int pid_origin, int pid, char *cmd, int status)
{
	t_child *new;
	t_child	*origin;

	search_pid(&origin, NULL, pid_origin);
	if (!origin)
	{
		return (err_display(
			"An error has occured in the piping process\n", NULL, NULL));
	}
	while (origin->right)
		origin = origin->right;
	if (!(new = (t_child*)malloc(sizeof(t_child))))
		shell_exit(MALLOC_ERR);
	new->is_pipe = 1;
	new->index = origin->index;
	new->pid = pid;
	new->status = status;
	new->priority = 0;
	new->right = NULL;
	new->left = NULL;
	new->prev = origin;
	origin->right = new;
	(!(new->exec = cmd)) ? shell_exit(MALLOC_ERR) : setpgid(pid, 0);
	ft_printf("%*c %d\n", get_nb_len(new->index) + 2, ' ', new->pid);
	return (0);
}
