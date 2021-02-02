/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 22:23:34 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:08:48 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_set_params(t_data *d)
{
	d->ptr.img = mlx_new_image(d->ptr.mlx, d->res.width, d->res.heigth);
	if (d->ptr.img == NULL)
		error("fonction mlx_new_image goes wrong\n", d);
	d->ptr.imgs = mlx_get_data_addr(d->ptr.img, &d->color.bpp,
		&d->res.size_line, &d->color.endian);
	d->cam.move_speed = 2.0;
	d->cam.rotate_speed = 2.0 * (M_PI / 180);
	d->ray.fov = 60 * (M_PI / 180);
	d->ray.nb_rays = d->res.width;
	d->ray.ray_dist = malloc(sizeof(double) * d->res.width);
	if (d->ray.ray_dist == NULL)
		error("malloc error", d);
	if (d->pars.save == 0)
	{
		d->ptr.window = mlx_new_window(d->ptr.mlx, d->res.width, d->res.heigth,
		"cub3D");
		if (d->ptr.window == NULL)
			error("can't open window\n", d);
		mlx_hook(d->ptr.window, KEYPRESS, 1L << 0, key_press, (void *)d);
		mlx_hook(d->ptr.window, KEYRELEASE, 1L << 1, key_release, (void *)d);
		mlx_hook(d->ptr.window, 33, (1L << 17), ft_exit_game, (void *)d);
	}
}

void	raycasting(t_data *d)
{
	cast_rays(d);
	sprite_render(d);
	ft_mini_map(d);
}

int		ft_loop(t_data *d)
{
	d->cam.rotate_angle += d->cam.turn_dir * d->cam.rotate_speed;
	d->cam.rotate_angle = normalize_angle(d->cam.rotate_angle);
	player_move(d);
	raycasting(d);
	mlx_put_image_to_window(d->ptr.mlx, d->ptr.window, d->ptr.img, 0, 0);
	return (0);
}

int		main(int argc, char **argv)
{
	t_data	d;

	ft_memset(&d, 0, sizeof(t_data));
	check_error_arguments(&d, argc, argv);
	d.ptr.mlx = mlx_init();
	if (d.ptr.mlx == NULL)
		error("fonction mlx_init goes wrong\n", &d);
	parse_cub(argv[1], &d);
	ft_set_params(&d);
	if (argc == 2)
	{
		mlx_loop_hook(d.ptr.mlx, ft_loop, &d);
		mlx_loop(d.ptr.mlx);
	}
	else if (argc == 3)
	{
		d.pars.save = 1;
		raycasting(&d);
		ft_create_bmp(&d);
		ft_exit_game(&d);
	}
	return (0);
}
