/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleconte <aleconte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:20:22 by aleconte          #+#    #+#             */
/*   Updated: 2020/11/19 18:20:30 by aleconte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list *new_elem;

	new_elem = NULL;
	if ((new_elem = malloc(sizeof(t_list))))
	{
		new_elem->content = content;
		new_elem->next = NULL;
	}
	return (new_elem);
}
