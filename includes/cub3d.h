/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 23:18:49 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 15:06:39 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <math.h>
# include "mlx.h"
# include "libft.h"
# include "bmp.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

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

typedef struct	s_sq
{
	t_ipos	pos;
	int		width;
	double	ratio;
}				t_sq;

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
	int		side_dir;
	double	rotate_angle;
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
	char	*s_map;
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
	t_st	*tab;
}				t_sp;

typedef struct	s_data
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

void			get_header_info(t_data *d, t_i *head_info, t_h *header);
void			bmp_image(t_data *d, int fd);
void			ft_create_bmp(t_data *d);

void			ft_set_params(t_data *d);
int				ft_loop(t_data *d);

void			disp_vertical_line(t_data *d, t_ipos pos, int y2, int color);
void			my_pixel_put(t_data *d, int x, int y, int color);
void			disp_square(t_sq square, int color, t_data *d);
void			ft_disp_minimap(t_m map, t_data *d);

void			error(char *str, t_data *d);
void			map_elem_surround(t_data *d, t_m *map, int i, int j);
void			check_map_errors(t_data *d, t_m *map);
void			error_index(t_data *d, char *line, int *all_elem, int index);
int				check_error_arguments(t_data *d, int argc, char **argv);

void			free_mlx(t_data *d);
void			free_struct(t_data *d);
int				ft_exit_game(t_data *d);

void			ft_mini_map(t_data *d);
int				alloc_matrix(t_m *map);
int				ft_create_matrix(char *str, t_m *map);
void			ft_set_map(t_data *d, t_m *map);
void			parse_map(t_data *d);

void			calculate_nb_sprite(t_data *d);
void			parse_res(t_data *d, char *line);
void			get_display_data(t_data *d, int *ret, int fd);
void			parse_cub(char *map, t_data *d);

void			get_text_tab(char *texture, t_data *d, t_t *struc);
void			parse_text(t_data *d, char *line);
int				*get_color_tab(t_data *d, char ***split, int **rgb);
void			parse_color(t_data *d, char *line);

int				key_press(int key, t_data *d);
int				key_release(int key, t_data *d);
void			player_move(t_data *d);
void			find_player(t_data *d);
void			init_player(t_data *d, int i, int j);

char			has_wall(double x, double y, t_data *d);
double			normalize_angle(double angle);
void			ray_dir(t_data *d);
double			calcul_dist(double x1, double x2, double y1, double y2);
void			cast_rays(t_data *d);

void			calcul_hit_dist(t_data *d, int column);
void			find_hz_step_and_intercept(t_data *d, t_dpos *intercept, \
				t_dpos *step);
void			find_hz_hit(t_data *d);
void			find_vt_step_and_intercept(t_data *d, t_dpos *intercept, \
				t_dpos *step);
void			find_vt_hit(t_data *d);

int				find_text_pixel(t_t text, int x_texture, int y_texture);
void			disp_wall_text(t_data *d, int col, t_rend *wall, t_t *text);
void			wall_display(t_data *d, int column);
void			sprite_draw(t_data *d, t_st *sprite, t_rend *s, int x_begin);
void			sprite_display(t_data *d, t_st *sprite);

void			create_sprite_tab(t_data *d);
void			calculate_dist_sprite(t_st *sprite, t_data *d);
void			sort_sprite_tab(t_data *d);
void			calculate_sprite_angle(t_st *sprite, t_data *d);
void			sprite_render(t_data *d);

int				ft_str_is_numeric(char *str);
int				ft_count_elem(char *str, char c);
int				count_strtab_elem(char **str);
int				is_full_tab(int *tab, int size);

void			free_split(char ***split);
void			free_matrix(int ***matrix, int heigth);
int				ft_abs(int x);
double			d_abs(double x);

#endif
