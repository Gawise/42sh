/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symlink.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:03 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:03 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <sys/types.h>
#include <sys/stat.h>

uint8_t		test_symlink_file(char *path)
{
	struct stat buf;

	if (stat(path, &buf))
		return (FAILURE);
	return (((buf.st_mode & S_IFMT) == S_IFLNK) ? 0 : 1);
}
