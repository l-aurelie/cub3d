#include "cub3d.h"
#include <math.h>

//TODO = memset data 

int		key_press(int key, t_ca *cam)
{
	if (key == KEY_S)//devant
		cam->walk_dir = 1; 
	else if (key == KEY_W)//bas
		cam->walk_dir = -1;
	else if (key == KEY_LEFT)//gauche
		cam->turn_dir = -1;
	else if (key == KEY_RIGHT)//dte
		cam->turn_dir = 1;
	//printf("w_dir = %d, t_dir = %d\n", cam->walk_dir, cam->turn_dir); 
	//printf("key= %d\n", key);
	return (0);
}

int		key_release(int key, t_ca *cam)
{
	if (key == KEY_S)//devant
		cam->walk_dir = 0; 
	else if (key == KEY_W)//bas
		cam->walk_dir = 0;
	else if (key == KEY_LEFT)//gauche
		cam->turn_dir = 0;
	else if (key == KEY_RIGHT)//dte
		cam->turn_dir = 0;
	//printf("w_dir = %d, t_dir = %d\n", cam->walk_dir, cam->turn_dir); 
	//printf("key= %d\n", key);
	return (0);
}

/*void	print_map(t_m map)
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
}*/

void	ft_disp_map(t_m map, t_p ptr, t_d data)
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
				disp_square(tileX, tileY, 0xffffff, data, map.sq_size);
			else
				disp_square(tileX, tileY, 0xff0000, data, map.sq_size);
			i++;
		}
		j++;
	}
}	

void	ft_set_params(t_d *data)
{
//=======res
	data->res.width = data->map.sq_size * data->map.width;
	data->res.heigth = data->map.sq_size * data->map.heigth;
//=======ptr

	data->ptr.mlx = mlx_init();
	if (!(data->ptr.window = mlx_new_window(data->ptr.mlx, data->res.width, data->res.heigth, "cube3d")))
		exit(EXIT_FAILURE);
//=======img
	data->ptr.img = mlx_new_image(data->ptr.mlx, data->res.width, data->res.heigth);
	data->ptr.imgs = mlx_get_data_addr(data->ptr.img, &data->color.bits_per_pixel, &data->res.size_line, &data->color.endian);
//======player
	data->cam.x = 200;
	data->cam.y = 200;
	data->cam.radius = 3;
	data->cam.turn_dir = 0;
	data->cam.walk_dir = 0;
	data->cam.rotate_angle = M_PI/2;
	data->cam.move_speed = 2.0;
	data->cam.rotate_speed = 2 * (M_PI/180);
//======Rays
	data->ray.fov_angle = 60 * (M_PI/180);
	data->ray.nb_rays = data->res.width;
}

char	has_wall(double x, double y, t_d data)
{
	//printf("has_wall_______\n");
	int grid_index_x;
	int grid_index_y;
	
//	if (x < 0 || x > data.res.width || y < 0 || y > data.res.heigth)
//		return ('1');
	grid_index_x = floor(x/data.map.sq_size);
	grid_index_y = floor(y/data.map.sq_size);
	return (data.map.grid[grid_index_y][grid_index_x]);
}

double	normalize_angle(double angle)
{
	if (angle > (2 * M_PI))
	{
		while (angle > (2 * M_PI))
			angle = angle - (2 * M_PI);
	}
	if (angle < 0)
		angle = angle + (2 * M_PI);
	return(angle);
}

double	calcul_dist(double x1, double x2, double y1, double y2)
{
	return(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 -y1)));
}

void	calcul_hit_dist(t_d *data)
{
	data->ray.h_dist = 0;//TODO = supprimer
       	data->ray.v_dist = 0;//TODO = supprimer
	if(data->ray.found_h == 1)
		data->ray.h_dist = calcul_dist(data->cam.x, data->ray.h_hit_x, data->cam.y, data->ray.h_hit_y);
	if(data->ray.found_v == 1)
		data->ray.v_dist = calcul_dist(data->cam.x, data->ray.v_hit_x, data->cam.y, data->ray.v_hit_y);
	
	if(data->ray.h_dist == 0)
	{
		printf("1 \n");
		data->ray.final_dist = data->ray.v_dist;
		data->ray.hit_x = data->ray.v_hit_x;//sup
		data->ray.hit_y = data->ray.v_hit_y;//sup
	}
	else if (data->ray.v_dist == 0)
	{
		printf("2\n");
		data->ray.final_dist = data->ray.h_dist;
		data->ray.hit_x = data->ray.h_hit_x;//sup
		data->ray.hit_y = data->ray.h_hit_y;//sup
	}
	else if (data->ray.h_dist > data->ray.v_dist)
	{
		printf("3\n");
		data->ray.final_dist = data->ray.v_dist;
		data->ray.hit_x = data->ray.v_hit_x;//sup
		data->ray.hit_y = data->ray.v_hit_y;//sup
	}
	else
	{
		printf("4\n");
		data->ray.final_dist = data->ray.h_dist;
		data->ray.hit_x = data->ray.h_hit_x;//sup
		data->ray.hit_y = data->ray.h_hit_y;//sup

	}
	printf("distH = %f, distV = %f, dist = %f\n", data->ray.final_dist, data->ray.h_hit_x, data->ray.v_hit_y);
}

void	find_hz_hit(t_d *data, int column_id)
{
	double	x_intercept; 
	double	y_intercept;
	double	xstep;
	double	ystep;
	double	next_x; 
	double	next_y;

	data->ray.found_h = 0; 
	/*if(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI)//down
		printf("angle down\n");
	if(!(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI))//up
		printf("angle up\n");
	if(data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI)//right
		printf("angle right\n");
if(!(data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI))//left
	printf("angle left\n");*/

	y_intercept = floor(data->cam.y/data->map.sq_size) * data->map.sq_size;
	if(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI)//down
		y_intercept += data->map.sq_size;
	x_intercept = data->cam.x + (y_intercept - data->cam.y) / tan(data->ray.ray_angle);


	ystep = data->map.sq_size;
	if(!(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI))//up
		ystep *= -1;
	xstep = data->map.sq_size / tan(data->ray.ray_angle);
	if (!(data->ray.ray_angle < (0.5 * M_PI) || data->ray.ray_angle > (1.5 * M_PI))/*left*/ && xstep > 0)
		xstep *= -1;
	if((data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI)/*right*/ && xstep < 0)
		xstep *= -1;
	next_x = x_intercept;
	next_y = y_intercept;

	if(!(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI))//up
		next_y--;
	while (next_x >= 0 && next_x <= data->res.width && next_y >= 0 && next_y <= data->res.heigth)
	{
		if(has_wall(next_x, next_y, *data) == '1')
		{
			data->ray.found_h = 1;
			data->ray.h_hit_x = next_x;
			data->ray.h_hit_y = next_y;
			break;
		}
		else
		{
		 	next_x += xstep;
			next_y += ystep;
		}
	}	
}

void	find_vt_hit(t_d *data, int column_id)
{
	double	x_intercept; 
	double	y_intercept;
	double	xstep;
	double	ystep;
	double	next_x; 
	double	next_y;

	data->ray.found_v = 0; 
//======intercept x
	x_intercept = floor(data->cam.y/data->map.sq_size) * data->map.sq_size;
	if(data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI)/*right*/ 
		x_intercept += data->map.sq_size;
//-------intercept y
	y_intercept = data->cam.y + (x_intercept - data->cam.x) * tan(data->ray.ray_angle);

//=======xstep
	xstep = data->map.sq_size;
	if (!(data->ray.ray_angle < (0.5 * M_PI) || data->ray.ray_angle > (1.5 * M_PI))/*left*/)
		xstep *= -1;
//-------ystep
	ystep = data->map.sq_size * tan(data->ray.ray_angle);
	if(!(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI)/*up*/ && ystep > 0)
		ystep *= -1;
	if((data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI)/*down*/ && ystep < 0)
		ystep *= -1;
//=======next
	next_x = x_intercept;
	next_y = y_intercept;
	if (!(data->ray.ray_angle < (0.5 * M_PI) || data->ray.ray_angle > (1.5 * M_PI))/*left*/)
		next_x--;
	//my_mlx_pixel_put(*data, x_intercept, y_intercept, 0x000000);
	//printf("ray angle = %f\n", data->ray.ray_angle);
	//printf("yint = %f, xint = %f\n", y_intercept, x_intercept);
	//printf("ystep = %f, xstep = %f\n", ystep, xstep);
	while (next_x >= 0 && next_x <= data->res.width && next_y >= 0 && next_y <= data->res.heigth)
	{
		if(has_wall(next_x, next_y, *data) == '1')
		{
			data->ray.found_v = 1;
			data->ray.v_hit_x = next_x;
			data->ray.v_hit_y = next_y;
			break;
		}
		else
		{
		 	next_x += xstep;
			next_y += ystep;
		}
	}	
}

void	cast_rays(t_d *data)
{
	//printf("castray______\n");
	int column_id;
	column_id = 0;
	
	data->ray.ray_angle = data->cam.rotate_angle - (data->ray.fov_angle /2);
	while (column_id < 1/*data->ray.nb_rays*/)
	{
		glbDrawLine(data->cam.x, data->cam.y, data->cam.x + cos(data->ray.ray_angle) * 30, data->cam.y + sin(data->ray.ray_angle) * 30, 0x00ff00, *data);
		data->ray.ray_angle += data->ray.fov_angle/data->ray.nb_rays;
	
		find_hz_hit(data, column_id);
		find_vt_hit(data, column_id);
		calcul_hit_dist(data);
		glbDrawLine(data->cam.x, data->cam.y, data->ray.hit_x, data->ray.hit_y, 0x00ff00, *data);

	column_id++;
	}
}
	
int		ft_loop(t_d *data)
{
//	printf("ft_loop______\n");
	mlx_clear_window(data->ptr.mlx, data->ptr.window);
	double move_step;
	double old_x;
	double old_y; 

//rotation=========
	data->cam.rotate_angle += data->cam.turn_dir * data->cam.rotate_speed;
	data->cam.rotate_angle = normalize_angle(data->cam.rotate_angle);
//player_move======
	move_step = data->cam.walk_dir * data->cam.move_speed;
	old_x = data->cam.x;
	old_y = data->cam.y;
	data->cam.x += cos(data->cam.rotate_angle) * move_step;
	data->cam.y += sin(data->cam.rotate_angle) * move_step;
	if (has_wall(data->cam.x, data->cam.y, *data) == '1')
	{
		data->cam.x = old_x;
		data->cam.y = old_y;
	}
//DISPLAY========
	ft_disp_map(data->map, data->ptr, *data);
	disp_square(data->cam.x, data->cam.y, 0xffff00, *data, 3);
	cast_rays(data);
	glbDrawLine(data->cam.x, data->cam.y, data->cam.x + cos(data->cam.rotate_angle) * 30, data->cam.y + sin(data->cam.rotate_angle) * 30, 0x000000, *data);
	
	mlx_put_image_to_window(data->ptr.mlx, data->ptr.window, data->ptr.img, 0, 0);
	mlx_do_sync(data->ptr.mlx);
	return (0);
}

int		main(int argc, char **argv)
{
	t_d		data;

	int		pos;
	int		*img_s;
	
	parse_map(argv[1], &data);
	ft_set_params(&data);

	mlx_hook(data.ptr.window, KEYPRESS, 1L<<0, key_press, (void *)&data.cam);	
	mlx_hook(data.ptr.window, KEYRELEASE, 1L<<1, key_release, (void *)&data.cam);
	mlx_loop_hook(data.ptr.mlx, ft_loop, &data);
	mlx_loop(data.ptr.mlx);
}

	//img_s = (int *)data.ptr.imgs;
	//pos = 0;
	//while (pos < data.res.width * data.res.heigth)
	//{
	//	img_s[pos] = 0x00FF0000;
	//	pos++;
	//}
	//mlx_key_hook(ptr.window, deal_key, (void *)&cam);

