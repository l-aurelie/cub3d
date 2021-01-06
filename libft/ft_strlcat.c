/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:16:46 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/17 16:16:49 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t j;
	size_t l_dest;

	l_dest = ft_strlen(dst);
	i = 0;
	j = 0;
	if (size == 0)
		return (ft_strlen(src));
	if (l_dest >= size)
		return (size + ft_strlen(src));
	else
	{
		i = l_dest;
		while (src[j] && i < size - 1)
		{
			dst[i] = src[j];
			i++;
			j++;
		}
		dst[i] = '\0';
	}
	return (l_dest + ft_strlen(src));
}
