/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:09:05 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:14:43 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	create_sprite_tab(t_data *d)
{
	int i;
	int j;

	calculate_nb_sprite(d);
	d->spri.nb_sprite = 0;
	i = 0;
	while (i < d->map.heigth)
	{
		j = 0;
		while (j < d->map.width)
		{
			if (d->map.grid[i][j] == '2')
			{
				d->spri.tab[d->spri.nb_sprite].pos.y = (i * d->map.sq_size) +
					(d->map.sq_size / 2);
				d->spri.tab[d->spri.nb_sprite].pos.x = (j * d->map.sq_size) +
					(d->map.sq_size / 2);
				d->spri.nb_sprite++;
			}
			j++;
		}
		i++;
	}
}

void	calculate_dist_sprite(t_st *sprite, t_data *d)
{
	sprite->dist = sqrt((d->cam.x - sprite->pos.x) * (d->cam.x -
	sprite->pos.x) + (d->cam.y - sprite->pos.y) * (d->cam.y - sprite->pos.y));
}

void	sort_sprite_tab(t_data *d)
{
	int		i;
	t_st	tmp;

	i = 0;
	while (i < d->spri.nb_sprite - 1)
	{
		if (d->spri.tab[i].dist < d->spri.tab[i + 1].dist && (i + 1) <
			d->spri.nb_sprite)
		{
			tmp = d->spri.tab[i];
			d->spri.tab[i] = d->spri.tab[i + 1];
			d->spri.tab[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
}

void	calculate_sprite_angle(t_st *sprite, t_data *d)
{
	double	vectx;
	double	vecty;
	double	total_angle;

	vectx = sprite->pos.x - d->cam.x;
	vecty = sprite->pos.y - d->cam.y;
	total_angle = atan2(vecty, vectx);
	sprite->angle = total_angle - d->cam.rotate_angle;
	sprite->angle = normalize_angle(sprite->angle);
	sprite->visible = 0;
	if (d_abs(sprite->angle) < d->ray.fov / 2. + 0.2 || d_abs(sprite->angle) >
		2 * M_PI - d->ray.fov / 2 - 0.2)
		sprite->visible = 1;
}

void	sprite_render(t_data *d)
{
	int i;

	i = 0;
	while (i < d->spri.nb_sprite)
	{
		calculate_dist_sprite(&d->spri.tab[i], d);
		calculate_sprite_angle(&d->spri.tab[i], d);
		i++;
	}
	sort_sprite_tab(d);
	i = 0;
	while (i < d->spri.nb_sprite)
	{
		if (d->spri.tab[i].visible)
			sprite_display(d, &d->spri.tab[i]);
		i++;
	}
}
