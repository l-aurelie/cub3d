/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:13:48 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:12:15 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_mlx(t_data *d)
{
	if (d->ptr.img)
		mlx_destroy_image(d->ptr.mlx, d->ptr.img);
	if (d->color.no.img)
		mlx_destroy_image(d->ptr.mlx, d->color.no.img);
	if (d->color.so.img)
		mlx_destroy_image(d->ptr.mlx, d->color.so.img);
	if (d->color.ea.img)
		mlx_destroy_image(d->ptr.mlx, d->color.ea.img);
	if (d->color.we.img)
		mlx_destroy_image(d->ptr.mlx, d->color.we.img);
	if (d->color.s.img)
		mlx_destroy_image(d->ptr.mlx, d->color.s.img);
	if (d->ptr.window)
		mlx_destroy_window(d->ptr.mlx, d->ptr.window);
	mlx_destroy_display(d->ptr.mlx);
	if (d->ptr.mlx)
		ft_free((void **)&d->ptr.mlx);
}

void	free_struct(t_data *d)
{
	if (d->ray.ray_dist)
		ft_free((void **)&d->ray.ray_dist);
	if (d->map.s_map)
		ft_free((void **)&d->map.s_map);
	if (d->map.grid)
		free_matrix(&d->map.grid, d->map.heigth);
	if (d->spri.tab)
		ft_free((void **)&d->spri.tab);
	if (d->pars.line)
		ft_free((void **)&d->pars.line);
	if (d->pars.split)
		free_split(&d->pars.split);
	if (d->pars.rgb)
		ft_free((void **)&d->pars.rgb);
}

int		ft_exit_game(t_data *d)
{
	free_struct(d);
	free_mlx(d);
	exit(EXIT_SUCCESS);
	return (0);
}
