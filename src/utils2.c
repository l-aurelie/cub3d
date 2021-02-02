/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 22:17:18 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:12:01 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_split(char ***split)
{
	int i;

	i = 0;
	while ((*split)[i])
	{
		free((*split)[i]);
		(*split)[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

void	free_matrix(int ***matrix, int heigth)
{
	int i;

	i = 0;
	while (i < heigth)
	{
		free((*matrix)[i]);
		(*matrix)[i] = NULL;
		i++;
	}
	free(*matrix);
	*matrix = NULL;
}

int		ft_abs(int x)
{
	if (x < 0)
		x = -x;
	return (x);
}

double	d_abs(double x)
{
	if (x < 0)
		x = -x;
	return (x);
}
