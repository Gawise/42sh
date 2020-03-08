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
void			set_term(int tty, int init, char *prompt,
							struct termios *new_term);
void			size_handler(int sig);
void			sig_handler(int sig);
void			putchar_n(char c, int n);
int				term_check(struct termios *new_term, struct termios *old_term,
							int tty);
void			init_signals(void);
void			print_cmdline(t_cs_line *cs);
void			line_master(t_cs_line *cs, char *input);
void			home_key(t_cs_line *cs);
void			end_key(t_cs_line *cs);
void			maj_arrow_down(t_cs_line *cs);
void			maj_arrow_up(t_cs_line *cs);
int				get_line(t_cs_line *cs);
char			*ft_prompt(char *prompt);
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

#endif
