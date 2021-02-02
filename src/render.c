/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:58:21 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:10:16 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		find_text_pixel(t_t text, int x_texture, int y_texture)
{
	char	*color;

	if (y_texture < 0)
		y_texture = 0;
	if (y_texture > text.heigth)
		y_texture = text.heigth;
	if (x_texture < 0)
		x_texture = 0;
	if (x_texture > text.width)
		x_texture = text.width;
	color = text.imgs + (y_texture * text.size_line + x_texture * text.bpp / 8);
	return (*(int*)color);
}

void	disp_wall_text(t_data *d, int col, t_rend *wall, t_t *text)
{
	if (d->ray.found_v)
		wall->text.x = fmod(d->ray.v_hit_y / d->map.sq_size, 1) * text->width;
	if (d->ray.found_h)
		wall->text.x = fmod(d->ray.h_hit_x / d->map.sq_size, 1) * text->width;
	if ((d->ray.down && d->ray.found_h) || (d->ray.left && d->ray.found_v))
		wall->text.x = text->width - 1 - wall->text.x;
	if (wall->begin < 0)
		wall->begin = 0;
	if (wall->end > d->res.heigth)
		wall->end = d->res.heigth;
	wall->curr.y = wall->begin;
	while (wall->curr.y < wall->end)
	{
		wall->text.y = (wall->curr.y + (wall->heigth / 2.0) - (d->res.heigth /
			2.0)) * ((double)text->heigth / (double)wall->heigth);
		my_pixel_put(d, col, wall->curr.y, find_text_pixel(*text, wall->text.x,
			wall->text.y));
		wall->curr.y++;
	}
}

void	wall_display(t_data *d, int column)
{
	t_rend	wall;
	t_ipos	pos;
	double	correct_dist;

	correct_dist = d->ray.ray_dist[column] * cos(d->ray.ray_angle -
		d->cam.rotate_angle);
	d->dist_plane = (d->res.width / 2) / tan(d->ray.fov / 2);
	wall.heigth = (d->map.sq_size / correct_dist) * d->dist_plane;
	wall.begin = (d->res.heigth / 2) - (wall.heigth / 2);
	wall.end = wall.begin + wall.heigth;
	pos.x = column;
	pos.y = 0;
	disp_vertical_line(d, pos, wall.begin, d->color.ceiling);
	pos.y = wall.end;
	disp_vertical_line(d, pos, d->res.heigth, d->color.floor);
	if (d->ray.up && d->ray.found_h)
		disp_wall_text(d, column, &wall, &d->color.no);
	else if (d->ray.down && d->ray.found_h)
		disp_wall_text(d, column, &wall, &d->color.so);
	else if (d->ray.left && d->ray.found_v)
		disp_wall_text(d, column, &wall, &d->color.we);
	else if (d->ray.right && d->ray.found_v)
		disp_wall_text(d, column, &wall, &d->color.ea);
}

void	sprite_draw(t_data *d, t_st *sprite, t_rend *s, int x_begin)
{
	double wall_dist;

	s->curr.x = 0;
	while (x_begin + s->curr.x < 0)
		s->curr.x++;
	while (s->curr.x < s->heigth && x_begin + s->curr.x < d->res.width)
	{
		wall_dist = d->ray.ray_dist[(int)(x_begin + s->curr.x)];
		if (wall_dist > sprite->dist)
		{
			s->text.x = s->curr.x * d->color.s.width / s->heigth;
			s->curr.y = s->begin;
			while (s->curr.y < s->end)
			{
				s->text.y = (s->curr.y + (s->heigth / 2.0) - (d->res.heigth /
					2.0)) * ((double)d->color.s.heigth / (double)s->heigth);
				s->color = find_text_pixel(d->color.s, s->text.x, s->text.y);
				if (s->color != s->transparence)
					my_pixel_put(d, x_begin + s->curr.x, s->curr.y, s->color);
				s->curr.y++;
			}
		}
		s->curr.x++;
	}
}

void	sprite_display(t_data *d, t_st *sprite)
{
	t_rend	s;
	int		x_begin;

	s.heigth = d->dist_plane / (cos(sprite->angle) * sprite->dist /
		d->map.sq_size);
	s.begin = (d->res.heigth / 2) - (s.heigth / 2);
	s.end = s.begin + s.heigth;
	if (s.begin < 0)
		s.begin = 0;
	if (s.end > d->res.heigth)
		s.end = d->res.heigth;
	x_begin = d->dist_plane * tan(sprite->angle) + (d->res.width / 2.0) -
		(s.heigth / 2);
	s.transparence = find_text_pixel(d->color.s, 0, 0);
	sprite_draw(d, sprite, &s, x_begin);
}
