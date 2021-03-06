/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: midrissi <midrissi@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 17:25:38 by midrissi          #+#    #+#             */
/*   Updated: 2019/11/10 20:13:32 by tlechien         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_all_key_value(char *key, char **env)
{
	char *value;

	if (env == g_shell->intern)
		return (value = get_key_value(key, g_shell->intern));
	else if (env == g_shell->env_tmp)
		return (value = get_key_value(key, g_shell->env_tmp));
	else if (!(value = get_key_value(key, g_shell->env)))
		return (value = get_key_value(key, g_shell->intern));
	return (value);
}

int		set_builtin(void)
{
	print_split(g_shell->env);
	print_split(g_shell->intern);
	return (0);
}
