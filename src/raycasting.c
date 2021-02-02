/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:16:30 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:12:37 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	has_wall(double x, double y, t_data *d)
{
	int grid_index_x;
	int grid_index_y;

	grid_index_x = floor(x / d->map.sq_size);
	grid_index_y = floor(y / d->map.sq_size);
	if (grid_index_x < 0 || grid_index_x >= d->map.width || grid_index_y < 0 ||
		grid_index_y >= d->map.heigth)
		return ('1');
	return (d->map.grid[grid_index_y][grid_index_x]);
}

double	normalize_angle(double angle)
{
	if (angle > (2 * M_PI))
	{
		while (angle > (2 * M_PI))
			angle = angle - (2 * M_PI);
	}
	if (angle < 0)
		angle = angle + (2 * M_PI);
	return (angle);
}

void	ray_dir(t_data *d)
{
	d->ray.down = 0;
	d->ray.up = 0;
	d->ray.right = 0;
	d->ray.left = 0;
	if (d->ray.ray_angle > 0 && d->ray.ray_angle < M_PI)
		d->ray.down = 1;
	if (!(d->ray.ray_angle > 0 && d->ray.ray_angle < M_PI))
		d->ray.up = 1;
	if (d->ray.ray_angle < 0.5 * M_PI || d->ray.ray_angle > 1.5 * M_PI)
		d->ray.right = 1;
	if (!(d->ray.ray_angle < 0.5 * M_PI || d->ray.ray_angle > 1.5 * M_PI))
		d->ray.left = 1;
}

double	calcul_dist(double x1, double x2, double y1, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	cast_rays(t_data *d)
{
	int column;

	column = 0;
	d->dist_plane = (d->res.width / 2) / tan(d->ray.fov / 2);
	while (column < d->ray.nb_rays)
	{
		d->ray.ray_angle = d->cam.rotate_angle + atan((column - (d->res.width /
			2)) / d->dist_plane);
		d->ray.ray_angle = normalize_angle(d->ray.ray_angle);
		ray_dir(d);
		find_hz_hit(d);
		find_vt_hit(d);
		calcul_hit_dist(d, column);
		wall_display(d, column);
		column++;
	}
}
