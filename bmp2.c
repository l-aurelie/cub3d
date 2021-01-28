#include "cub3d.h"

void  get_header_info(t_d *data, t_i *head_info, t_h *header)
{
	ft_memset(head_info, 0, sizeof(t_i));
	ft_memset(header, 0, sizeof(t_h));
	header->signature[0] = 'B';
	header->signature[1] = 'M';
//	header->size_file = sizeof(t_h) + sizeof(t_i) + data->res.width * data->res.heigth * 3;
	header->size_file = sizeof(t_h) + sizeof(t_i);
	head_info->size_info = sizeof(t_i);
	head_info->width = data->res.width;
	head_info->heigth = data->res.heigth;
	head_info->nb_color_planes = 1;
	head_info->bit_per_pix = 24;
}

int		find_img_pixel(t_d *data, int x, int y)
{
	char	*color;
	color = data->ptr.imgs + (y * data->res.size_line + x * data->color.bits_per_pixel / 8);
	return (*(int*)color);
}

void  bmp_image(t_d *data, int fd)
{
	t_px pix[data->res.width];
	int i;
	int j;

	i = data->res.heigth;
	while (--i >= 0)
	{
		j = 0;
		while (j < data->res.width) 
		{
			int color = find_img_pixel(data, j, i);
			pix[j].b = color >> 0;
			pix[j].g = color >> 8;
			pix[j].r = color >> 16;
			++j;
		}
		write(fd, pix, sizeof(pix));
	}
}


void  ft_create_bmp(t_d *data)
{
	char *name;
	int fd;

	t_i head_info;
	t_h header;
	name = (char*)"Cub3D.bmp";
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0)
		error("can't create bmp file", data);
	get_header_info(data, &head_info, &header);
	write(fd, &header, sizeof(t_h));
	write(fd, &head_info, sizeof(t_i));
	bmp_image(data, fd);
	close(fd);
}
