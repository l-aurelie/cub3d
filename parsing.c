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
		if (tab[i] == 0)
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
	
	if(strncmp(texture + ft_strlen(texture) - 4, ".xpm", 4))
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
	if (ft_strncmp(split[0], "NO", 2) == 0)
		get_text_tab(split[1], data, &(data->color.no));
	else if (ft_strncmp(split[0], "SO", 2) == 0)
		get_text_tab(split[1], data, &(data->color.so));
	else if (ft_strncmp(split[0], "WE", 2) == 0)
		get_text_tab(split[1], data, &(data->color.we));
	else if (ft_strncmp(split[0], "EA", 2) == 0)
		get_text_tab(split[1], data, &(data->color.ea));
	else if (ft_strncmp(split[0], "S", 2) == 0)
		get_text_tab(split[1], data, &(data->color.s));
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

void	find_int_color(int *color, int r, int g, int b)
{
		*color = (r << 16) + (g << 8) + b;
		printf("color = %d\n", *color);
}
void	parse_color(t_d *data, char *line)
{
	char **split;
	char **color; 

	split =	ft_split(line , ' ');
	if (count_strtab_elem(split) != 2)
		error("color element is wrong\n");
	color = ft_split(split[1], ',');
	if (count_strtab_elem(color) != 3)
		error("color rgb wrong number of element\n");
	if(!ft_str_is_numeric(color[0]) || !ft_str_is_numeric(color[1]) || !ft_str_is_numeric(color[2]))
		error("colors must be numeric\n");
	if(!(ft_atoi(color[0]) >= 0 && ft_atoi(color[0]) <= 255) || !(ft_atoi(color[1]) >= 0 && ft_atoi(color[1]) <= 255) || !(ft_atoi(color[2]) >= 0 && ft_atoi(color[2]) <= 255))
		error("color must be between 0 and 255\n");	
	if (ft_strncmp(split[0], "C", 1) == 0)
		find_int_color(&data->color.ceiling, ft_atoi(color[0]), ft_atoi(color[1]), ft_atoi(color[2]));
	if (ft_strncmp(split[0], "F", 1) == 0)//changer pour strcmp
		find_int_color(&data->color.floor, ft_atoi(color[0]), ft_atoi(color[1]), ft_atoi(color[2]));

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
	printf("line = %s\n", line);

		if (line[0])
		{
			if(line[0] == 'S' && line[1] == 'O')
				all_elem[7] = 1;
			index = ft_chr_pos(map_elem, line[0]);
			if(index == -1)
			{
				if (!all_elem[0])
					error("missing resolution informations\n");	
				if (!all_elem[1] || !all_elem[2] || !all_elem[3] || !all_elem[4] || !all_elem[7])
					error("missing texture informations\n");
				if (!all_elem[5] || !all_elem[6])
					error("missing color informations\n");
			}
			(*f[index])(data, line);
			all_elem[index] = 1;
		}

	}

	while (get_next_line(fd, &line))
	{
		data->map.s_map = ft_strjoin(data->map.s_map, line, 1, 1);
		data->map.s_map = ft_strjoin(data->map.s_map, ",", 1, 0);
	}
	ft_set_map(&data->map);
}
