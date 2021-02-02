/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:54:23 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:10:00 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calculate_nb_sprite(t_data *d)
{
	int i;
	int j;

	d->spri.nb_sprite = 0;
	i = 0;
	while (i < d->map.heigth)
	{
		j = 0;
		while (j < d->map.width)
		{
			if (d->map.grid[i][j] == '2')
				d->spri.nb_sprite++;
			j++;
		}
		i++;
	}
	d->spri.tab = malloc(sizeof(t_st) * d->spri.nb_sprite);
	if (d->spri.tab == NULL)
		error("malloc failed\n", d);
}

void	parse_res(t_data *d, char *line)
{
	int max_width;
	int max_heigth;

	d->pars.split = ft_split(line, ' ');
	if (count_strtab_elem(d->pars.split) != 3)
		error("resolution wrong number of elements\n", d);
	if (ft_strcmp(d->pars.split[0], "R"))
		error("wrong element in resolution\n", d);
	if (!(ft_str_is_numeric(d->pars.split[1]) &&
		ft_str_is_numeric(d->pars.split[2])))
		error("resolution must be numeric\n", d);
	d->res.width = ft_atoi(d->pars.split[1]);
	d->res.heigth = ft_atoi(d->pars.split[2]);
	if (!(d->res.width > 0 && d->res.heigth > 0))
		error("resolution can't be negative or 0\n", d);
	free_split(&d->pars.split);
	mlx_get_screen_size(d->ptr.mlx, &max_width, &max_heigth);
	if (d->pars.save != 1 && d->res.width > max_width)
		d->res.width = max_width;
	if (d->pars.save != 1 && d->res.heigth > max_heigth)
		d->res.heigth = max_heigth;
}

void	get_display_data(t_data *d, int *ret, int fd)
{
	char		*display_elem;
	int			index;
	int			all_elem[8];
	static void	(*f[])() = {&parse_res, &parse_text, &parse_text, &parse_text,
		&parse_text, &parse_color, &parse_color};

	ft_memset(&all_elem, 0, sizeof(int) * 8);
	display_elem = "RNSWEFC";
	while (*ret > 0 && !is_full_tab(all_elem, 8))
	{
		*ret = get_next_line(fd, &d->pars.line);
		d->pars.line = ft_strtrim(d->pars.line, " ", 1);
		if (d->pars.line[0])
		{
			if (d->pars.line[0] == 'S' && d->pars.line[1] == 'O')
				all_elem[7] += 1;
			index = ft_chr_pos(display_elem, d->pars.line[0]);
			if (index == -1)
				error_index(d, d->pars.line, all_elem, index);
			(*f[index])(d, d->pars.line);
			if (!(d->pars.line[0] == 'S' && d->pars.line[1] == 'O'))
				all_elem[index] += 1;
		}
		ft_free((void **)&d->pars.line);
	}
}

void	parse_cub(char *map, t_data *d)
{
	int		fd;
	int		ret;

	ret = 1;
	fd = open(map, O_RDONLY);
	get_display_data(d, &ret, fd);
	d->map.s_map = ft_strdup("\0");
	d->pars.line = ft_strdup("\0");
	while (ret > 0 && ft_strlen(d->pars.line) == 0)
	{
		ft_free((void **)&d->pars.line);
		ret = get_next_line(fd, &d->pars.line);
	}
	while (ret > 0)
	{
		if (ft_strlen(d->pars.line) == 0)
			error("empty line in map\n", d);
		d->map.s_map = ft_strjoin(d->map.s_map, d->pars.line, 1, 1);
		d->map.s_map = ft_strjoin(d->map.s_map, ",", 1, 0);
		ret = get_next_line(fd, &d->pars.line);
	}
	ft_free((void **)&d->pars.line);
	parse_map(d);
}
