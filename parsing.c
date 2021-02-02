#include "./includes/cub3d.h"

int		ft_str_is_numeric(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int		ft_count_elem(char *str, char c)
{
	int count;

	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

void	free_split(char ***split)
{
	int i;

	i = 0;
	while ((*split)[i])
	{
		free((*split)[i]);
		(*split)[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

void	free_matrix(int ***matrix, int heigth)
{
	int i;

	i = 0;
	while (i < heigth)
	{
		free((*matrix)[i]);
		(*matrix)[i] = NULL;
		i++;
	}
	free(*matrix);
	*matrix = NULL;
}

int		count_strtab_elem(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		is_full_tab(int *tab, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (tab[i] != 1)
			return (0);
		i++;
	}
	return (1);
}

void	error(char *str, t_data *d)
{
	printf("Error\n");
	printf("%s", str);
	free_struct(d);
	free_mlx(d);
	exit(EXIT_FAILURE);
}

void	get_text_tab(char *texture, t_data *d, t_t *struc)
{
	int fd;

	if (ft_strcmp(texture + ft_strlen(texture) - 4, ".xpm"))
		error("texture extension file must be .xpm\n", d);
	fd = open(texture, O_RDONLY);
	if (fd == -1)
		error("texture file cannot be open\n", d);
	close(fd);
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

void	error_index(t_data *d, char *line, int *all_elem, int index)
{
	if (index == -1 && line[0] == '1' && line[1] == '1')
	{
		if (all_elem[0] != 1)
			error("missing  or multiple resolution informations\n", d);
		if (all_elem[1] != 1 || all_elem[2] != 1 || all_elem[3] != 1 ||
			all_elem[4] != 1 || all_elem[7] != 1)
			error("missing or to many texture informations\n", d);
		if (all_elem[5] != 1 || all_elem[6] != 1)
			error("missing or to many color informations\n", d);
	}
	else
		error("wrong element in display data\n", d);
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
