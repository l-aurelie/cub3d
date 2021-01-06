/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:24:12 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/17 16:26:13 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	cc;

	i = ft_strlen(s);
	cc = (char)c;
	while (i >= 0)
	{
		if (s[i] == cc)
			return ((char*)s + i);
		i--;
	}
	return (NULL);
}
