#include "cub3d.h"


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

int		ft_free(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
	return (0);
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
	while(str[i])
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
			return(0);
		i++;
	}
	return(1);
}

void	error(char *str, t_d *data)//TODO gestion erreur a faire + free
{
	printf("%s", str);
	free_struct(data);
	free_mlx(data);
	exit(EXIT_FAILURE);
}

void	get_text_tab(char *texture, t_d *data, t_t *struc)
{
	int fd;
	
	if(ft_strcmp(texture + ft_strlen(texture) - 4, ".xpm"))
		error("texture extension file must be .xpm\n", data);
	fd = open(texture, O_RDONLY);
	if (fd == -1)
		error("texture file cannot be open\n", data);
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
		error("texture_name_error\n", data);
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
		error("texture name error\n", data);
	free_split(&split);
}

/*void	print_tab(int *tab, int size)
{
	int i = 0; 

	while (i < size)
	{
		printf("%d, ", tab[i]);
		i++;
	}
		printf("\n");
}*/

int		*get_color_tab(t_d *data, char **split, int **rgb)
{
	int i;

	i = 0;
	split[0] = ft_substr(split[0], 1, ft_strlen(split[0]) -1, 1);
	*rgb = malloc(sizeof(int) * 3);
	while(split[i])
	{
		split[i] = ft_strtrim(split[i], " ", 1);
		if (!split[i][0])
			error("color must be tree element separated by comas\n", data);
		if (!ft_str_is_numeric(split[i]))
			error("color must contain only digit separated by comas\n", data);
		(*rgb)[i] = ft_atoi(split[i]);
		if(!((*rgb)[i] >= 0 && (*rgb)[i] <= 255))
			error("rgb must be between 0 and 255\n", data);
		i++;
	}
	free_split(&split);
	return (*rgb);
}
void	parse_color(t_d *data, char *line)
{
	char **split;
	int *rgb;
	
	split = ft_split(line, ',');
	if (count_strtab_elem(split) != 3)
		error("color rgb wrong number of element\n", data);

	if (ft_strncmp(split[0], "C ", 2) == 0)
	{
		rgb = get_color_tab(data,split, &rgb);
		data->color.ceiling = (rgb[0] << 16) + (rgb[1] << 8) + rgb[2];
	}
	else if (ft_strncmp(split[0], "F ", 2) == 0)
	{
		rgb = get_color_tab(data, split, &rgb);
		data->color.floor = (rgb[0] << 16) + (rgb[1] << 8) + rgb[2];
	}
	else
		error("wrong element in color\n", data);
	ft_free((void **)&rgb);
}

void	parse_res(t_d *data, char *line)
{
	char **split; //todo free split 
	
	split =	ft_split(line , ' ');
	if (count_strtab_elem(split) != 3)
		error("resolution wrong number of elements\n", data);
	if (ft_strcmp(split[0], "R"))
		error("wrong element in resolution\n", data);
	if (!(ft_str_is_numeric(split[1]) && ft_str_is_numeric(split[2])))
		error("resolution must be numeric\n", data);
	data->res.width = ft_atoi(split[1]);
	data->res.heigth = ft_atoi(split[2]);
	if(!(data->res.width > 0 && data->res.heigth > 0))
		error("resolution can't be negative or 0\n", data);
	free_split(&split);
}

void	error_index(t_d *data, char *line, int *all_elem, int index)
{
	if(index == -1 && line[0] == '1' && line[1] == '1')
	{
		if (all_elem[0] != 1)
			error("missing  or multiple resolution informations\n", data);	
		if (all_elem[1] != 1 || all_elem[2] != 1 || all_elem[3] != 1 || all_elem[4] != 1 || all_elem[7] != 1)
			error("missing or to many texture informations\n", data);
		if (all_elem[5] != 1 || all_elem[6] != 1)
			error("missing or to many color informations\n", data);
		}
	else
		error("wrong element in display data\n", data);
}

void	parse_cub(char *map, t_d *data)
{
	int		fd;
	char	*line;
	char	*map_elem;
	int 	index;
	int		ret;
	int		all_elem[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	static void (*f[])() = {&parse_res, &parse_text, &parse_text, &parse_text, &parse_text, &parse_color, &parse_color};	
	
	line = NULL;
	map_elem = "RNSWEFC";
	fd = open(map, O_RDONLY);
	data->map.s_map = ft_strdup("\0");
	ret = 1;
	while (ret > 0 && !is_full_tab(all_elem, 8))
	{
		ret = get_next_line(fd, &line);
		line = ft_strtrim(line, " ", 1);
		if (line[0])
		{
			if(line[0] == 'S' && line[1] == 'O')
				all_elem[7] += 1;
			index = ft_chr_pos(map_elem, line[0]);
			if (index == -1)
				error_index(data, line, all_elem, index);
		
			(*f[index])(data, line);
			if(!(line[0] == 'S' && line[1] == 'O'))
				all_elem[index] += 1;
		}
		ft_free((void **)&line);
	}

	line = ft_strdup("\0");
	while (ret > 0 && ft_strlen(line) == 0)
	{
		ft_free((void **)&line);
		ret = get_next_line(fd, &line);
	}
	while (ret > 0)
	{
		if (ft_strlen(line) == 0)
			error("empty line in map\n", data);
		data->map.s_map = ft_strjoin(data->map.s_map, line, 1, 1);
		data->map.s_map = ft_strjoin(data->map.s_map, ",", 1, 0);
		ret = get_next_line(fd, &line);
	}
	ft_free((void **)&line);
	parse_map(data);
}
