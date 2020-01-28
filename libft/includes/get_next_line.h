/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:43:00 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:43:04 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 1083

int					get_next_line(const int fd, char **line);

typedef struct		s_gnl
{
	char			**content;
	int				num;
	struct s_gnl	*next;
}					t_gnl;
#endif
