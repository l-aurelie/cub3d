/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 17:46:10 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/02 23:32:28 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# define BUFFER_SIZE 20

typedef	struct	s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;
int				ft_isalpha(int c);
int				ft_islower(int c);
int				ft_isupper(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_strlen(const char *s);
int				ft_toupper(int c);
int				ft_tolower(int c);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
size_t			ft_strlcat(char	*dst, const char *src, size_t size);
char			*ft_strnstr(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *str);
char			*ft_strdup(const char *s1);
void			*ft_calloc(size_t count, size_t size);
void			ft_bzero(void *s, size_t n);
void			*ft_memset(void *b, int c, size_t n);
void			*ft_memcpy(void *s1, const void *s2, size_t n);
void			*ft_memccpy(void *s1, const void *s2, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memmove(void *s1, const void *s2, size_t n);
char			*ft_substr(char *s, int start, int len, int to_free);
char			*ft_strtrim(char *s1, char const *set, int to_free);
char			*ft_strjoin(char *s1, char *s2, int f_1, int f_2);
char			**ft_split(char const *s, char c);
char			*ft_itoa(long int nb);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **alst, t_list *new);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **alst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *), \
				void (*del)(void *));

int				ft_strcmp(char *s1, char *s2);

int				ft_chr_pos(char *str, char c);
int				is_charset(char c, char *charset);
int				ft_free(void **ptr);

int				ft_check_buffer_nl(char *buffer);
char			*ft_strjoin_line(char *s1, char const *s2);
char			*prep_line(char *buff, char *line_result, int len);
int				ft_read_line(int fd, char **line, char *buff, \
				char *line_result);

int				get_next_line(int fd, char **line);

#endif
