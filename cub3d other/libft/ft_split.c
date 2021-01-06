/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:59:43 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/19 18:00:12 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_strdup_word(char **split, char const *s, char c)
{
	int size;
	int word;

	word = 0;
	while (*s)
	{
		while (*s && (*s == c))
			s++;
		size = 0;
		while (s[size] && !(s[size] == c))
			size++;
		if (s[size] || size)
		{
			split[word] = malloc(sizeof(char) * (size + 1));
			if (split[word] == NULL)
				return (NULL);
			ft_strlcpy(split[word], s, size + 1);
			word++;
		}
		s = s + size;
	}
	return (split);
}

char		**ft_split(char const *s, char c)
{
	int		i;
	int		word;
	char	**split;

	split = NULL;
	word = 0;
	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		while (s[i] && (s[i] == c))
			i++;
		if (s[i])
			word++;
		while (s[i] && !(s[i] == c))
			i++;
	}
	split = malloc(sizeof(char *) * (word + 1));
	if (split == NULL)
		return (NULL);
	split[word] = 0;
	split = ft_strdup_word(split, s, c);
	return (split);
}
