/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:46:39 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:09:13 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_mini_map(t_data *d)
{
	int		width_minimap;
	int		heigth_minimap;
	t_sq	square;

	square.width = 16;
	square.ratio = 0.2;
	square.pos.x = d->cam.x - 8;
	square.pos.y = d->cam.y - 8;
	width_minimap = d->map.width * d->map.sq_size * 0.2;
	heigth_minimap = d->map.heigth * d->map.sq_size * 0.2;
	if (width_minimap < d->res.width / 2 && heigth_minimap < d->res.heigth / 2)
	{
		ft_disp_minimap(d->map, d);
		disp_square(square, 0xff0000, d);
	}
}

int		alloc_matrix(t_m *map)
{
	int i;

	i = 0;
	map->grid = malloc(sizeof(int *) * map->heigth);
	if (map->grid == NULL)
		return (-1);
	while (i < map->heigth)
	{
		map->grid[i] = malloc(sizeof(int) * (map->width + 1));
		if (map->grid[i] == NULL)
			return (-1);
		map->grid[i][map->width] = '\0';
		i++;
	}
	return (0);
}

int		ft_create_matrix(char *str, t_m *map)
{
	int i;

	i = 0;
	map->width = 0;
	map->heigth = 0;
	while (*str)
	{
		i = 0;
		while (str[i] && str[i] != ',')
			i++;
		if (i > map->width)
			map->width = i;
		str = str + i;
		if (*str == ',')
			str++;
		map->heigth++;
	}
	if (alloc_matrix(map) == -1)
		return (-1);
	return (0);
}

void	ft_set_map(t_data *d, t_m *map)
{
	int		i;
	int		j;
	char	*s;

	s = map->s_map;
	map->sq_size = 32;
	if (ft_create_matrix(map->s_map, map) == -1)
		error("malloc failed\n", d);
	i = 0;
	while (i < map->heigth && *s)
	{
		j = 0;
		while (*(map->s_map) && *s != ',')
			map->grid[i][j++] = *s++;
		while (j < map->width)
		{
			map->grid[i][j] = ' ';
			j++;
		}
		s++;
		i++;
	}
	ft_free((void **)&map->s_map);
}

void	parse_map(t_data *d)
{
	ft_set_map(d, &d->map);
	check_map_errors(d, &d->map);
	find_player(d);
	create_sprite_tab(d);
}
