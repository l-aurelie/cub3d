/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:42:04 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 12:31:26 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		key_press(int key, t_data *d)
{
	if (key == KEY_W)
		d->cam.walk_dir += 1;
	else if (key == KEY_S)
		d->cam.walk_dir -= 1;
	else if (key == KEY_D)
		d->cam.side_dir += 1;
	else if (key == KEY_A)
		d->cam.side_dir -= 1;
	else if (key == KEY_LEFT)
		d->cam.turn_dir -= 1;
	else if (key == KEY_RIGHT)
		d->cam.turn_dir += 1;
	else if (key == KEY_ESC)
		ft_exit_game(d);
	return (0);
}

int		key_release(int key, t_data *d)
{
	if (key == KEY_W)
		d->cam.walk_dir -= 1;
	else if (key == KEY_S)
		d->cam.walk_dir += 1;
	else if (key == KEY_D)
		d->cam.side_dir -= 1;
	else if (key == KEY_A)
		d->cam.side_dir += 1;
	else if (key == KEY_LEFT)
		d->cam.turn_dir += 1;
	else if (key == KEY_RIGHT)
		d->cam.turn_dir -= 1;
	return (0);
}

void	player_move(t_data *d)
{
	double	move_step;
	double	old_x;
	double	old_y;

	old_x = d->cam.x;
	old_y = d->cam.y;
	if (d->cam.side_dir)
	{
		move_step = d->cam.side_dir * d->cam.move_speed;
		d->cam.x += cos(d->cam.rotate_angle + M_PI_2) * move_step;
		d->cam.y += sin(d->cam.rotate_angle + M_PI_2) * move_step;
	}
	if (d->cam.walk_dir)
	{
		move_step = d->cam.walk_dir * d->cam.move_speed;
		d->cam.x += cos(d->cam.rotate_angle) * move_step;
		d->cam.y += sin(d->cam.rotate_angle) * move_step;
	}
	if (has_wall(d->cam.x, d->cam.y, d) != '0')
	{
		d->cam.x = old_x;
		d->cam.y = old_y;
	}
}

void	find_player(t_data *d)
{
	int i;
	int j;

	i = 0;
	while (i < d->map.heigth)
	{
		j = 0;
		while (j < d->map.width)
		{
			init_player(d, i, j);
			j++;
		}
		i++;
	}
	if (d->cam.dir == 0)
		error("player position is missing\n", d);
}

void	init_player(t_data *d, int i, int j)
{
	if (d->map.grid[i][j] == 'N' || d->map.grid[i][j] == 'S' ||
				d->map.grid[i][j] == 'W' || d->map.grid[i][j] == 'E')
	{
		if (d->cam.dir)
			error("there must be only one player\n", d);
		else
		{
			d->cam.dir = d->map.grid[i][j];
			if (d->cam.dir == 'S')
				d->cam.rotate_angle = 0.5 * M_PI;
			else if (d->cam.dir == 'N')
				d->cam.rotate_angle = 1.5 * M_PI;
			else if (d->cam.dir == 'W')
				d->cam.rotate_angle = M_PI;
			else if (d->cam.dir == 'E')
				d->cam.rotate_angle = 2 * M_PI;
			d->cam.x = j * d->map.sq_size + d->map.sq_size / 2 - 0.0001;
			d->cam.y = i * d->map.sq_size + d->map.sq_size / 2 - 0.0001;
			d->map.grid[i][j] = '0';
		}
	}
}
