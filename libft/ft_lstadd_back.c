/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 14:05:07 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/18 14:07:07 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *last;
	t_list *begin;

	if (*alst)
	{
		begin = *alst;
		while (begin)
		{
			last = begin;
			begin = begin->next;
		}
		last->next = new;
		new->next = NULL;
	}
	else
		*alst = new;
}
