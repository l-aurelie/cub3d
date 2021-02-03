/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 23:31:43 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/03 15:07:02 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_H
# define BMP_H
# include "cub3d.h"

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
