/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:33:09 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/17 15:36:30 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*u_s;
	unsigned char	u_c;

	i = 0;
	u_s = (unsigned char *)s;
	u_c = (unsigned char)c;
	while (i < n)
	{
		if (u_s[i] == u_c)
			return ((void *)u_s + i);
		i++;
	}
	return (NULL);
}
