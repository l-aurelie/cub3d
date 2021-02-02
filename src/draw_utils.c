/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 20:17:43 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:11:05 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	disp_vertical_line(t_data *d, t_ipos pos, int y2, int color)
{
	int j;

	if (pos.y < 0)
	{
		pos.y = 0;
	}
	j = pos.y;
	if (y2 > d->res.heigth)
	{
		y2 = d->res.heigth;
	}
	while (j < y2)
	{
		my_pixel_put(d, pos.x, j, color);
		j++;
	}
}

void	my_pixel_put(t_data *d, int x, int y, int color)
{
	char *dst;

	dst = d->ptr.imgs + (y * d->res.size_line + x * (d->color.bpp / 8));
	*(unsigned int*)dst = color;
}

void	disp_square(t_sq square, int color, t_data *d)
{
	int i;
	int j;

	if (square.ratio)
	{
		square.width *= square.ratio;
		square.pos.x *= square.ratio;
		square.pos.y *= square.ratio;
	}
	j = square.pos.y + square.width;
	while (j >= square.pos.y)
	{
		i = square.pos.x + square.width;
		while (i >= square.pos.x)
		{
			if (j == square.pos.y || i == square.pos.x)
				my_pixel_put(d, i, j, 0xd3d3d3);
			else
				my_pixel_put(d, i, j, color);
			i--;
		}
		j--;
	}
}

void	ft_disp_minimap(t_m map, t_data *d)
{
	int		i;
	int		j;
	t_sq	square;

	j = 0;
	square.ratio = 0.2;
	square.width = map.sq_size;
	while (j < map.heigth)
	{
		i = 0;
		while (i < map.width)
		{
			square.pos.x = i * map.sq_size;
			square.pos.y = j * map.sq_size;
			if (map.grid[j][i] == '1')
				disp_square(square, 0x000000, d);
			else if (map.grid[j][i] == '0')
				disp_square(square, 0xffffff, d);
			else if (map.grid[j][i] == '2')
				disp_square(square, 0xa9a9a9, d);
			i++;
		}
		j++;
	}
}
