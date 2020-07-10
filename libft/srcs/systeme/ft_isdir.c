/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:10:09 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:10:09 by user42           ###   ########.fr       */
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
