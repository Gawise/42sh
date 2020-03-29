#ifndef ANALYZER_H
# define ANALYZER_H

# include "struct.h"

/*
** jobs_str/redir.c
*/

char		*get_redir_str(t_list *redir_lst);
char		*get_args_str(char **args);

/*
** jobs_str/core.c
*/

int		p_set_jobs_str(t_parser *parser);

/*
** heredoc.c
*/

void		a_remove_leading_tabs(t_parser *parser);

/*
** 
*/

/*
** 
*/

/*
** 
*/

/*
** 
*/

/*
** 
*/

/*
** 
*/

/*
** 
*/

/*
** 
*/

#endif
