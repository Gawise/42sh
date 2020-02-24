/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:26 by hmerieux          #+#    #+#             */
/*   Updated: 2020/02/11 22:35:55 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_dlist	*ft_dlstnew(void *content, size_t content_size)
{
	t_dlist	*lstnew;

	if (!(lstnew = (t_dlist *)malloc(sizeof(t_dlist))))
		return (NULL);
	if (content == NULL)
	{
		lstnew->data = NULL;
		lstnew->size = 0;
	}
	else
	{
		lstnew->data = content;
		lstnew->size = content_size;
	}
	lstnew->prev = NULL;
	lstnew->next = NULL;
	return (lstnew);
}
