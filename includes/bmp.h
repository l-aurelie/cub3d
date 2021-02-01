#ifndef BMP_H
# define BMP_H
# include "cub3d.h"

typedef struct s_px		t_px;
struct __attribute__((__packed__))	s_px
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};				


typedef struct s_h		t_h;		
struct __attribute__((__packed__))	s_h
{
	char			signature[2];
	unsigned int	size_file;
	unsigned int	reserved;
	unsigned int	offset;
};		

typedef struct s_i		t_i;
struct __attribute__((__packed__))	s_i
{
	unsigned int		size_info;
	int					width;
	int					heigth;
	unsigned short int	nb_color_planes;
	unsigned short int	bit_per_pix;
	unsigned int		compression;
	unsigned int		image_size;
	int					horizontal_res;
	int					vertical_res;
	unsigned int		color_table;
	unsigned int		important_colors;
};

#endif