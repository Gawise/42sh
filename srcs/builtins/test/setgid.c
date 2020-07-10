/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setgid.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:02 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <sys/types.h>
#include <sys/stat.h>

uint8_t		test_setgid_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (FAILURE);
	return ((buf.st_mode & S_ISGID) ? 0 : 1);
}
