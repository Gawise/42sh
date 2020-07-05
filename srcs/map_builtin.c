#include "libft.h"
#include "exec.h"
#include "sh.h"

static	void	feed_tab_value(int *tabi)
{
	tabi[0] = B_ECHO;
	tabi[1] = B_CD;
	tabi[2] = B_ENV;
	tabi[3] = B_SET;
	tabi[4] = B_UNSET;
	tabi[5] = B_HASH;
	tabi[6] = B_EXIT;
	tabi[7] = B_JOBS;
	tabi[8] = B_FG;
	tabi[9] = B_BG;
	tabi[10] = B_TYPE;
	tabi[11] = B_EXPORT;
	tabi[12] = B_ALIAS;
	tabi[13] = B_UNALIAS;
	tabi[14] = B_TEST;
	tabi[15] = B_FC;
}

static	void	feed_tab_key(char **tabi)
{
	tabi[0] = "echo";
	tabi[1] = "cd";
	tabi[2] = "env";
	tabi[3] = "set";
	tabi[4] = "unset";
	tabi[5] = "hash";
	tabi[6] = "exit";
	tabi[7] = "jobs";
	tabi[8] = "fg";
	tabi[9] = "bg";
	tabi[10] = "type";
	tabi[11] = "export";
	tabi[12] = "alias";
	tabi[13] = "unalias";
	tabi[14] = "test";
	tabi[15] = "fc";
}

void			init_builtin_map(t_hash_map *map)
{
	uint8_t		i;
	int32_t		*value;
	int32_t		tab_value[16];
	char		*tab_key[16];

	i = 0;
	feed_tab_value(tab_value);
	feed_tab_key(tab_key);
	while (i < 16)
	{
		value = ft_memalloc(sizeof(int32_t));
		protect_malloc(value);
		ft_hash_add(map, tab_key[i],
				ft_memcpy(value, &tab_value[i], sizeof(int32_t)), free);
		i++;
	}
}
