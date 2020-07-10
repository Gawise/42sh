/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:42 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:42 by user42           ###   ########.fr       */
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
