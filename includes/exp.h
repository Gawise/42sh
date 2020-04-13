#ifndef EXP_H
# define EXP_H
# include "struct.h"
# include <pwd.h>

/*
** exp.c
*/

int	word_expansions(t_simple_cmd *cmd);
int	exp_main(char **word, int assign);

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

void	find_tilde_exp_assign(char **word, t_exp exp);
void	find_tilde_exp(char **word, t_exp exp);

#endif
