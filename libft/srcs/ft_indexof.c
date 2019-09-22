/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_indexof.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-azz <aben-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/19 12:19:20 by aben-azz          #+#    #+#             */
/*   Updated: 2019/06/17 18:53:22 by midrissi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_indexof(char *string, char c)
{
	int i;

	i = 0;
	while (*(string + i))
		if (*(string + i++) == c)
			return (i - 1);
	return (-1);
}
