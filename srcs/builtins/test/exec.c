/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:45:59 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:45:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"
#include <sys/types.h>
#include <sys/stat.h>

uint8_t		test_exec_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (FAILURE);
	return ((buf.st_mode & S_IXUSR) ? 0 : 1);
}
