#include "cub3d.h"
#include <math.h>

//TODO = memset data 

int		key_press(int key, t_d *data)
{	
	data->cam.side_move = 0;
	if (key == KEY_A || key == KEY_D )
		data->cam.side_move = 1;
	if (key == KEY_W)//devant
		data->cam.walk_dir = 1; 
	else if (key == KEY_S)//bas
		data->cam.walk_dir = -1;
	else if (key == KEY_D)//droite 
		data->cam.walk_dir = 1; 
	else if (key == KEY_A)//gauche
		data->cam.walk_dir = -1;
	else if (key == KEY_LEFT)//rot_gauche
		data->cam.turn_dir = -1;
	else if (key == KEY_RIGHT)//rot_dte
		data->cam.turn_dir = 1;
	else if (key == KEY_ESC)
		ft_exit_game(data);	
	return (0);
}

int		key_release(int key, t_ca *cam)
{
	if (key == KEY_W)//devant
		cam->walk_dir = 0; 
	else if (key == KEY_S)//bas
		cam->walk_dir = 0;
	else if (key == KEY_D)//droite 
		cam->walk_dir = 0; 
	else if (key == KEY_A)//gauche
		cam->walk_dir = 0;
	else if (key == KEY_LEFT)//gauche
		cam->turn_dir = 0;
	else if (key == KEY_RIGHT)//dte
		cam->turn_dir = 0;
	return (0);
}

void	ft_set_params(t_d *data)
{
//=======ptr
	if (!(data->ptr.window = mlx_new_window(data->ptr.mlx, data->res.width, data->res.heigth, "cube3d")))
		exit(EXIT_FAILURE);
//=======img
	data->ptr.img = mlx_new_image(data->ptr.mlx, data->res.width, data->res.heigth);
	data->ptr.imgs = mlx_get_data_addr(data->ptr.img, &data->color.bits_per_pixel, &data->res.size_line, &data->color.endian);
//======player
	data->cam.turn_dir = 0;
	data->cam.walk_dir = 0;
	data->cam.move_speed = 4.0;
	data->cam.rotate_speed = 4.0 * (M_PI/180);
//======Rays
	data->ray.fov_angle = 60 * (M_PI/180);
	data->ray.nb_rays = data->res.width;
	data->ray.ray_dist = malloc(sizeof(double) * data->res.width);//TODO protection
}

void    calculate_dist_sprite(t_st *sprite, t_d *data)
{
        sprite->dist = sqrt((data->cam.x - sprite->pos.x) * (data->cam.x - sprite->pos.x) + (data->cam.y - sprite->pos.y) * (data->cam.y - sprite->pos.y));
}

void    sort_sprite_tab(t_d *data)
{
    int		i;
    t_st	tmp;

    i = 0;
    while (i < data->spri.nb_sprite - 1)
    {
        if (data->spri.tab[i].dist < data->spri.tab[i + 1].dist && (i + 1) < data->spri.nb_sprite)
        {
            tmp = data->spri.tab[i];
            data->spri.tab[i] = data->spri.tab[i + 1];
            data->spri.tab[i + 1] = tmp;
            i = -1;
        }
        i++;
    }
}

void	calculate_sprite_angle(t_st *sprite, t_d *data)
{
	double	vectx;
	double	vecty;
	double	angle_diff;

	vectx  = sprite->pos.x - data->cam.x;  
	vecty = sprite->pos.y -  data->cam.y; 
	sprite->angle = atan2(vecty, vectx);
//	printf("rotate = %f\n", data->cam.rotate_angle);
	printf("sprite angle = %f\n", sprite->angle);
	angle_diff = data->cam.rotate_angle - sprite->angle;
	printf("==============\n\n");
	printf("diff_angle = %f\n", angle_diff);
	angle_diff = normalize_angle(angle_diff);
	printf("norma diff_angle = %f\n", angle_diff);
	sprite->visible = 0;
	//printf("diff = %f\n", angle_diff);
	if (angle_diff < data->ray.fov_angle / 2 || angle_diff > 2 * M_PI - data->ray.fov_angle / 2)
		sprite->visible = 1;
}

void	sprite_display(t_d *data, t_st *sprite)
{
	//printf("sprite display======\n");
	double	dist_plane;
	double	sprite_heigth;
	int		sprite_begin;
	int		sprite_end;
	double	dx;
	double	dy;
	double	sprite_angle;
	int		x_begin;
	double 	x_curr;
	double	y_curr;
	int		x_texture; 
	int		y_texture;	
	int		color;
	int		transparence;
	double	wall_dist;
	
	dist_plane = (data->res.width / 2) / tan(data->ray.fov_angle / 2);//todo fonction pour le calculer une seul fois
	sprite_heigth = (data->map.sq_size / sprite->dist) * dist_plane;
	
	sprite_begin = (data->res.heigth / 2) - (sprite_heigth / 2);
	sprite_end = sprite_begin + sprite_heigth;
	if (sprite_begin < 0)
		sprite_begin = 0;
	if (sprite_end  > data->res.heigth)
		sprite_end = data->res.heigth;
	
//	printf("sprite heigth = %f\n", sprite_heigth);	
	//printf("begin = %d, end = %d\n", sprite_begin, sprite_end );
	dx = data->cam.x - sprite->pos.x;
	dy = data->cam.y - sprite->pos.y;
	sprite_angle = atan2(dy, dx) - data->cam.rotate_angle;
	printf("======sprite angle = %f\n", sprite_angle);

	x_begin = dist_plane * tan(sprite_angle) + (data->res.width / 2.0) - (sprite_heigth / 2);
//	printf("x_begin = %d", x_begin);
//========draw
	
	transparence = find_text_pixel(data->color.s, 0, 0);
	x_curr = 0;
	while (x_begin + x_curr < 0)
		x_curr++;
	while (x_curr < sprite_heigth && x_begin + x_curr < data->res.width)
	{
		//printf("x_curr = %f\n", x_curr);
		wall_dist = data->ray.ray_dist[(int)(x_begin	+ x_curr)];
		if (wall_dist > sprite->dist)
		{
			//printf("wall_dist = %f, sprite_dist = %f\n", wall_dist, sprite->dist);
			x_texture = x_curr * data->color.s.width / sprite_heigth;
			
			//printf("(x_texture) %f * %d / %f  = %d\n",x_curr, data->color.s.width, sprite_heigth, x_texture);
			y_curr = sprite_begin;
			while (y_curr < sprite_end)
			{
				//printf("y_curr = %f \n", y_curr);
				y_texture = (y_curr + (sprite_heigth / 2.0) - (data->res.heigth / 2.0)) * ((double)data->color.s.heigth / (double)sprite_heigth);
				//printf("y_texture = %d\n", y_texture);
				color = find_text_pixel(data->color.s, x_texture, y_texture);
				//printf("color = %d\n", color);
				if (color != transparence)
					my_mlx_pixel_put(data, x_begin + x_curr, y_curr, color);	
				y_curr++;
			}
		}
		x_curr++;
	}
}

void	sprite_init(t_d *data)
{
	int i;

	i = 0;
	while (i < data->spri.nb_sprite)
	{
		calculate_dist_sprite(&data->spri.tab[i], data);
		calculate_sprite_angle(&data->spri.tab[i], data);
		if (data->spri.tab[i].visible)
			sprite_display(data, &data->spri.tab[i]);
		i++;
	}
	sort_sprite_tab(data);
//	print_sprite_tab(data);
}

char	has_wall(double x, double y, t_d *data)
{
	//printf("has_wall_______\n");
	int grid_index_x;
	int grid_index_y;
	


	grid_index_x = floor(x/data->map.sq_size);
	grid_index_y = floor(y/data->map.sq_size);
//	if (grid_index_x < 0 || grid_index_x >= data->map.width || grid_index_y < 0 || grid_index_y >= data->map.heigth)
//		return ('1');
//	printf("grid_index_x = %d, grid_index_y = %d\n", grid_index_x, grid_index_y);
//	printf("width = %d, heigth = %d\n", data->map.width, data->map.heigth);
	return (data->map.grid[grid_index_y][grid_index_x]);
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

void	ray_dir(t_d *data)
{
	data->ray.down = 0;//a garder ou supprimer (memset)
	data->ray.up = 0;
	data->ray.right = 0;
	data->ray.left = 0;
	
	if(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI)//down
		data->ray.down = 1;
	if(!(data->ray.ray_angle > 0 && data->ray.ray_angle < M_PI))//up
		data->ray.up = 1;
	if(data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI)//right
		data->ray.right = 1;
	if(!(data->ray.ray_angle < 0.5 * M_PI || data->ray.ray_angle > 1.5 * M_PI))//left
		data->ray.left = 1;
}

double	calcul_dist(double x1, double x2, double y1, double y2)
{
	return(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 -y1)));
}

void	calcul_hit_dist(t_d *data, int column)
{
	data->ray.h_dist = 0;//TODO = supprimer
       	data->ray.v_dist = 0;//TODO = supprimer

	if(data->ray.found_h == 1)
		data->ray.h_dist = calcul_dist(data->cam.x, data->ray.h_hit_x, data->cam.y, data->ray.h_hit_y);
	if(data->ray.found_v == 1)
		data->ray.v_dist = calcul_dist(data->cam.x, data->ray.v_hit_x, data->cam.y, data->ray.v_hit_y);
	
	if(data->ray.h_dist == 0)
	{
		data->ray.ray_dist[column] = data->ray.v_dist;
		data->ray.hit_x = data->ray.v_hit_x;//sup
		data->ray.hit_y = data->ray.v_hit_y;//sup
	}
	else if (data->ray.v_dist == 0)
	{
		data->ray.ray_dist[column] = data->ray.h_dist;
		data->ray.hit_x = data->ray.h_hit_x;//sup
		data->ray.hit_y = data->ray.h_hit_y;//sup
	}
	else if (data->ray.h_dist > data->ray.v_dist)
	{
		data->ray.ray_dist[column] = data->ray.v_dist;
		data->ray.hit_x = data->ray.v_hit_x;//sup
		data->ray.hit_y = data->ray.v_hit_y;//sup
		data->ray.found_h = 0;
	}
	else
	{
		data->ray.ray_dist[column] = data->ray.h_dist;
		data->ray.hit_x = data->ray.h_hit_x;//sup
		data->ray.hit_y = data->ray.h_hit_y;//sup
		data->ray.found_v = 0;
	}
}

void	find_hz_hit(t_d *data)
{
	double	x_intercept; 
	double	y_intercept;
	double	xstep;
	double	ystep;
	double	next_x; 
	double	next_y;
	double	y;

	data->ray.found_h = 0; 

	y_intercept = floor(data->cam.y/data->map.sq_size) * data->map.sq_size;
	if(data->ray.down)//down
		y_intercept += data->map.sq_size;
	x_intercept = data->cam.x + (y_intercept - data->cam.y) / tan(data->ray.ray_angle);


	ystep = data->map.sq_size;
	if(data->ray.up)//up
		ystep *= -1;
	xstep = data->map.sq_size / tan(data->ray.ray_angle);
	if (data->ray.left/*left*/ && xstep > 0)
		xstep *= -1;
	if(data->ray.right/*right*/ && xstep < 0)
		xstep *= -1;
	next_x = x_intercept;
	next_y = y_intercept;
	while (next_x >= 0 && next_x < data->map.sq_size * data->map.width && next_y >= 0 && next_y <  data->map.sq_size * data->map.heigth)
	{
		y = next_y;
		if(data->ray.up)//up
			y--;

		if(has_wall(next_x, y, data) == '1' ||has_wall(next_x, y, data) == ' ')//TODO laisser ' '?
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

void	find_vt_hit(t_d *data)
{
	double	x_intercept; 
	double	y_intercept;
	double	xstep;
	double	ystep;
	double	next_x; 
	double	next_y;
	double	x;

	data->ray.found_v = 0; 
//======intercept x
	x_intercept = floor(data->cam.x/data->map.sq_size) * data->map.sq_size;
	if(data->ray.right)/*right*/ 
		x_intercept += data->map.sq_size;
//-------intercept y
	y_intercept = data->cam.y + (x_intercept - data->cam.x) * tan(data->ray.ray_angle);

//=======xstep
	xstep = data->map.sq_size;
	if (data->ray.left)/*left*/
		xstep *= -1;
//-------ystep
	ystep = data->map.sq_size * tan(data->ray.ray_angle);
	if(data->ray.up/*up*/ && ystep > 0)
		ystep *= -1;
	if(data->ray.down/*down*/ && ystep < 0)
		ystep *= -1;
//=======next
	next_x = x_intercept;
	next_y = y_intercept;
	while (next_x >= 0 && next_x < data->map.sq_size * data->map.width && next_y >= 0 && next_y <  data->map.sq_size * data->map.heigth)
	{
		x = next_x;
		if (data->ray.left)
			x--;
		
		//printf("next_x = %f, next_y = %f\n", next_x, next_y);
		//printf("width = %d, heigth = %d\n", data->res.width, data->res.heigth);
		if(has_wall(x, next_y, data) == '1' ||has_wall(x, next_y, data) == ' ')//TODO laisser ' '?
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

int		find_text_pixel(t_t text, int x_texture, int y_texture)
{
	char	*color;
	if (y_texture < 0)
		y_texture = 0;
	if (y_texture > text.heigth)
		y_texture = text.heigth;
	if (x_texture < 0)
		x_texture = 0;
	if (x_texture > text.width)
		x_texture = text.width;
	color = text.imgs + (y_texture * text.size_line + x_texture * text.bpp / 8);
	return (*(int*)color);
}

void	disp_wall_text(t_d *data, int col, int w_begin, int w_end, t_t *text, int wall_heigth)
{
	int x_texture;
	int y_texture;
	int y;
	int color;

	if(data->ray.found_v)
		x_texture = fmod(data->ray.v_hit_y / data->map.sq_size, 1) * text->width;
	if(data->ray.found_h)
		x_texture = fmod(data->ray.h_hit_x / data->map.sq_size, 1) * text->width;
	if ((data->ray.down && data->ray.found_h)/*south*/ || (data->ray.left && data->ray.found_v)/*west*/)
        x_texture = 64 - x_texture;	
	if (w_begin < 0)
		w_begin = 0;
	if (w_end > data->res.heigth)
		w_end = data->res.heigth;
	y = w_begin;
	while (y < w_end)
	{
		y_texture = (y + (wall_heigth / 2.0) - (data->res.heigth / 2.0)) * ((double)text->heigth / (double)wall_heigth);
		my_mlx_pixel_put(data, col, y, find_text_pixel(*text, x_texture, y_texture));
		y++;
	}	
}

void	wall_display(t_d *data, int column)
{
	double	dist_plane;
	double	wall_heigth;
	int		wall_begin;
	int		wall_end;
	double	correct_dist;

	correct_dist = data->ray.ray_dist[column] * cos(data->ray.ray_angle - data->cam.rotate_angle);
	dist_plane = (data->res.width / 2) / tan(data->ray.fov_angle / 2);
	wall_heigth = (data->map.sq_size / correct_dist) * dist_plane;

	wall_begin = (data->res.heigth / 2) - (wall_heigth / 2);
	wall_end = wall_begin + wall_heigth;

	disp_vertical_line(data, column, 0, wall_begin, data->color.ceiling);
	disp_vertical_line(data, column, wall_end, data->res.heigth, data->color.floor);
//================wall Texture
	if(data->ray.up && data->ray.found_h)//north
		disp_wall_text(data, column, wall_begin, wall_end, &data->color.no, wall_heigth);
	else if(data->ray.down && data->ray.found_h)//south
		disp_wall_text(data, column, wall_begin, wall_end, &data->color.so, wall_heigth);

	else if(data->ray.left && data->ray.found_v)//west
		disp_wall_text(data, column, wall_begin, wall_end, &data->color.we, wall_heigth);

	else if(data->ray.right && data->ray.found_v)//east
		disp_wall_text(data, column, wall_begin, wall_end, &data->color.ea, wall_heigth);
}

void	cast_rays(t_d *data)
{
	//printf("castray______\n");
	int column;
	column = 0;
	
	data->ray.ray_angle = data->cam.rotate_angle - (data->ray.fov_angle /2);
	while (column < data->ray.nb_rays)
	{
		data->ray.ray_angle += data->ray.fov_angle/data->ray.nb_rays;
		data->ray.ray_angle = normalize_angle(data->ray.ray_angle);
		ray_dir(data);	
		find_hz_hit(data);
		find_vt_hit(data);
		calcul_hit_dist(data, column);
//3d======		
		wall_display(data, column);
//2d======
//		draw_line(data->cam.x, data->cam.y, data->ray.hit_x , data->ray.hit_y, 0x00ff00, data);
		column++;
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
	if (data->cam.side_move == 1)
	{
		data->cam.x += cos(data->cam.rotate_angle + M_PI_2) * move_step;
		data->cam.y += sin(data->cam.rotate_angle + M_PI_2) * move_step;
	}
	else	
	{
		data->cam.x += cos(data->cam.rotate_angle) * move_step;
		data->cam.y += sin(data->cam.rotate_angle) * move_step;
	}
	if (has_wall(data->cam.x, data->cam.y, data) == '1')
	{
		data->cam.x = old_x;
		data->cam.y = old_y;
	}
//DISPLAY========
/*
	ft_disp_map(data->map, data->ptr, data);
	disp_square(data->cam.x, data->cam.y, 0xffff00, data, 3);

	cast_rays(data);
	draw_line(data->cam.x, data->cam.y, (data->cam.x + cos(data->cam.rotate_angle) * 30), (data->cam.y + sin(data->cam.rotate_angle) * 30), 0x000000, data);*/
//========mini map	
	
	cast_rays(data);
	ft_disp_minimap(data->map, data->ptr, data);
	disp_square(data->cam.x * 0.2, data->cam.y * 0.2, 0xffff00, data, 3 * 0.2);	
	draw_line(data->cam.x * 0.2, data->cam.y * 0.2, data->ray.hit_x * 0.2, data->ray.hit_y * 0.2, 0x00ff00, data);
	draw_line(data->cam.x * 0.2, data->cam.y * 0.2, (data->cam.x + cos(data->cam.rotate_angle) * 30) * 0.2, (data->cam.y + sin(data->cam.rotate_angle) * 30) * 0.2, 0x000000, data);
	
//	print_tab_double(data->ray.ray_dist, data->ray.nb_rays);
//==========sprite
	sprite_init(data);
	mlx_put_image_to_window(data->ptr.mlx, data->ptr.window, data->ptr.img, 0, 0);
	mlx_do_sync(data->ptr.mlx);
	return (0);
}

void	free_mlx(t_d *data)
{
	if (data->ptr.img)
		mlx_destroy_image(data->ptr.mlx, data->ptr.img);
	if (data->color.no.img)
		mlx_destroy_image(data->ptr.mlx, data->color.no.img);
	if (data->color.so.img)
		mlx_destroy_image(data->ptr.mlx, data->color.so.img);
	if (data->color.ea.img)
		mlx_destroy_image(data->ptr.mlx, data->color.ea.img);	
	if (data->color.we.img)
		mlx_destroy_image(data->ptr.mlx, data->color.we.img);	
	if (data->color.s.img)
		mlx_destroy_image(data->ptr.mlx, data->color.s.img);
	//mlx_do_key_autorepeaton(params->mlx_ptr);
	//mlx_do_sync(params->mlx_ptr);
	if (data->ptr.window)
		mlx_destroy_window(data->ptr.mlx, data->ptr.window);
}

//void	free_struct(t_d *data)
//{

//}

int	ft_exit_game(t_d *data)
{
	//free_struct(data);
//	free_mlx(data);
	exit(EXIT_FAILURE);
	return (0);
}

int		main(int argc, char **argv)
{
	t_d		data;

	//int		pos;
	//int		*img_s;
	ft_memset(&data, 0, sizeof data);
	data.ptr.mlx = mlx_init();
	parse_cub(argv[1], &data);
	ft_set_params(&data);
	mlx_hook(data.ptr.window, KEYPRESS, 1L<<0, key_press, (void *)&data);
	mlx_hook(data.ptr.window, KEYRELEASE, 1L<<1, key_release, (void *)&data.cam);
	mlx_hook(data.ptr.window, 33, (1L << 17), ft_exit_game, data.ptr.mlx);
	mlx_loop_hook(data.ptr.mlx, ft_loop, &data);
	mlx_loop(data.ptr.mlx);
}
