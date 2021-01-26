/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 13:23:26 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/22 15:41:16 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*prep_line(char *buff, char *line_result, int len)
{
	char	*line_end;
	int		i;

	i = 0;
	line_end = malloc(sizeof(char) * (len + 1));
	if (line_end == NULL)
		return (NULL);
	len = 0;
	while (buff[len] != '\n')
	{
		line_end[len] = buff[len];
		len++;
	}
	line_end[len] = 0;
	while (buff[++len])
	{
		buff[i] = buff[len];
		i++;
	}
	buff[i] = 0;
	if (!(line_result = ft_strjoin_line(line_result, line_end)))
		return (NULL);
	free(line_end);
	return (line_result);
}

int		ft_read_line(int fd, char **line, char *buff, char *line_result)
{
	int len;
	int ret;

	ret = 1;
	while ((len = ft_check_buffer_nl(buff)) == -1 && ret > 0)
	{
		if (!(line_result = ft_strjoin_line(line_result, buff)))
			return (-1);
		ret = read(fd, buff, BUFFER_SIZE);
		if (ret == -1)
			return (-1);
		buff[ret] = 0;
	}
	if ((len = ft_check_buffer_nl(buff)) != -1)
	{
		if (!(*line = prep_line(buff, line_result, len)))
			return (-1);
		return (1);
	}
	if (!(*line = ft_strjoin_line(line_result, buff)))
		return (-1);
	if (**line && ret == 0)
		return (1);
	if (ret == 0)
		return (0);
	return (-1);
}

int		get_next_line(int fd, char **line)
{
	static char	buff[BUFFER_SIZE + 1];
	char		*line_result;

	if (BUFFER_SIZE < 1 || fd < 0 || read(fd, 0, 0) < 0 || !line)
		return (-1);
	line_result = malloc(sizeof(char) * 1);
	if (line_result == NULL)
		return (-1);
	line_result[0] = '\0';
	return (ft_read_line(fd, line, buff, line_result));
}
