/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:41:31 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/17 15:42:45 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *s1, const void *s2, size_t n)
{
	unsigned char		*u_s1;
	const unsigned char	*u_s2;

	u_s1 = (unsigned char *)s1;
	u_s2 = (unsigned char *)s2;
	if (!u_s1 && !u_s2)
		return (u_s1);
	if (u_s2 < u_s1)
	{
		while (n-- > 0)
		{
			u_s1[n] = u_s2[n];
		}
	}
	else
	{
		ft_memcpy(u_s1, u_s2, n);
	}
	return (u_s1);
}
