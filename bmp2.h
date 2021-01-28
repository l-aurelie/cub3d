#ifndef BMP_H
# define BMP_H
# include "cub3d.h"

typedef struct s_px
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}				t_px;


typedef struct s_h		
{
	char			signature[2];
	unsigned int	size_file;
	unsigned int	reserved;
	unsigned int	offset;
}				t_h;

typedef struct s_i
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
}				t_i;

#endif
