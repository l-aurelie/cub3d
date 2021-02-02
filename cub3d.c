#include "./includes/cub3d.h"
#include <math.h>

int		key_press(int key, t_data *d)
{
	if (key == KEY_W)
		d->cam.walk_dir = 1;
	else if (key == KEY_S)
		d->cam.walk_dir = -1;
	else if (key == KEY_D)
		d->cam.side_dir = 1;
	else if (key == KEY_A)
		d->cam.side_dir = -1;
	else if (key == KEY_LEFT)
		d->cam.turn_dir = -1;
	else if (key == KEY_RIGHT)
		d->cam.turn_dir = 1;
	else if (key == KEY_ESC)
		ft_exit_game(d);
	return (0);
}

int		key_release(int key, t_data *d)
{
	if (key == KEY_W)
		d->cam.walk_dir = 0;
	else if (key == KEY_S)
		d->cam.walk_dir = 0;
	else if (key == KEY_D)
		d->cam.side_dir = 0;
	else if (key == KEY_A)
		d->cam.side_dir = 0;
	else if (key == KEY_LEFT)
		d->cam.turn_dir = 0;
	else if (key == KEY_RIGHT)
		d->cam.turn_dir = 0;
	return (0);
}

void	ft_set_params(t_data *d)
{
	d->ptr.img = mlx_new_image(d->ptr.mlx, d->res.width, d->res.heigth);
	if (d->ptr.img == NULL)
		error("fonction mlx_new_image goes wrong\n", d);
	d->ptr.imgs = mlx_get_data_addr(d->ptr.img, &d->color.bpp,
		&d->res.size_line, &d->color.endian);
	d->cam.move_speed = 2.0;
	d->cam.rotate_speed = 2.0 * (M_PI / 180);
	d->ray.fov = 60 * (M_PI / 180);
	d->ray.nb_rays = d->res.width;
	d->ray.ray_dist = malloc(sizeof(double) * d->res.width);//TODO protection
	if (d->ray.ray_dist == NULL)
		error("malloc error", d);
	if (d->pars.save == 0)
	{
		d->ptr.window = mlx_new_window(d->ptr.mlx, d->res.width, d->res.heigth,
		"cub3D");
		if (d->ptr.window == NULL)
			error("can't open window\n", d);
		mlx_hook(d->ptr.window, KEYPRESS, 1L << 0, key_press, (void *)d);
		mlx_hook(d->ptr.window, KEYRELEASE, 1L << 1, key_release, (void *)d);
		mlx_hook(d->ptr.window, 33, (1L << 17), ft_exit_game, (void *)d);
	}
}

void	calculate_dist_sprite(t_st *sprite, t_data *d)
{
	sprite->dist = sqrt((d->cam.x - sprite->pos.x) * (d->cam.x -
	sprite->pos.x) + (d->cam.y - sprite->pos.y) * (d->cam.y - sprite->pos.y));
}

void	sort_sprite_tab(t_data *d)
{
	int		i;
	t_st	tmp;

	i = 0;
	while (i < d->spri.nb_sprite - 1)
	{
		if (d->spri.tab[i].dist < d->spri.tab[i + 1].dist && (i + 1) <
			d->spri.nb_sprite)
		{
			tmp = d->spri.tab[i];
			d->spri.tab[i] = d->spri.tab[i + 1];
			d->spri.tab[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
}

void	calculate_sprite_angle(t_st *sprite, t_data *d)
{
	double	vectx;
	double	vecty;
	double	total_angle;

	vectx = sprite->pos.x - d->cam.x;
	vecty = sprite->pos.y - d->cam.y;
	total_angle = atan2(vecty, vectx);
	sprite->angle = total_angle - d->cam.rotate_angle;
	sprite->angle = normalize_angle(sprite->angle);
	sprite->visible = 0;
	if (d_abs(sprite->angle) < d->ray.fov / 2. + 0.2 || d_abs(sprite->angle) >
		2 * M_PI - d->ray.fov / 2 - 0.2)
		sprite->visible = 1;
}

void	sprite_draw(t_data *d, t_st *sprite, t_rend *s, int x_begin)
{
	double wall_dist;

	s->curr.x = 0;
	while (x_begin + s->curr.x < 0)
		s->curr.x++;
	while (s->curr.x < s->heigth && x_begin + s->curr.x < d->res.width)
	{
		wall_dist = d->ray.ray_dist[(int)(x_begin + s->curr.x)];
		if (wall_dist > sprite->dist)
		{
			s->text.x = s->curr.x * d->color.s.width / s->heigth;
			s->curr.y = s->begin;
			while (s->curr.y < s->end)
			{
				s->text.y = (s->curr.y + (s->heigth / 2.0) - (d->res.heigth /
					2.0)) * ((double)d->color.s.heigth / (double)s->heigth);
				s->color = find_text_pixel(d->color.s, s->text.x, s->text.y);
				if (s->color != s->transparence)
					my_pixel_put(d, x_begin + s->curr.x, s->curr.y, s->color);
				s->curr.y++;
			}
		}
		s->curr.x++;
	}
}

void	sprite_display(t_data *d, t_st *sprite)
{
	t_rend	s;
	int		x_begin;
	double	wall_dist;

//	s.heigth = (d->map.sq_size / sprite->dist) * d->dist_plane;
	s.heigth = d->dist_plane / (cos(sprite->angle) * sprite->dist /
		d->map.sq_size);
	s.begin = (d->res.heigth / 2) - (s.heigth / 2);
	s.end = s.begin + s.heigth;
	if (s.begin < 0)
		s.begin = 0;
	if (s.end > d->res.heigth)
		s.end = d->res.heigth;
	x_begin = (double)(d->res.width / 2.0) + tan(sprite->angle) *
		(double)(d->res.width / 2.0) / tan(d->ray.fov / 2.0) - s.heigth / 2;
//	x_begin = d->dist_plane * tan(sprite->angle) + (d->res.width / 2.0) -
	//	(s.heigth / 2);
	s.transparence = find_text_pixel(d->color.s, 0, 0);
	sprite_draw(d, sprite, &s, x_begin);
}

void	sprite_render(t_data *d)
{
	int i;

	i = 0;
	while (i < d->spri.nb_sprite)
	{
		calculate_dist_sprite(&d->spri.tab[i], d);
		calculate_sprite_angle(&d->spri.tab[i], d);
		i++;
	}
	sort_sprite_tab(d);
	i = 0;
	while (i < d->spri.nb_sprite)
	{
		if (d->spri.tab[i].visible)
			sprite_display(d, &d->spri.tab[i]);
		i++;
	}
}

char	has_wall(double x, double y, t_data *d)
{
	int grid_index_x;
	int grid_index_y;

	grid_index_x = floor(x / d->map.sq_size);
	grid_index_y = floor(y / d->map.sq_size);
	if (grid_index_x < 0 || grid_index_x >= d->map.width || grid_index_y < 0 ||
		grid_index_y >= d->map.heigth)
		return ('1');
	return (d->map.grid[grid_index_y][grid_index_x]);
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
	return (angle);
}

void	ray_dir(t_data *d)
{
	d->ray.down = 0;
	d->ray.up = 0;
	d->ray.right = 0;
	d->ray.left = 0;
	if (d->ray.ray_angle > 0 && d->ray.ray_angle < M_PI)
		d->ray.down = 1;
	if (!(d->ray.ray_angle > 0 && d->ray.ray_angle < M_PI))
		d->ray.up = 1;
	if (d->ray.ray_angle < 0.5 * M_PI || d->ray.ray_angle > 1.5 * M_PI)
		d->ray.right = 1;
	if (!(d->ray.ray_angle < 0.5 * M_PI || d->ray.ray_angle > 1.5 * M_PI))
		d->ray.left = 1;
}

double	calcul_dist(double x1, double x2, double y1, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	calcul_hit_dist(t_data *d, int column)
{
	d->ray.h_dist = 0;
	d->ray.v_dist = 0;
	if (d->ray.found_h == 1)
		d->ray.h_dist = calcul_dist(d->cam.x, d->ray.h_hit_x, d->cam.y,
			d->ray.h_hit_y);
	if (d->ray.found_v == 1)
		d->ray.v_dist = calcul_dist(d->cam.x, d->ray.v_hit_x, d->cam.y,
			d->ray.v_hit_y);
	if ((d->ray.v_dist < d->ray.h_dist && d->ray.v_dist != 0) ||
			d->ray.h_dist == 0)
	{
		d->ray.ray_dist[column] = d->ray.v_dist;
		d->ray.hit_x = d->ray.v_hit_x;
		d->ray.hit_y = d->ray.v_hit_y;
		d->ray.found_h = 0;
	}
	else
	{
		d->ray.ray_dist[column] = d->ray.h_dist;
		d->ray.hit_x = d->ray.h_hit_x;
		d->ray.hit_y = d->ray.h_hit_y;
		d->ray.found_v = 0;
	}
}

void	find_hz_step_and_intercept(t_data *d, t_dpos *intercept, t_dpos *step)
{
	d->ray.found_h = 0;
	intercept->y = floor(d->cam.y / d->map.sq_size) * d->map.sq_size;
	if (d->ray.down)
		intercept->y += d->map.sq_size;
	intercept->x = d->cam.x + (intercept->y - d->cam.y) / tan(d->ray.ray_angle);
	step->y = d->map.sq_size;
	if (d->ray.up)
		step->y *= -1;
	step->x = d->map.sq_size / tan(d->ray.ray_angle);
	if ((d->ray.left && step->x > 0) || (d->ray.right && step->x < 0))
		step->x *= -1;
}

void	find_hz_hit(t_data *d)
{
	t_dpos	next;
	t_dpos	step;
	double	y;

	find_hz_step_and_intercept(d, &next, &step);
	while (next.x >= 0 && next.x < d->map.sq_size * d->map.width && next.y >=
		0 && next.y < d->map.sq_size * d->map.heigth)
	{
		y = next.y;
		if (d->ray.up)
			y--;
		if (has_wall(next.x, y, d) == '1' || has_wall(next.x, y, d) == ' ')
		{
			d->ray.found_h = 1;
			d->ray.h_hit_x = next.x;
			d->ray.h_hit_y = next.y;
			break ;
		}
		next.x += step.x;
		next.y += step.y;
	}
}

void	find_vt_step_and_intercept(t_data *d, t_dpos *intercept, t_dpos *step)
{
	d->ray.found_v = 0;
	intercept->x = floor(d->cam.x / d->map.sq_size) * d->map.sq_size;
	if (d->ray.right)
		intercept->x += d->map.sq_size;
	intercept->y = d->cam.y + (intercept->x - d->cam.x) * tan(d->ray.ray_angle);
	step->x = d->map.sq_size;
	if (d->ray.left)
		step->x *= -1;
	step->y = d->map.sq_size * tan(d->ray.ray_angle);
	if ((d->ray.up && step->y > 0) || (d->ray.down && step->y < 0))
		step->y *= -1;
}

void	find_vt_hit(t_data *d)
{
	t_dpos	step;
	t_dpos	next;
	double	x;

	find_vt_step_and_intercept(d, &next, &step);
	while (next.x >= 0 && next.x < d->map.sq_size * d->map.width && next.y >=
		0 && next.y < d->map.sq_size * d->map.heigth)
	{
		x = next.x;
		if (d->ray.left)
			x--;
		if (has_wall(x, next.y, d) == '1' || has_wall(x, next.y, d) == ' ')
		{
			d->ray.found_v = 1;
			d->ray.v_hit_x = next.x;
			d->ray.v_hit_y = next.y;
			break ;
		}
		next.x += step.x;
		next.y += step.y;
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

void	disp_wall_text(t_data *d, int col, t_rend *wall, t_t *text)
{
	if (d->ray.found_v)
		wall->text.x = fmod(d->ray.v_hit_y / d->map.sq_size, 1) * text->width;
	if (d->ray.found_h)
		wall->text.x = fmod(d->ray.h_hit_x / d->map.sq_size, 1) * text->width;
	if ((d->ray.down && d->ray.found_h) || (d->ray.left && d->ray.found_v))
		wall->text.x = text->width - 1 - wall->text.x;
	if (wall->begin < 0)
		wall->begin = 0;
	if (wall->end > d->res.heigth)
		wall->end = d->res.heigth;
	wall->curr.y = wall->begin;
	while (wall->curr.y < wall->end)
	{
		wall->text.y = (wall->curr.y + (wall->heigth / 2.0) - (d->res.heigth /
			2.0)) * ((double)text->heigth / (double)wall->heigth);
		my_pixel_put(d, col, wall->curr.y, find_text_pixel(*text, wall->text.x,
			wall->text.y));
		wall->curr.y++;
	}
}

void	wall_display(t_data *d, int column)
{
	t_rend	wall;
	t_ipos pos;
	double	correct_dist;

	correct_dist = d->ray.ray_dist[column] * cos(d->ray.ray_angle -
		d->cam.rotate_angle);
	d->dist_plane = (d->res.width / 2) / tan(d->ray.fov / 2);
	wall.heigth = (d->map.sq_size / correct_dist) * d->dist_plane;
	wall.begin = (d->res.heigth / 2) - (wall.heigth / 2);
	wall.end = wall.begin + wall.heigth;
	pos.x = column;
	pos.y = 0;
	disp_vertical_line(d, pos, wall.begin, d->color.ceiling);
	pos.y = wall.end;
	disp_vertical_line(d, pos, d->res.heigth, d->color.floor);
	if (d->ray.up && d->ray.found_h)
		disp_wall_text(d, column, &wall, &d->color.no);
	else if (d->ray.down && d->ray.found_h)
		disp_wall_text(d, column, &wall, &d->color.so);
	else if (d->ray.left && d->ray.found_v)
		disp_wall_text(d, column, &wall, &d->color.we);
	else if (d->ray.right && d->ray.found_v)
		disp_wall_text(d, column, &wall, &d->color.ea);
}

void	cast_rays(t_data *d)
{
	int column;

	column = 0;
	d->dist_plane = (d->res.width / 2) / tan(d->ray.fov / 2);
	//d->ray.ray_angle = d->cam.rotate_angle - (d->ray.fov / 2);
	//d->ray.ray_angle = d->cam.rotate_angle + atan((column -
		//(d->res.width / 2)) / ((d->res.width / 2) / tan((d->ray.fov / 2))));
	while (column < d->ray.nb_rays)
	{
		d->ray.ray_angle = d->cam.rotate_angle + atan((column - (d->res.width /
			2)) / d->dist_plane);
	//	d->ray.ray_angle += d->ray.fov / d->res.width;
		d->ray.ray_angle = normalize_angle(d->ray.ray_angle);
		ray_dir(d);
		find_hz_hit(d);
		find_vt_hit(d);
		calcul_hit_dist(d, column);
		wall_display(d, column);
		column++;
	}
}

void	ft_mini_map(t_data *d)
{
	int width_minimap;
	int heigth_minimap;
	t_sq square;

	square.width = 16;
	square.ratio = 0.2;
	square.pos.x = d->cam.x - 8;
	square.pos.y = d->cam.y - 8;
	width_minimap = d->map.width * d->map.sq_size * 0.2;
	heigth_minimap = d->map.heigth * d->map.sq_size * 0.2;
	if (width_minimap < d->res.width / 2 && heigth_minimap < d->res.heigth / 2)
	{
		ft_disp_minimap(d->map, d);
		disp_square(square, 0xff0000, d);
	}
}

void	raycasting(t_data *d)
{
	cast_rays(d);
	sprite_render(d);
	ft_mini_map(d);
}

void	player_move(t_data *d)
{
	double	move_step;
	double	old_x;
	double	old_y;

	old_x = d->cam.x;
	old_y = d->cam.y;
	if (d->cam.side_dir)
	{
		move_step = d->cam.side_dir * d->cam.move_speed;
		d->cam.x += cos(d->cam.rotate_angle + M_PI_2) * move_step;
		d->cam.y += sin(d->cam.rotate_angle + M_PI_2) * move_step;
	}
	if (d->cam.walk_dir)
	{
		move_step = d->cam.walk_dir * d->cam.move_speed;
		d->cam.x += cos(d->cam.rotate_angle) * move_step;
		d->cam.y += sin(d->cam.rotate_angle) * move_step;
	}
	if (has_wall(d->cam.x, d->cam.y, d) != '0')
	{
		d->cam.x = old_x;
		d->cam.y = old_y;
	}
}

int		ft_loop(t_data *d)
{
	d->cam.rotate_angle += d->cam.turn_dir * d->cam.rotate_speed;
	d->cam.rotate_angle = normalize_angle(d->cam.rotate_angle);
	player_move(d);
	raycasting(d);
	mlx_put_image_to_window(d->ptr.mlx, d->ptr.window, d->ptr.img, 0, 0);
	return (0);
}

void	free_mlx(t_data *d)
{
	printf("free mlx");
	if (d->ptr.img)
		mlx_destroy_image(d->ptr.mlx, d->ptr.img);
	if (d->color.no.img)
		mlx_destroy_image(d->ptr.mlx, d->color.no.img);
	if (d->color.so.img)
		mlx_destroy_image(d->ptr.mlx, d->color.so.img);
	if (d->color.ea.img)
		mlx_destroy_image(d->ptr.mlx, d->color.ea.img);
	if (d->color.we.img)
		mlx_destroy_image(d->ptr.mlx, d->color.we.img);
	if (d->color.s.img)
		mlx_destroy_image(d->ptr.mlx, d->color.s.img);
	if (d->ptr.window)
		mlx_destroy_window(d->ptr.mlx, d->ptr.window);
	mlx_destroy_display(d->ptr.mlx);
	if (d->ptr.mlx)
		ft_free((void **)&d->ptr.mlx);
}

void	free_struct(t_data *d)
{
	if (d->ray.ray_dist)
		ft_free((void **)&d->ray.ray_dist);
	if (d->map.s_map)
		ft_free((void **)&d->map.s_map);
	if (d->map.grid)
		free_matrix(&d->map.grid, d->map.heigth);
	if (d->spri.tab)
		ft_free((void **)&d->spri.tab);
	if (d->pars.line)
		ft_free((void **)&d->pars.line);
	if (d->pars.split)
		free_split(&d->pars.split);
	if (d->pars.rgb)
		ft_free((void **)&d->pars.rgb);
}

int		ft_exit_game(t_data *d)
{
	free_struct(d);
	free_mlx(d);
	exit(EXIT_SUCCESS);
	return (0);
}

int		check_error_arguments(t_data *d, int argc, char **argv)
{
	int fd;
	int error_state;

	error_state = 0;
	if (argc < 2 || argc > 3)
		error_state = 1;
	else
	{
		if (ft_strcmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub"))
			error_state = 1;
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
			error_state = 1;
		close(fd);
		if (argc == 3 && ft_strcmp(argv[2], "--save") != 0)
			error_state = 1;
		if (argc == 3)
			d->pars.save = 1;
	}
	if (error_state)
	{
		printf("please enter program name followed by [map_description].cub.");
		exit(EXIT_FAILURE);
	}
	return (error_state);
}

int		main(int argc, char **argv)//29ligne
{
	t_data	d;

	ft_memset(&d, 0, sizeof(t_data));
	check_error_arguments(&d, argc, argv);
	d.ptr.mlx = mlx_init();
	if (d.ptr.mlx == NULL)
		error("fonction mlx_init goes wrong\n", &d);
	parse_cub(argv[1], &d);
	ft_set_params(&d);
	if (argc == 2)
	{
		mlx_loop_hook(d.ptr.mlx, ft_loop, &d);
		mlx_loop(d.ptr.mlx);
	}
	else if (argc == 3)
	{
		d.pars.save = 1;
		raycasting(&d);
		ft_create_bmp(&d);
		ft_exit_game(&d);
	}
	return (0);
}
