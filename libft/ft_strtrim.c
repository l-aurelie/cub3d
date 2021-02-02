/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:26:42 by aleconte          #+#    #+#             */
/*   Updated: 2021/02/02 17:09:30 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char *s1, char const *set, int to_free)
{
	int		len;
	char	*trimmed;
	char	*need_free;

	need_free = s1;
	if (!s1)
		return (NULL);
	if (!set)
		return ((char *)s1);
	while (*s1 && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen(s1);
	while (len >= 0 && ft_strchr(set, s1[len]))
		len--;
	if (to_free)
	{
		trimmed = ft_substr(s1, 0, len + 1, 0);
		ft_free((void **)&need_free);
	}
	else
		trimmed = ft_substr(s1, 0, len + 1, 0);
	return (trimmed);
}
