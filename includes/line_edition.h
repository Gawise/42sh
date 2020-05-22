/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edition.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 20:47:25 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/08 16:56:10 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <sys/ioctl.h>
# include <fcntl.h>
# include <termios.h>
# include <term.h>
# include "struct.h"
# define PROMPT_SIZE 15
# include "libft.h"

# define READ_SIZE 8
# define ARROW_LEFT "\e[D"
# define ARROW_RIGHT "\e[C"
# define LINE_DOWN "\e[1;2B"
# define LINE_UP "\e[1;2A"
# define HISTORY_UP "\e[A"
# define HISTORY_DOWN "\e[B"
# define MV_WORD_RIGHT "\e[1;5C"//"\e[1;2C"
# define MV_WORD_LEFT "\e[1;5D"//"\e[1;2D"
# define HOME_KEY "\e[H"
# define END_KEY "\e[F"
# define CLIP_ARROW_RIGHT "\e[1;4C"
# define CLIP_ARROW_LEFT "\e[1;4D"
# define CLIP_ARROW_UP "\e[1;4A"
# define CLIP_ARROW_DOWN "\e[1;4B"
# define REVBACK_SPACE "\e[3~"
# define HOME_KEY_BIS (char)1
# define END_KEY_BIS (char)5
# define COPY_CLIP "\ec"//(char)11
# define CUT_CLIP "\ek"//(char)12
# define PASTE_CLIP "\ev"//(char)16
# define BACK_SPACE (char)127
# define CTRL_D (char)4

t_cs_line		*cs_master(char *prompt, int init);
void			space_bar(t_cs_line *cs);
int				check_keys(char *caps);
void			arrow_up(t_cs_line *cs);
void			arrow_down(t_cs_line *cs);
void			arrow_right(t_cs_line *cs);
void			arrow_left(t_cs_line *cs);
int				my_putchar(int c);
void			get_cs_line_position(int *col, int *rows);
void			read_input(void);
void			cs_set(void);
void			ft_clear(int del_prompt);
void			move_cs(t_cs_line **cs);
int				term_init(int init, char *prompt);
void			set_term(int tty, char *prompt, struct termios *new_term);
void			size_handler(int sig);
void			sig_handler(int sig);
void			putchar_n(char c, int n);
int				term_check(struct termios *new_term, int tty);
void			init_signals(void);
void			print_cmdline(t_cs_line *cs);
void			line_master(t_cs_line *cs, char *input);
void			home_key(t_cs_line *cs);
void			end_key(t_cs_line *cs);
void			maj_arrow_down(t_cs_line *cs);
void			maj_arrow_up(t_cs_line *cs);
int				get_line(t_cs_line *cs);
char			*ft_prompt(char *prompt, char *color);
int				get_col(t_cs_line *cs);
t_point			cs_pos(t_cs_line *cs);
void			ft_utoa(char **str);
void			join_input(t_cs_line *cs, char *input);
void			print_prompt(t_cs_line *cs);
void			history_up(t_cs_line *cs);
void			history_down(t_cs_line *cs);
t_dlist			*init_history(void);
void			mv_word_left(t_cs_line *cs);
void			mv_word_right(t_cs_line *cs);
void			update_history(t_dlist *hs);
t_dlist			*get_history(void);
void			clip_arrow_right(t_cs_line *cs);
void			clip_arrow_left(t_cs_line *cs);
void			clip_arrow_up(t_cs_line *cs);
void			set_scroll(t_cs_line *cs);
void			cmdline_printer(t_cs_line *cs, t_point start, t_point end);
void			clip_arrow_down(t_cs_line *cs);
void			copy_clip(t_cs_line *cs);
void			paste_clip(t_cs_line *cs);
void			cut_clip(t_cs_line *cs);
int				revback_space(t_cs_line *cs);
t_point			trim_input(t_cs_line *cs);
int				ctrl_d(t_cs_line *cs);
int				back_space(t_cs_line *cs);
void			init_char_keys(t_hash_map **map);
void			init_input_map(t_hash_map *map);
int				ft_strcheck(char *s, char *oc);

#endif
