/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 15:31:17 by aben-azz          #+#    #+#             */
/*   Updated: 2019/11/03 05:14:13 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void			ft_clear_all_lines(t_cap *tcap)
{
	ft_move(tcap, "up", tcap->cursy);
	tputs(tcap->carriage, 1, ft_put_termcaps);
	ft_move(tcap, "!right", tcap->prompt_len);
	tputs(tcap->clr_curr_line, 1, ft_put_termcaps);
	if (tcap->char_len + tcap->prompt_len > tcap->cursx_max + 1)
	{
		ft_move(tcap, "down", 1);
		tputs(tcap->carriage, 1, ft_put_termcaps);
		tputs(tcap->clr_all_line, 1, ft_put_termcaps);
		ft_move(tcap, "up", 1);
		ft_move(tcap, "!right", tcap->prompt_len);
	}
}

static int		handle_eol(char *buffer, t_cap *tcap)
{
	char str[2];

	ft_bzero(str, 2);
	if (buffer[0] == '\n')
	{
		tcap->carry[0] = buffer[1];
		tcap->carry[1] = buffer[2];
		tcap->overflow = 1;
		return (-2);
	}
	else if (buffer[1] == '\n')
	{
		str[0] = buffer[0];
		ft_insert(str, tcap);
		tcap->overflow = 1;
		tcap->carry[0] = buffer[2];
		return (-2);
	}
	else
	{
		ft_strncpy(str, buffer, 2);
		ft_insert(str, tcap);
		return (-2);
	}
	return (1);
}

int				checkbuffer(char *string)
{
	int i;

	i = -1;
	while (string[++i] != '\0')
		if (!ft_isprint(string[i]))
			return (0);
	return (1);
}

static int		handle_key(char *buffer, t_cap *tcap)
{
	int key;

	key = 0;
	if (~ft_indexof(buffer, '\n'))
	{
		ft_bzero(tcap->carry, 2);
		enter_event(tcap);
		return (handle_eol(buffer, tcap));
	}
	else if (buffer[0] == SPACE && !buffer[1] && !buffer[2])
		return (space_event(tcap));
	else if (checkbuffer(buffer))
		return (ft_insert(buffer, tcap));
	else if (is_arrow(buffer))
		return (read_arrow(buffer[2], tcap));
	else if ((key = is_alt(buffer)) != -1)
		return (1);
	else if ((key = is_shift_arrow(buffer)) != -1)
		return (read_arrow(key + 10, tcap));
	else if ((key = is_key(buffer)) != -1)
		return (read_key(key, tcap));
	else if (DEBUG_LOG)
		ft_printf("%c|%hhd", buffer[0], buffer[0]);
	return (1);
}

int				read_buffer(char *buffer, t_cap *tcap)
{
	if (ft_isprint(buffer[0]) && g_shell->autocomp->state &&
									(buffer[0] != ENTER) && buffer[0] != SPACE)
	{
		tputs(tcap->clr_all_line, 1, ft_put_termcaps);
		g_shell->autocomp->state = 0;
	}
	if (g_shell->ctrl_r->state && ft_isprint(buffer[0]))
		return (add_buffer_ctrl_r(buffer, g_shell->ctrl_r));
	else if (g_shell->ctrl_r->state && buffer[0] == ENTER)
		return (end_ctrl_r(g_shell->ctrl_r));
	else if (g_shell->autocomp->state &&
							(buffer[0] == ENTER && !buffer[1] && !buffer[2]))
	{
		enter_event(tcap);
		ft_bzero(buffer, 3);
	}
	else
		return (handle_key(buffer, tcap));
	return (1);
}
