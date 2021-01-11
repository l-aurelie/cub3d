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
	double	fov_angle;
	double	ray_angle;
	double	nb_rays;

	int		down;
	int		up;
	int		right;
	int		left;

	int		found_v;
	double	v_hit_x;
	double	v_hit_y;
	double	v_dist;

	int		found_h;
	double	h_hit_x;
	double	h_hit_y;
	double	h_dist;

	double	hit_x;
	double	hit_y;
	double	final_dist;
}				t_ra;


typedef struct	s_ca
{
	double	x;
	double	y;
	int		radius;
	int		turn_dir;
	int		walk_dir;
	double 	rotate_angle;
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
	int		sq_size;
	int		width;
	int		heigth;
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

void	disp_square(int x, int y, int color, t_d data, int width);
//int		deal_key(int key, t_ca *cam);
void	ft_set_params(t_d *data);
void	disp_vertical_line(t_d data, int x, int y1, int y2, int color);
void	draw_line(int x0, int y0, int x1, int y1, int color, t_d data);
void    glbDrawLine(int firstx,int firsty,int lastx,int lasty, int color, t_d data);
void	parse_map(char *map, t_d *data);
void	ft_set_map(t_m *map);
void	my_mlx_pixel_put(t_d data, int x, int y, int color);
void	print_map(t_m map);

#endif
