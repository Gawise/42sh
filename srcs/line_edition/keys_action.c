#include "line_edition.h"
#include "sh.h"
#include "struct.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>

void	set_scroll(t_cs_line *cs)
{
	int	scroll_add;

	if (cs)
	{
		cs->cr = get_line(cs);
		if (cs->cr + cs->min_row >= cs->screen.y && cs->min_row > (cs->screen.x
			<= (int)ft_strlen(cs->prompt) || cs->scroll ? 1 : 0))
		{
			scroll_add = cs->cr + cs->min_row - cs->screen.y + 1;
			while (cs->min_row - scroll_add < 0)
				scroll_add--;
			cs->min_row -= scroll_add;
			tputs(tgoto(tgetstr("SF", NULL), 0, scroll_add), 1, &my_putchar);
		}
		if (cs->cr - cs->scroll + cs->min_row + (cs->screen.x <= (int)
			ft_strlen(cs->prompt) || cs->scroll == 0 ? 1 : 0) >= cs->screen.y)
			cs->scroll = cs->cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
	}
}

void	init_char_keys(t_hash_map **map)
{
	char	c[2];

	if (map && *map)
	{
		c[1] = '\0';
		c[0] = HOME_KEY_BIS;
		ft_hash_add(*map, c, &home_key, NULL);
		c[0] = END_KEY_BIS;
		ft_hash_add(*map, c, &end_key, NULL);
		c[0] = BACK_SPACE;
		ft_hash_add(*map, c, &back_space, NULL);
		c[0] = CTRL_D;
		ft_hash_add(*map, c, &ctrl_d, NULL);
	}
}

void	init_input_map(t_hash_map *map)
{
	ft_hash_add(map, ARROW_LEFT, &arrow_left, NULL);
	ft_hash_add(map, ARROW_RIGHT, &arrow_right, NULL);
	ft_hash_add(map, HISTORY_DOWN, &history_down, NULL);
	ft_hash_add(map, LINE_UP, &arrow_up, NULL);
	ft_hash_add(map, LINE_DOWN, &arrow_down, NULL);
	ft_hash_add(map, MV_WORD_RIGHT, &mv_word_right, NULL);
	ft_hash_add(map, MV_WORD_LEFT, &mv_word_left, NULL);
	ft_hash_add(map, HISTORY_UP, &history_up, NULL);
	ft_hash_add(map, HOME_KEY, &home_key, NULL);
	ft_hash_add(map, END_KEY, &end_key, NULL);
	ft_hash_add(map, CLIP_ARROW_RIGHT, &clip_arrow_right, NULL);
	ft_hash_add(map, CLIP_ARROW_LEFT, &clip_arrow_left, NULL);
	ft_hash_add(map, CLIP_ARROW_UP, &clip_arrow_up, NULL);
	ft_hash_add(map, CLIP_ARROW_DOWN, &clip_arrow_down, NULL);
	ft_hash_add(map, REVBACK_SPACE, &revback_space, NULL);
	ft_hash_add(map, PASTE_CLIP, &paste_clip, NULL);
	ft_hash_add(map, COPY_CLIP, &copy_clip, NULL);
	ft_hash_add(map, CUT_CLIP, &cut_clip, NULL);
	init_char_keys(&map);
}

int		check_keys(char *caps)
{
	int				ret;
	t_cs_line		*cs;
	void			(*fct)();

	if (!(cfg_shell()))
		return (-1);
	ret = 0;
	fct = ft_hash_lookup(((t_cfg *)cfg_shell())->input_map, caps);
	if ((cs = cs_master(NULL, 0)) && caps[0] == (char)4)
		ret = ctrl_d(cs);
	else if (fct && (ret = (caps[0] == (char)4 ? -1 : 0)) <= 0)
		fct(cs);
	else if ((ft_strcmp(caps, "\n") == 0 || caps[0] == '\n') && (ret = -1) < 0)
		line_master(cs, caps);
	else if (ft_strcmp(caps, "\033[6n") == 0)
		ret = -1;
	else if (caps[0] != 127 && ret == 0 && (caps[0] != '\033' && caps[0] >= 32)
			&& (ret = 1))
	{
		line_master(cs, caps);
		if (ft_strchr(caps, '\n'))
			ret = -1;
		print_cmdline(cs);
	}
	return (ret);
}
