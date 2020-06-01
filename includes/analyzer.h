#ifndef ANALYZER_H
# define ANALYZER_H

# include "struct.h"
# include <pwd.h>


/*
** main.c
*/

int		analyzer_routine(t_cmd_table *cmd);

/*
** jobs_str/redir.c
*/

char		*get_redir_str(t_list *redir_lst);
char		*get_args_str(char **args);

/*
** jobs_str/core.c
*/

int		a_set_jobs_str(t_cmd_table *cmd);

/*
** heredoc.c
*/

void		a_remove_leading_tabs(t_cmd_table *cmd);

/*
** make_args_tab.c 
*/

int		a_make_args_tab(t_cmd_table *cmd);

/*
** args_to_tab.c
*/

void		**args_to_tab(t_list *lst, char *cmd);

/*
** exp.c
*/

int	word_expansions(t_simple_cmd *cmd);
int	exp_main(char **word, int assign);

/*
** exp_getenv.c
*/

char	*exp_getenv(char *str, int type, int hash);

/*
** exp_tools.c
*/

void	exp_substitute(t_exp *exp, char *str);
void	init_exp(t_exp *exp);
void	free_exp_content(t_exp *exp);
void	exp_flush_buf(t_exp *exp, char **str);
void	exp_add_to_buf(t_exp *exp, char **str, char **res);

/*
** parameter.c
*/

int	parse_param_exp(char **word, t_exp exp);

/*
** parameter_recursive.c
*/

int	rec_param_exp(t_exp *exp, char **str);

/*
** param_resolve.c
*/

char	*resolve_parameter(char *str, int hash);
int	resolve_colon_param(char **str, t_exp *exp, char *param);
int	resolve_brace_param(char **str, t_exp *exp, char *param);
int	resolve_pattern_param(char **str, t_exp *exp, char *param);

/*
** param_substitution.c
*/

int	substitute_parameter(t_exp *exp, char **str);
int	substitute_word(t_exp *exp, char **str);
int	assign_word(t_exp *exp, char **str, char *param);
int	substitute_null(t_exp *exp, char **str);
void	substitute_pattern(t_exp *exp, int type);

/*
** word_parameter.c
*/

int	rec_word_parse(t_exp *exp, char **str);
int	skip_word(t_exp *exp, char **str);

/*
** tilde_exp.c
*/

void	find_tilde_exp(char **word, t_exp exp);
int		parse_tilde_exp(char **string, t_exp *exp, int assign);

/*
** tilde_assign.c
*/

void	find_tilde_exp_assign(char **word, t_exp exp);

/*
** quote_removal.c
*/

char	*a_quote_removal(char **str);

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
