/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_test.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 00:44:20 by aben-azz          #+#    #+#             */
/*   Updated: 2019/06/26 00:44:43 by aben-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	create_file(char *name, char *path, t_ab *comp, char ext)
{
	t_stat	stats;
	char	full_path[MAX_PATH];
	char	file[MAX_PATH];
	char	extension;

	extension = 0;
	ft_bzero(file, MAX_PATH);
	cat_fullpath(full_path, name, path);
	lstat(full_path, &stats);
	if ((comp->isdir = file_name_ext(name, stats, file)))
	{
		comp->data[comp->len] = ft_strdup(file);
		comp->max_offset = ft_max(comp->max_offset, ft_strlen(file));
		comp->isdir = comp->isdir == 'd';
		comp->ext[comp->len] = comp->isdir ? 'd' : ext;
		comp->len++;
	}
}

int		add_to_completion(t_ab *autocomp, char *path, char ext)
{
	t_dirent	*d;
	DIR			*dir;
	char		final_path[MAX_PATH];

	final_path[0] = 0;
	get_tilde(path, final_path);
	if ((dir = opendir(final_path)))
	{
		while ((d = readdir(dir)))
		{
			if ((d->d_name[0] != '.' || path[0] == '.') &&
			!ft_strncmp(d->d_name, autocomp->match, ft_strlen(autocomp->match)))
				create_file(d->d_name, final_path, autocomp, ext);
		}
		closedir(dir);
		return (1);
	}
	else
		dprintf(debug(), "probleme opendir\n");
	return (0);
}

int		first_arg_completion(t_ab *autocomp, t_cap *tc, char *str, int position)
{
	if (is_env_var(autocomp, str))
		return (env_completion(autocomp, str));
	else if (tc->command[position - 1] && tc->command[position - 1] == '/')
		return (add_to_completion(autocomp, str, 0));
	command_completion(autocomp, str);
	path_completion(autocomp, str);
	return (autocomp->len > 0);
}

int		print_name(t_ab *autocomp, char *str, int i)
{
	if (i == autocomp->pos)
		ft_printf("\x1b[7m%s\x1b[0m", str);
	else if (autocomp->ext[i] == 'd')
		ft_printf("\x1b[31m%s\x1b[0m", str);
	else if (autocomp->ext[i] == 'c')
		ft_printf("\x1b[33m%s\x1b[0m", str);
	else if (autocomp->ext[i] == 'i' || autocomp->ext[i] == 'e')
		ft_printf("\x1b[36m%s\x1b[0m", str);
	else
		ft_printf(str);
	ft_move(g_shell->tcap, "!right", autocomp->max_offset - ft_strlen(str) + 2);
	return (1);
}

char	file_name_ext(char *string, t_stat stats, char *name)
{
	char ext;

	ext = 0;
	if (S_ISDIR(stats.st_mode))
		ext = 'd';
	else if (S_ISREG(stats.st_mode))
		ext = '-';
	ft_bzero(name, MAX_PATH);
	ft_strcpy(name, string);
	if (ext == 'd')
		name[ft_strlen(name)] = '/';
	return (ext);
}
