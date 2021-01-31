#include "cub3d.h"

int ft_abs(int x)
{
	if (x < 0)
		x = -x;
	return (x);
}

double d_abs(double x)
{
	if (x < 0)
		x = -x;
	return (x);
}

void	draw_line(int x0, int y0, int x1, int y1, int color, t_data *d)
{
  int dx =  ft_abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -ft_abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */
 
  for (;;){  /* loop */
			my_pixel_put(d, x0, y0, color);
	if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void	disp_vertical_line(t_data *d, int x, int y1, int y2, int color)
{
	int j;
 
 	if (y1 < 0)
	{
		y1 = 0;
	}
	j = y1;
 	if (y2 > d->res.heigth)
	{
		y2 = d->res.heigth;
	}
	while (j < y2)
	{
		my_pixel_put(d, x, j, color);
		j++;
	}

}

void	my_pixel_put(t_data *d, int x, int y, int color)
{
    char *dst;
	dst = d->ptr.imgs + (y * d->res.size_line + x * (d->color.bpp / 8));
	*(unsigned int*)dst = color;
}

void	disp_square(int x, int y, int color, t_data *d, int width)
{
	int i;
	int j;

	j = y + width; 
	while (j >= y)
	{
		i = x + width;
		while (i >= x)
		{
			if (j == y || /*j == y + width ||*/ i == x /*|| i == x + width*/)
				my_pixel_put(d, i, j, 0xd3d3d3);
			else
				my_pixel_put(d, i, j, color);
			i--;
		}
		j--;
	}
}

void	print_map(t_m map)
{
	int i;
	int j;

	i = 0; 
	j = 0;
	while (i < map.heigth)
	{	
		j = 0;
		while(j < map.width)
		{
			printf("%c", map.grid[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

void	ft_disp_minimap(t_m map, t_p ptr, t_data *d)
{
	//printf("disp_map_____________\n");
	int i; 
	int j;
	int tileX;
	int tileY;

	i = 0;
	j = 0;
	while (j < map.heigth)
	{
		i = 0; 
		while (i < map.width)
		{
			tileX = i * map.sq_size;	
			tileY = j * map.sq_size;

			if (map.grid[j][i] == '1')
				disp_square(0.2 * tileX, 0.2 * tileY, 0x000000, d, 0.2 * map.sq_size);
			else if (map.grid[j][i] == '0')
				disp_square(0.2 * tileX, 0.2 * tileY, 0xffffff, d, 0.2 * map.sq_size);
			else if (map.grid[j][i] == '2')
				disp_square(0.2 * tileX, 0.2 * tileY, 0xa9a9a9, d, 0.2 * map.sq_size);

			i++;
		}
		j++;
	}
}	

void	ft_disp_map(t_m map, t_p ptr, t_data *d)
{
	//printf("disp_map_____________\n");
	int i; 
	int j;
	int tileX;
	int tileY;

	i = 0;
	j = 0;
	while (j < map.heigth)
	{
		i = 0; 
		while (i < map.width)
		{
			tileX = i * map.sq_size;	
			tileY = j * map.sq_size;

			if (map.grid[j][i] == '1')
				disp_square(tileX, tileY, 0xffffff, d, map.sq_size);
			else
				disp_square(tileX, tileY, 0xff0000, d, map.sq_size);
			i++;
		}
		j++;
	}
}	
