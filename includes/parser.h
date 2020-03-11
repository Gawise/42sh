#ifndef PARSER_H
# define PARSER_H

# include "struct.h"

int				ft_parser(t_lexer *lexer, t_parser *parser);


// init.c

t_cmd_table			*init_cmd_table(void);
t_and_or			*init_and_or(void);
t_simple_cmd			*init_simple_cmd(void);
t_assignment			*init_assignment(void);
t_redir				*init_redir(void);

// word.c

int				p_cmd_name(t_token *token, t_parser *parser);
int				p_file_name(t_token *token, t_parser *parser);
int				p_add_arg(t_token *token, t_parser *parser);
int				p_add_assign_arg(t_token *token, t_parser *parser);

// redir.c

int				p_create_redir(t_simple_cmd *cmd);
int				p_add_io_num(t_token *token, t_parser *parser);
int				p_add_redir(t_token *token, t_parser *parser);

// assign.c

int				p_add_assign(t_token *token, t_parser *parser);
int				p_assign_val(t_token *token, t_parser *parser);
int				p_assign_join(t_token *token, t_parser *parser);

// and_or.c

int				p_add_and_or(t_token *token, t_parser *parser);

// cmd.c

int				p_add_cmd(t_token *token, t_parser *parser);

// debug/parser.c

void				print_cmd_table(t_list *table);
void				print_parser(t_parser *parser);
int				syn_err(t_token *token, t_parser *parser);

// io_number.c

int				p_add_io_num(t_token *token, t_parser *parser);

// newline.c

int				p_add_table(t_parser *parser);

// amp.c

int				p_add_amp(t_token *token, t_parser *parser);

// args_tab.c

void				p_make_args_tab(t_parser *parser);

// lst_to_tab.c

void				**lst_to_tab(t_list *lst);

// tools.c

int				is_digitstr(char *str);

#endif
