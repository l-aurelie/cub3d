/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:42:29 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/22 15:47:03 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_check_buffer_nl(char *buffer)
{
	int i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_strjoin_line(char *s1, char const *s2)
{
	char	*joined;
	size_t	size;
	int		i;

	i = 0;
	if (!s1 && !s2)
		return (NULL);
	while (s1 && s1[i])
		i++;
	size = i;
	i = 0;
	while (s2 && s2[i])
		i++;
	size += i;
	joined = malloc(sizeof(char) * (size + 1));
	if (joined == NULL)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
		*joined++ = s1[i++];
	while (s2 && *s2)
		*joined++ = *s2++;
	*joined = 0;
	free(s1);
	return (joined - size);
}
