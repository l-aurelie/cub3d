#include "cub3d.h"

void	my_mlx_pixel_put(t_d data, int x, int y, int color)
{
    char *dst;
	dst = data.ptr.imgs + (y * data.res.size_line + x * (data.color.bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
