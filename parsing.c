#include "cub3d.h"

void	parse_res(void)
{
//	printf("coucou\n");
}

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

int		count_strtab_elem(char **str)
{
	int i;
	
	i = 0;
	while(str[i])
		i++;
	return (i);
}

void	ft_size_map(char *str, t_m *map)
{
	int i;

	i = 0;
	map->width = 0;
	map->heigth = 0;
	while (*str)
	{
		i = 0;
		while ( str[i] && str[i] != ',')
			i++;	
		if (i > map->width)
			map->width = i;
		str = str + i;
		if (*str == ',')
			str++;
		map->heigth++;
	}
}

int		is_full_tab(int *tab, int size)
{
	int i; 
	
	i = 0;
	while (i < size)
	{
		if (tab[i] != 1)
			return(0);
		i++;
	}
	return(1);
}

void	ft_set_map(t_m *map)
{
	int **grid; 
	int i; 
	int j;
	
	i = 0;
	map->sq_size = 32;//#
 	ft_size_map(map->s_map, map);
	grid = malloc(sizeof (int *) * map->heigth);
	while(i < map->heigth)
	{
		grid[i] = malloc(sizeof(int) * (map->width + 1));
	 	grid[i][map->width] = '\0';
		i++;
	}
	i = 0; 
	while (i < map->heigth && *(map->s_map))
	{
		j = 0;
		while(*(map->s_map) && *(map->s_map) != ',')
		{
			grid[i][j] = *(map->s_map);
			j++;
			map->s_map++;
		}
		while (j < map->width)
		{
			grid[i][j] = ' ';
			j++;
		}
		map->s_map++;
		i++;
	}
	map->grid = grid;
	//print_map(*map); 
}
void	error(char *str)//TODO gestion erreur a faire + free
{
	printf("%s", str);
	exit(EXIT_FAILURE);
}

void	get_text_tab(char *texture, t_d *data, t_t *struc)
{
	int fd;
	
	if(ft_strcmp(texture + ft_strlen(texture) - 4, ".xpm"))
		error("texture extension file must be .xpm\n");
	fd = open(texture, O_RDONLY);
	if (fd == -1)
		error("texture file cannot be open\n");
	close (fd);
	struc->path = texture;
	struc->img = mlx_xpm_file_to_image(data->ptr.mlx, struc->path, &struc->width, &struc->heigth);
//NULL
	struc->imgs = mlx_get_data_addr(struc->img, &struc->bpp, &struc->size_line, &struc->endian);
}

void	parse_text(t_d *data, char *line)
{
	char **split; 
	
	split =	ft_split(line , ' ');
	if (count_strtab_elem(split) != 2)
		error("texture_name_error\n");
	if (ft_strcmp(split[0], "NO") == 0)
		get_text_tab(split[1], data, &(data->color.no));
	else if (ft_strcmp(split[0], "SO") == 0)
		get_text_tab(split[1], data, &(data->color.so));
	else if (ft_strcmp(split[0], "WE") == 0)
		get_text_tab(split[1], data, &(data->color.we));
	else if (ft_strcmp(split[0], "EA") == 0)
		get_text_tab(split[1], data, &(data->color.ea));
	else if (ft_strcmp(split[0], "S") == 0)
		get_text_tab(split[1], data, &(data->color.s));
	else
		error("texture name error\n");
}

void	print_tab(int *tab, int size)
{
	int i = 0; 

	while (i < size)
	{
		printf("%d, ", tab[i]);
		i++;
	}
		printf("\n");
}

int		*get_color_tab(char **split, int **rgb)
{
	int i;
	i = 0;
//	printf("split 0  = %s\n", split[0]);
	split[0] = ft_substr(split[0], 1, ft_strlen(split[0]) -1);
	*rgb = malloc(sizeof(int) * 3);
	while(split[i])
	{
//		printf("split %d =%s\n",i, split[i]); 	
		split[i] = ft_strtrim(split[i], " ");
		if (!ft_str_is_numeric(split[i]))
			error("color must contain only digit separated by comas\n");
		(*rgb)[i] = ft_atoi(split[i]);
		if(!((*rgb)[i] >= 0 && (*rgb)[i] <= 255))
			error("rgb must be between 0 and 255\n");
//		printf("tab %d = %d\n",i, (*rgb)[i]);
		free(split[i]);
		i++;
	}
	free(split);
	return (*rgb);
}
void	parse_color(t_d *data, char *line)
{
	char **split;
	int *rgb;
//	printf("line = %s\n", line);
	split = ft_split(line, ',');
	if (count_strtab_elem(split) != 3)
		error("color rgb wrong number of element\n");

	if (ft_strncmp(split[0], "C ", 2) == 0)
	{
		rgb = get_color_tab(split, &rgb);
		data->color.ceiling = (rgb[0] << 16) + (rgb[1] << 8) + rgb[2];
	}
	else if (ft_strncmp(split[0], "F ", 2) == 0)
	{
		rgb = get_color_tab(split, &rgb);
		data->color.floor = (rgb[0] << 16) + (rgb[1] << 8) + rgb[2];
	}
	else
		error("wrong element in color\n");
}
void	error_index(char *line, int *all_elem, int index)
{
	if(index == -1 && line[0] == '1' && line[1] == '1')
	{
		if (all_elem[0] != 1)
			error("missing  or multiple resolution informations\n");	
		if (all_elem[1] != 1 || all_elem[2] != 1 || all_elem[3] != 1 || all_elem[4] != 1 || all_elem[7] != 1)
			error("missing or to many texture informations\n");
		if (all_elem[5] != 1 || all_elem[6] != 1)
			error("missing or to many color informations\n");
		}
	else
		error("wrong element before map\n");
}

void	calculate_nb_sprite(t_d *data)
{
	int i;
	int j;
	
	data->spri.nb_sprite = 0;
	i = 0;
	while (i < data->map.heigth)
	{
		j = 0;
		while (j < data->map.width)
		{
			if (data->map.grid[i][j] == '2')
				data->spri.nb_sprite++;
			j++;
		}
		i++;
	}
	data->spri.tab = malloc(sizeof(t_st) * data->spri.nb_sprite);
}

void	print_sprite_tab(t_d *data)
{
	int i = 0;
	//printf("nb sprit = %d\n", data->spri.nb_sprite);
	while(i < data->spri.nb_sprite)
	{
		printf("spri %d ", i);	
	//	printf("elem x = %d\n", data->spri.tab[i].pos.x);
	//	printf("elem y = %d\n", data->spri.tab[i].pos.y);
	//	printf("dist = %f\n", data->spri.tab[i].dist);
	//	printf("angle = %f\n", data->spri.tab[i].angle);
		printf("visible = %d\n", data->spri.tab[i].visible);
		i++;
	}
}

void	create_sprite_tab(t_d *data)
{
	int i;
	int j;
	calculate_nb_sprite(data);
	
	data->spri.nb_sprite = 0;
	i = 0;
	while (i < data->map.heigth)
	{
		j = 0;
		while (j < data->map.width)
		{
			if (data->map.grid[i][j] == '2')
			{
				data->spri.tab[data->spri.nb_sprite].pos.y = (i * data->map.sq_size) + (data->map.sq_size / 2);
				data->spri.tab[data->spri.nb_sprite].pos.x = (j * data->map.sq_size) + (data->map.sq_size / 2);
				data->spri.nb_sprite++;
			}
			j++;
		}
		i++;
	}
}

void	parse_map(char *map, t_d *data)
{
	int		fd;
	char	*line = 0;
	char	*map_elem;
	int 	index;
	int		all_elem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	static void (*f[])() = {&parse_res, &parse_text, &parse_text, &parse_text, &parse_text, &parse_color, &parse_color};	
	line = 0;
	map_elem = "RNSWEFC";
	fd = open(map, O_RDONLY);
	data->map.s_map = ft_strdup("\0");
	while (!is_full_tab(all_elem, 8))
	{
		get_next_line(fd, &line);
		line = ft_strtrim(line, " ");
		if (line[0])
		{
			if(line[0] == 'S' && line[1] == 'O')
				all_elem[7] += 1;
			index = ft_chr_pos(map_elem, line[0]);
			if (index == -1)
				error_index(line, all_elem, index);
		
			(*f[index])(data, line);
			if(!(line[0] == 'S' && line[1] == 'O'))
				all_elem[index] += 1;
		}

	}

	while (get_next_line(fd, &line))
	{
		data->map.s_map = ft_strjoin(data->map.s_map, line, 1, 1);
		data->map.s_map = ft_strjoin(data->map.s_map, ",", 1, 0);
	}
	ft_set_map(&data->map);
	create_sprite_tab(data);
}
