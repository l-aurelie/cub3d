#include "cub3d.h"

void	parse_res(void)
{
//	printf("coucou\n");
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
//	print_map(*map); 
}

void	parse_map(char *map, t_d *data)
{
	int		fd;
	//int 	i = 0;
	char	*line = 0;
	char	*map_elem;
	int 	index;
	int		all_elem[7] = {0,0,0,0,0,0,0};
	static void (*f[])() = {&parse_res, &parse_res, &parse_res, &parse_res, &parse_res, &parse_res, &parse_res, &parse_res};	
	line = 0;
	map_elem = "RNSWEFC";
	fd = open(map, O_RDONLY);	
	data->map.s_map = ft_strdup("\0");
	while (!is_full_tab(all_elem, 7))
	{
		get_next_line(fd, &line);
		index = ft_chr_pos(map_elem, line[0]);
		(*f[index])();
		all_elem[index] = 1;
	}
	/*i = 0; 
	while(i < 7)
	{
		printf("tab = %d,", all_elem[i]);
		i++;
	}
	printf("\n");*/
	while (get_next_line(fd, &line))
	{
		data->map.s_map = ft_strjoin(data->map.s_map, line, 1, 1);
		data->map.s_map = ft_strjoin(data->map.s_map, ",", 1, 0);
	}
	ft_set_map(&data->map);
}
