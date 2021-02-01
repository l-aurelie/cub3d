#ifndef CUB3D_H
#define CUB3D_H 
#include <math.h>
#include "mlx.h"
#include "libft.h"
#include "bmp.h"
#include <stdlib.h>
#include <fcntl.h>

#include <stdio.h>

# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_W 119
# define KEY_ESC 65307
# define KEYPRESS 2
# define KEYRELEASE 3
# define MOUSE_PRESS 4
# define MOUSE_RELEASE 5

typedef struct	s_ipos
{
	int	x;
	int	y;
}				t_ipos;

typedef struct	s_dpos
{
	double	x;
	double	y;
}				t_dpos;

typedef struct	s_pa
{
	int		save;
	char	**split;
	char	*line;
	int		*rgb;
}				t_pa;

typedef struct	s_rend
{
	double	heigth;
	int		begin;
	int		end;
	t_ipos	text;
	int		color;
	int		transparence;
	t_ipos	curr;
}				t_rend;

typedef struct	s_ra
{
	double	fov;
	double	ray_angle;
	double	*ray_dist;
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
}				t_ra;

typedef struct	s_ca
{
	double	x;
	double	y;
	char	dir;
	int		turn_dir;
	int		walk_dir;
	double 	rotate_angle;
	double	move_speed;
	double	rotate_speed; 
	int		side_move;
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

typedef struct	s_t
{
	char			*path;
	void			*img;
	char			*imgs;
	int				width;
	int				heigth;
	int				bpp;
	int				endian;
	int				size_line;

}				t_t;

typedef struct	s_co
{
	int		floor;
	int		ceiling;
	t_t		no;
	t_t		so;
	t_t		we;
	t_t		ea;
	t_t		s;
	int		bpp;
	int		endian;
}				t_co;

typedef struct	s_m
{
	double	sq_size;
	int		width;
	int		heigth;
	char 	*s_map;
	int		**grid;
}				t_m;

typedef struct	s_st
{
	t_ipos	pos;
	double	dist;
	double	angle;
	int		visible;
}				t_st;

typedef struct	s_sp
{
	int		nb_sprite;
	t_st		*tab;

}				t_sp;

typedef struct	s_d
{
	t_ca	cam;
	t_r		res;
	t_p		ptr;
	t_co	color;
	t_m		map;
	t_ra	ray;
	t_sp	spri;
	t_pa	pars;
	double	dist_plane;
}				t_data;

void	disp_square(int x, int y, int color, t_data *d, int width);
void	ft_set_params(t_data *d);
void	disp_vertical_line(t_data *d, int x, int y1, int y2, int color);
void	draw_line(int x0, int y0, int x1, int y1, int color, t_data *d);
void	parse_cub(char *map, t_data *d);
void	my_pixel_put(t_data *d, int x, int y, int color);
void	print_map(t_m map);
void	ft_disp_minimap(t_m map, t_p ptr, t_data *d);
void	ft_disp_map(t_m map, t_p ptr, t_data *d);
void	error_index(t_data *d, char *line, int *all_elem, int index);
void	calculate_dist_sprite(t_st *sprite, t_data *d);
void	calculate_angle_sprite(t_st *sprite,t_data *d);
void	sort_sprite_tab(t_data *d);
void	print_sprite_tab(t_data *d);
void	print_tab_double(double *tab, int size);
void	sprite_init(t_data *d);
int		ft_abs(int x);
double	d_abs(double x);
double  normalize_angle(double angle);
int		find_text_pixel(t_t text, int x_texture, int y_texture);
void	init_player(t_data *d);
void	ft_set_map(t_data *d, t_m *map);
void	error(char *sterror, t_data *d);
void	parse_map(t_data *d);
int		ft_exit_game(t_data *d);  
int		ft_free(void **ptr);
void	free_matrix(int ***matrix, int heigth);
void	free_mlx(t_data *d);
void	free_struct(t_data *d);
void	free_split(char ***split);
void	ft_create_bmp(t_data *d);

#endif
