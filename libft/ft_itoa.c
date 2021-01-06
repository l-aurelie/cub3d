/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:25:51 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/19 20:08:52 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*size_str(long int nb, int *i)
{
	char	*str;

	while (nb / 10 > 0)
	{
		nb = nb / 10;
		*i = *i + 1;
	}
	str = malloc(sizeof(char) * (*i + 1));
	if (str == NULL)
		return (NULL);
	str[*i] = '\0';
	return (str);
}

static void	ft_fillitoa(char *result, int i, long int nb)
{
	while (nb != 0)
	{
		result[--i] = (nb % 10) + '0';
		nb = nb / 10;
	}
}

char		*ft_itoa(long int nb)
{
	char		*result;
	int			i;
	int			neg_sign;

	neg_sign = 0;
	i = 1;
	if (nb < 0)
	{
		nb = -nb;
		neg_sign = 1;
		i++;
	}
	result = size_str(nb, &i);
	if (result == NULL)
		return (NULL);
	if (nb == 0)
		result[--i] = (nb % 10) + '0';
	ft_fillitoa(result, i, nb);
	if (neg_sign)
		result[0] = '-';
	return (result);
}
