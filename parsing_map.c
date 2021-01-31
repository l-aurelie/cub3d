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

void	ft_set_map(t_data *d, t_m *map)
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

void	map_elem_surround(t_data *d, t_m *map, int i, int j)
{
	if (map->grid[i - 1][j] == ' ' || map->grid[i + 1][j] == ' ' || map->grid[i][j - 1] == ' ' || map->grid[i][j + 1] == ' ')
		error("map must be surrounded by walls\n", d);
}

void	check_map_errors( t_data *d, t_m *map)
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
					error("unauthorised character in map\n", d);
				if (is_charset(map->grid[i][j], "20NSWE"))
					map_elem_surround(d, map, i, j);
			}
			else
				if(!is_charset(map->grid[i][j], " 1"))
					error("map must be surrounded by wall\n", d);
			j++;
		}
		i++;
	}

}

void	init_player_dir(t_data *d)
{
	if(d->cam.dir == 'S')
		d->cam.rotate_angle = 0.5 * M_PI;
	if(d->cam.dir == 'N')
		d->cam.rotate_angle = 1.5 * M_PI;
	if(d->cam.dir == 'W')
		d->cam.rotate_angle =  M_PI;
	if(d->cam.dir == 'E')
		d->cam.rotate_angle = 2 * M_PI;
	//printf("rotate_angle = %f\n", d->cam.rotate_angle);
}

void	init_player(t_data *d)
{
	int i;
	int j;
	
	i = 0;
	d->cam.dir = 0;//supp, memset
	while (i < d->map.heigth)
	{
		j = 0;
		while (j < d->map.width)
		{
			if (d->map.grid[i][j] == 'N' || d->map.grid[i][j] == 'S' || d->map.grid[i][j] == 'W' || d->map.grid[i][j] == 'E')
			{	
				if (d->cam.dir)
					error("there must be only one player\n", d);
				else
				{
					d->cam.dir = d->map.grid[i][j];
					init_player_dir(d);
					d->cam.x = j * d->map.sq_size + d->map.sq_size /2 - 0.0001;
					d->cam.y = i * d->map.sq_size + d->map.sq_size /2 - 0.0001;
					d->map.grid[i][j] = '0';
				}
			}
			j++;
		}
		i++;
	}
	if (d->cam.dir == 0)
		error("player position is missing\n", d); 
}

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
}

void	print_sprite_tab(t_data *d)
{
	int i = 0;
	//printf("nb sprit = %d\n", d->spri.nb_sprite);
	while(i < d->spri.nb_sprite)
	{
		//printf("spri %d ", i);	
	//	printf("elem x = %d\n", d->spri.tab[i].pos.x);
	//	printf("elem y = %d\n", d->spri.tab[i].pos.y);
	//	printf("dist = %f\n", d->spri.tab[i].dist);
	//	printf("angle = %f\n", d->spri.tab[i].angle);
	//	printf("visible = %d\n", d->spri.tab[i].visible);
		i++;
	}
}

void	create_sprite_tab(t_data *d)
{
	int i;
	int j;
	calculate_nb_sprite(d);
	
	d->spri.nb_sprite = 0;
	i = 0;
	while (i < d->map.heigth)
	{
		j = 0;
		while (j < d->map.width)
		{
			if (d->map.grid[i][j] == '2')
			{
				d->spri.tab[d->spri.nb_sprite].pos.y = (i * d->map.sq_size) + (d->map.sq_size / 2);
				d->spri.tab[d->spri.nb_sprite].pos.x = (j * d->map.sq_size) + (d->map.sq_size / 2);
				d->spri.nb_sprite++;
			}
			j++;
		}
		i++;
	}
}

void	parse_map(t_data *d)
{
	ft_set_map(d, &d->map);
	check_map_errors(d, &d->map);
	init_player(d);
	create_sprite_tab(d);
}
