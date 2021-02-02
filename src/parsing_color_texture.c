/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_color_texture.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:48:51 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 00:10:47 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_text_tab(char *texture, t_data *d, t_t *struc)
{
	int fd;

	if (ft_strcmp(texture + ft_strlen(texture) - 4, ".xpm"))
		error("texture extension file must be .xpm\n", d);
	fd = open(texture, O_RDONLY);
	if (fd == -1)
		error("texture file cannot be open\n", d);
	close(fd);
	if (struc->img)
		error("texture documented twice\n", d);
	struc->path = texture;
	struc->img = mlx_xpm_file_to_image(d->ptr.mlx, struc->path, &struc->width,
		&struc->heigth);
	if (struc->img == NULL)
		error("fonction mlx_xpm_file_to_image failed\n", d);
	struc->imgs = mlx_get_data_addr(struc->img, &struc->bpp, &struc->size_line,
		&struc->endian);
}

void	parse_text(t_data *d, char *line)
{
	d->pars.split = ft_split(line, ' ');
	if (count_strtab_elem(d->pars.split) != 2)
		error("texture_name_error\n", d);
	if (ft_strcmp(d->pars.split[0], "NO") == 0)
		get_text_tab(d->pars.split[1], d, &(d->color.no));
	else if (ft_strcmp(d->pars.split[0], "SO") == 0)
		get_text_tab(d->pars.split[1], d, &(d->color.so));
	else if (ft_strcmp(d->pars.split[0], "WE") == 0)
		get_text_tab(d->pars.split[1], d, &(d->color.we));
	else if (ft_strcmp(d->pars.split[0], "EA") == 0)
		get_text_tab(d->pars.split[1], d, &(d->color.ea));
	else if (ft_strcmp(d->pars.split[0], "S") == 0)
		get_text_tab(d->pars.split[1], d, &(d->color.s));
	else
		error("texture name error\n", d);
	free_split(&d->pars.split);
}

int		*get_color_tab(t_data *d, char ***split, int **rgb)
{
	int i;

	i = 0;
	(*split)[0] = ft_substr((*split[0]), 1, ft_strlen((*split[0])) - 1, 1);
	*rgb = malloc(sizeof(int) * 3);
	if (*rgb == NULL)
		error("malloc failed\n", d);
	while ((*split)[i])
	{
		(*split)[i] = ft_strtrim((*split)[i], " ", 1);
		if (!(*split)[i][0])
			error("color must be tree element separated by comas\n", d);
		if (!ft_str_is_numeric((*split)[i]))
			error("color must contain only digit separated by comas\n", d);
		(*rgb)[i] = ft_atoi((*split)[i]);
		if (!((*rgb)[i] >= 0 && (*rgb)[i] <= 255))
			error("rgb must be between 0 and 255\n", d);
		i++;
	}
	free_split(split);
	return (*rgb);
}

void	parse_color(t_data *d, char *line)
{
	int	comas;

	comas = ft_count_elem(line, ',');
	if (comas != 2)
		error("color rgb wrong number of elements\n", d);
	d->pars.split = ft_split(line, ',');
	if (count_strtab_elem(d->pars.split) != 3)
		error("color rgb wrong number of elements\n", d);
	if (ft_strncmp(d->pars.split[0], "C ", 2) == 0)
	{
		d->pars.rgb = get_color_tab(d, &d->pars.split, &d->pars.rgb);
		d->color.ceiling = (d->pars.rgb[0] << 16) + (d->pars.rgb[1] << 8) +
			d->pars.rgb[2];
	}
	else if (ft_strncmp(d->pars.split[0], "F ", 2) == 0)
	{
		d->pars.rgb = get_color_tab(d, &d->pars.split, &d->pars.rgb);
		d->color.floor = (d->pars.rgb[0] << 16) + (d->pars.rgb[1] << 8) +
			d->pars.rgb[2];
	}
	else
		error("wrong element in color\n", d);
	ft_free((void **)&d->pars.rgb);
}
