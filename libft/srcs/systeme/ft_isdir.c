/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:53:25 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/16 17:58:54 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <sys/stat.h>

int		ft_isdir(char *path, int link)
{
	struct stat info;

	if (!link)
		lstat(path, &info);
	else
		stat(path, &info);
	return (S_ISDIR(info.st_mode));
}
