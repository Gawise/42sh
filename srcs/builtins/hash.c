#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

static void	hash_usage_error(void)
{
	ft_dprintf(2, "hash: usage: hash [-r] [name ...]\n");
}
/*
static void	hash_not_found(char *cmd)
{
	ft_dprintf(2, "hash: %s: not found\n", cmd);
}
*/
int		hash_check_opt(t_process *p, int *ac)
{
	int		i;
	int		ret;
	int		r_opt;

	r_opt = 0;
	i = 0;
	while ((ret = ft_getopt(ac, &i, p->av, "r")) != -1)
	{
		if (ret == 'r')
			r_opt = 1;
		else if (ret == '?')
		{
			hash_usage_error();
			return (-1);
		}
	}
	return (r_opt);
}

int		hash_fill_map(t_hash_map **map, char *path, char *cmd)
{
	char	*file;

	if (!cmd)
		return (0);
	if (!(file = ft_which(path, cmd)))
		return (0);
	if (!*map)
		*map = ft_hash_init(100);
	if (!ft_hash_add(*map, cmd, file))
		ft_ex("Cannot allocate memory\n");
	return (1);
}

static void		hash_print_node(t_list *lst)
{
	t_hash_node	*node;

	node = (t_hash_node *)lst->data;
	if (node)
		ft_printf("%s=%s\n", node->key, node->data);
}

void		hash_print_tab(t_hash_map *map)
{
	size_t		i;
	t_list		*slot;

	i = 0;
	if (!map)
	{
		ft_dprintf(2, "hash: hash table empty\n");
		return ;
	}
	while (i < map->size)
	{
		if ((slot = map->slots[i]))
			ft_lstiter(slot, hash_print_node);
		i++;
	}
}

uint8_t		ft_hash(t_job *j, t_process *p)
{
	int		r_opt;
	t_hash_map	**map;
	int		ac;

	(void)j;
	r_opt = 0;
	ac = 1;
	map = &cfg_shell()->map;
	if (!p)
		return (1);
	if (!p->av[1])
		hash_print_tab(*map);
	if ((r_opt = hash_check_opt(p, &ac)) == -1)
		return (1);
	if (r_opt)
		ft_hash_del_map(map, free);
	while (p->av[ac])
	{
		if (!hash_fill_map(map,
		find_var_value(p->env, "PATH"), p->av[ac]))
			return (1);
		ac++;
	}
	return (0);
}
