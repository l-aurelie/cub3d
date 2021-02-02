/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_hit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 22:11:59 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:09:30 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calcul_hit_dist(t_data *d, int column)
{
	d->ray.h_dist = 0;
	d->ray.v_dist = 0;
	if (d->ray.found_h == 1)
		d->ray.h_dist = calcul_dist(d->cam.x, d->ray.h_hit_x, d->cam.y,
			d->ray.h_hit_y);
	if (d->ray.found_v == 1)
		d->ray.v_dist = calcul_dist(d->cam.x, d->ray.v_hit_x, d->cam.y,
			d->ray.v_hit_y);
	if ((d->ray.v_dist < d->ray.h_dist && d->ray.v_dist != 0) ||
			d->ray.h_dist == 0)
	{
		d->ray.ray_dist[column] = d->ray.v_dist;
		d->ray.hit_x = d->ray.v_hit_x;
		d->ray.hit_y = d->ray.v_hit_y;
		d->ray.found_h = 0;
	}
	else
	{
		d->ray.ray_dist[column] = d->ray.h_dist;
		d->ray.hit_x = d->ray.h_hit_x;
		d->ray.hit_y = d->ray.h_hit_y;
		d->ray.found_v = 0;
	}
}

void	find_hz_step_and_intercept(t_data *d, t_dpos *intercept, t_dpos *step)
{
	d->ray.found_h = 0;
	intercept->y = floor(d->cam.y / d->map.sq_size) * d->map.sq_size;
	if (d->ray.down)
		intercept->y += d->map.sq_size;
	intercept->x = d->cam.x + (intercept->y - d->cam.y) / tan(d->ray.ray_angle);
	step->y = d->map.sq_size;
	if (d->ray.up)
		step->y *= -1;
	step->x = d->map.sq_size / tan(d->ray.ray_angle);
	if ((d->ray.left && step->x > 0) || (d->ray.right && step->x < 0))
		step->x *= -1;
}

void	find_hz_hit(t_data *d)
{
	t_dpos	next;
	t_dpos	step;
	double	y;

	find_hz_step_and_intercept(d, &next, &step);
	while (next.x >= 0 && next.x < d->map.sq_size * d->map.width && next.y >=
		0 && next.y < d->map.sq_size * d->map.heigth)
	{
		y = next.y;
		if (d->ray.up)
			y--;
		if (has_wall(next.x, y, d) == '1' || has_wall(next.x, y, d) == ' ')
		{
			d->ray.found_h = 1;
			d->ray.h_hit_x = next.x;
			d->ray.h_hit_y = next.y;
			break ;
		}
		next.x += step.x;
		next.y += step.y;
	}
}

void	find_vt_step_and_intercept(t_data *d, t_dpos *intercept, t_dpos *step)
{
	d->ray.found_v = 0;
	intercept->x = floor(d->cam.x / d->map.sq_size) * d->map.sq_size;
	if (d->ray.right)
		intercept->x += d->map.sq_size;
	intercept->y = d->cam.y + (intercept->x - d->cam.x) * tan(d->ray.ray_angle);
	step->x = d->map.sq_size;
	if (d->ray.left)
		step->x *= -1;
	step->y = d->map.sq_size * tan(d->ray.ray_angle);
	if ((d->ray.up && step->y > 0) || (d->ray.down && step->y < 0))
		step->y *= -1;
}

void	find_vt_hit(t_data *d)
{
	t_dpos	step;
	t_dpos	next;
	double	x;

	find_vt_step_and_intercept(d, &next, &step);
	while (next.x >= 0 && next.x < d->map.sq_size * d->map.width && next.y >=
		0 && next.y < d->map.sq_size * d->map.heigth)
	{
		x = next.x;
		if (d->ray.left)
			x--;
		if (has_wall(x, next.y, d) == '1' || has_wall(x, next.y, d) == ' ')
		{
			d->ray.found_v = 1;
			d->ray.v_hit_x = next.x;
			d->ray.v_hit_y = next.y;
			break ;
		}
		next.x += step.x;
		next.y += step.y;
	}
}
