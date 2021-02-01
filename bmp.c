#include "./includes/cub3d.h"


void  get_header_info(t_data *d, t_i *head_info, t_h *header)
{
	ft_memset(head_info, 0, sizeof(t_i));
	ft_memset(header, 0, sizeof(t_h));
	header->signature[0] = 'B';
	header->signature[1] = 'M';
	header->size_file = sizeof(t_h) + sizeof(t_i) + d->res.width * d->res.heigth * 3;
	head_info->size_info = sizeof(t_i);
	head_info->width = d->res.width;
	head_info->heigth = d->res.heigth;
	head_info->nb_color_planes = 1;
	head_info->bit_per_pix = 24;
}

/*int		find_img_pixel(t_data *d, int x, int y)
{	char	*color;

 if (y < 0)
        y = 0;
    if (y > d->res.heigth)
        y = d->res.heigth;
    if (x < 0)
        x = 0;
    if (x > d->res.width)
        x = d->res.width;
	color = d->ptr.imgs + (y * d->res.size_line + x * d->color.bpp / 8);
	return (*(int*)color);
}*/

int			ft_get_px_from_image(t_data *d, unsigned int x, unsigned int y)
{
	int pos;

	int *imgs = (int *)d->ptr.imgs;
	if (y > (unsigned int)d->res.heigth || x > (unsigned int)d->res.width)
		return (0);
	pos = (y * d->res.size_line + x * d->color.bpp / 8) / sizeof(int);
	return (imgs[pos]);
}


void  bmp_image(t_data *d, int fd)
{
	t_px pix[d->res.width];
	int i;
	int j;

	i = d->res.heigth;
	while (--i >= 0)
	{
		j = 0;
		while (j < d->res.width) 
		{
			int color = ft_get_px_from_image(d, j, i);
			pix[j].b = color >> 0;
			pix[j].g = color >> 8;
			pix[j].r = color >> 16;
			++j;
		}
		write(fd, pix, sizeof(pix));
	}
}


void  ft_create_bmp(t_data *d)
{
	char *name;
	int fd;

	t_i head_info;
	t_h header;
	name = (char*)"Cub3D.bmp";
	fd = open(name, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd < 0)
		error("can't create bmp file", d);
	get_header_info(d, &head_info, &header);
	write(fd, &header, sizeof(t_h));
	write(fd, &head_info, sizeof(t_i));
	bmp_image(d, fd);
	close(fd);
}