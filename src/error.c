/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 21:30:19 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 15:49:41 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error(char *str, t_data *d)
{
	printf("Error\n");
	printf("%s", str);
	free_struct(d);
	free_mlx(d);
	exit(EXIT_FAILURE);
}

void	map_elem_surround(t_data *d, t_m *map, int i, int j)
{
	if (map->grid[i - 1][j] == ' ' || map->grid[i + 1][j] == ' ' ||
		map->grid[i][j - 1] == ' ' || map->grid[i][j + 1] == ' ')
		error("map must be surrounded by walls\n", d);
}

void	check_map_errors(t_data *d, t_m *map)
{
	int i;
	int j;

	i = 0;
	while (i < map->heigth)
	{
		j = 0;
		while (j < map->width)
		{
			if (!(i == 0 || j == 0 || j == (map->width - 1) || i ==
				(map->heigth - 1)))
			{
				if (!is_charset(map->grid[i][j], " 120NSWE"))
					error("unauthorised character in map\n", d);
				if (is_charset(map->grid[i][j], "20NSWE"))
					map_elem_surround(d, map, i, j);
			}
			else if (!is_charset(map->grid[i][j], " 1"))
				error("map must be surrounded by wall\n", d);
			j++;
		}
		i++;
	}
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

int		check_error_arguments(t_data *d, int argc, char **argv)
{
	int fd;
	int error_state;

	error_state = 0;
	if (argc < 2 || argc > 3)
		error_state = 1;
	else
	{
		if (ft_strcmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub"))
			error_state = 1;
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			error_state = 1;
		close(fd);
		if (argc == 3 && ft_strcmp(argv[2], "--save") != 0)
			error_state = 1;
		if (argc == 3)
			d->pars.save = 1;
	}
	if (error_state)
	{
		printf("Error\nplease enter program name followed by [...].cub.");
		exit(EXIT_FAILURE);
	}
	return (error_state);
}
