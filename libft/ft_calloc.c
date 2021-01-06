/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:14:28 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/17 15:16:18 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*alloc;
	size_t	i;

	i = 0;
	alloc = malloc(size * count);
	if (alloc == NULL)
		return (NULL);
	while (i < (count * size))
	{
		alloc[i] = 0;
		i++;
	}
	return ((void *)alloc);
}
