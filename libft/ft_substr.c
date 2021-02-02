/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:28:57 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/02 17:06:43 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char *s, int start, int len, int to_free)
{
	char	*sub;
	int		size;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		size = 0;
	else
		size = ft_strlen(s + start);
	if (size > len)
		size = len;
	sub = malloc(sizeof(char) * (size + 1));
	if (sub == NULL)
		return (NULL);
	while (i < size)
		sub[i++] = s[start++];
	sub[i] = 0;
	if (to_free == 1)
		ft_free((void **)&s);
	return (sub);
}
