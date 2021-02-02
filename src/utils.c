/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 20:23:25 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:12:52 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		ft_str_is_numeric(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int		ft_count_elem(char *str, char c)
{
	int count;

	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

int		count_strtab_elem(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		is_full_tab(int *tab, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (tab[i] != 1)
			return (0);
		i++;
	}
	return (1);
}
