#ifndef CUB3D_H
#define CUB3D_H 
#include <math.h>
#include "mlx.h"
#include "libft.h"
#include <stdlib.h>
#include <fcntl.h>

#include <stdio.h>

# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_A 113
# define KEY_S 115
# define KEY_D 100
# define KEY_W 122
# define KEY_ESC 65307
# define KEYPRESS 2
# define KEYRELEASE 3
# define MOUSE_PRESS 4
# define MOUSE_RELEASE 5

typedef struct	s_ra
{
}				t_ra;


typedef struct	s_ca
{
	double	x;
	double	y;
	double	xdir;
	double	ydir;
	double	xplane;
	double	yplane;

	int		turn_dir;
	int		walk_dir;
	double	move_speed;
	double	rotate_speed; 
}				t_ca;

typedef struct	s_r
{
	int		width;
	int		heigth;
	int		size_img;
	int		size_line;
}				t_r;

typedef struct	s_p
{
	void	*window;
	void	*mlx;
	void	*img; 
	char	*imgs; 
}				t_p;

typedef struct	s_co
{
	int		floor;
	int		ceiling;
	int		walls;
	int		bits_per_pixel;
	int		endian;
}				t_co;

typedef struct	s_m
{
	char 	*s_map;
	int		**grid;
}				t_m;

typedef struct	s_d
{
	t_ca	cam;
	t_r		res;
	t_p		ptr;
	t_co	color;
	t_m		map;
	t_ra	ray;
}				t_d;

int		deal_key(int key, t_ca *cam);
void	ft_set_params(t_d *data);
void	parse_map(char *map, t_d *data);
void	my_mlx_pixel_put(t_d data, int x, int y, int color);

#endif
