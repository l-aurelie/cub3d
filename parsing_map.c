#include "cub3d.h"



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

void	ft_set_map(t_d *data, t_m *map)
{
	int **grid; 
	int i; 
	int j;
	char *s;
	
	s = map->s_map;
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
	while (i < map->heigth && *s)
	{
		j = 0;
		while(*(map->s_map) && *s != ',')
		{
			grid[i][j] = *s;
			j++;
			s++;
		}
		while (j < map->width)
		{
			grid[i][j] = ' ';
			j++;
		}
		s++;
		i++;
	}
	map->grid = grid;
	ft_free((void **)&map->s_map);
//print_map(*map); 
}

void	map_elem_surround(t_d *data, t_m *map, int i, int j)
{
	if (map->grid[i - 1][j] == ' ' || map->grid[i + 1][j] == ' ' || map->grid[i][j - 1] == ' ' || map->grid[i][j + 1] == ' ')
		error("map must be surrounded by walls\n", data);
}

void	check_map_errors( t_d *data, t_m *map)
{
	int i;
	int j;
	
	i = 0;
	while (i < map->heigth)
	{
		j = 0;
		while (j < map->width)
		{
			if (!(i == 0 || j == 0 || j == (map->width - 1) || i == (map->heigth - 1)))
			{
				if (!is_charset(map->grid[i][j], " 120NSWE"))
					error("unauthorised character in map\n", data);
				if (is_charset(map->grid[i][j], "20NSWE"))
					map_elem_surround(data, map, i, j);
			}
			else
				if(!is_charset(map->grid[i][j], " 1"))
					error("map must be surrounded by wall\n", data);
			j++;
		}
		i++;
	}

}

void	init_player_dir(t_d *data)
{
	if(data->cam.dir == 'S')
		data->cam.rotate_angle = 0.5 * M_PI;
	if(data->cam.dir == 'N')
		data->cam.rotate_angle = 1.5 * M_PI;
	if(data->cam.dir == 'W')
		data->cam.rotate_angle =  M_PI;
	if(data->cam.dir == 'E')
		data->cam.rotate_angle = 2 * M_PI;
	//printf("rotate_angle = %f\n", data->cam.rotate_angle);
}

void	init_player(t_d *data)
{
	
	int i;
	int j;
	
	i = 0;
	data->cam.dir = 0;//supp, memset
	while (i < data->map.heigth)
	{
		j = 0;
		while (j < data->map.width)
		{
			if (data->map.grid[i][j] == 'N' || data->map.grid[i][j] == 'S' || data->map.grid[i][j] == 'W' || data->map.grid[i][j] == 'E')
			{	
				if (data->cam.dir)
					error("there must be only one player\n", data);
				else
				{
					data->cam.dir = data->map.grid[i][j];
					init_player_dir(data);
					data->cam.x = j * data->map.sq_size + data->map.sq_size /2;
					data->cam.y = i * data->map.sq_size + data->map.sq_size /2;
					//printf("cam dir = %c\n",data->cam.dir);
				}
			}
			j++;
		}
		i++;
	}
	if (data->cam.dir == 0)
		error("player position is missing\n", data); 
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
		//printf("spri %d ", i);	
	//	printf("elem x = %d\n", data->spri.tab[i].pos.x);
	//	printf("elem y = %d\n", data->spri.tab[i].pos.y);
	//	printf("dist = %f\n", data->spri.tab[i].dist);
	//	printf("angle = %f\n", data->spri.tab[i].angle);
	//	printf("visible = %d\n", data->spri.tab[i].visible);
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

void	parse_map(t_d *data)
{
	ft_set_map(data, &data->map);
	check_map_errors(data, &data->map);
	init_player(data);
	create_sprite_tab(data);
}
