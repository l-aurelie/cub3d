# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/02 22:39:56 by aleconte          #+#    #+#              #
#    Updated: 2021/02/03 00:05:39 by aleconte         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = Cub3D

SRCS      = $(addprefix src/,bmp.c raycasting.c utils2.c cub3d.c raycasting_hit.c utils.c draw_utils.c parsing.c render.c error.c parsing_color_texture.c sprite.c exit.c map.c player.c)

OBJS      = ${SRCS:.c=.o}

DEP       = ${OBJS:.o=.d}

CC        = gcc

MLX_DIR = minilibx-linux

CFLAGS    = -I libft -I ${MLX_DIR} -Iincludes -Wall -Wextra -Werror

LDFLAGS    = -Llibft -Lminilibx-linux

LDLIBS    = -lm -lmlx -lXext -lX11 -lft

RM        = rm -f

all :     libft2 minilibx ${NAME}

libft2 : 
		make -C libft
minilibx : 
		make -C ${MLX_DIR}

$(NAME) : ${OBJS}
		  ${CC} -o ${NAME} ${OBJS} ${CFLAGS} ${LDFLAGS} ${LDLIBS}

clean :   
					${RM} ${OBJS}

fclean :  clean
	        ${RM} ${NAME}
			make -C libft fclean
			make -C ${MLX_DIR} clean

lib_fclean : 
			./libft/ make fclean

re :      fclean all

.PHONY :  all clean fclean re
