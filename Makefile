NAME      = Cub3D

SRCS      = cub3d.c parsing.c parsing_map.c others.c bmp.c

OBJS      = ${SRCS:.c=.o}

DEP       = ${OBJS:.o=.d}

CC        = gcc

MLX_DIR = minilibx-linux

CFLAGS    = -I libft -I ${MLX_DIR} -Iincludes -fsanitize=address -g


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
					${RM} ${OBJS} ${DEP}

fclean :  clean
	        ${RM} ${NAME}
			make -C libft fclean
			make -C ${MLX_DIR} clean

lib_fclean : 
			./libft/ make fclean

re :      fclean all

-include  ${OBJS:.o=.d}

.PHONY :  all clean fclean re
