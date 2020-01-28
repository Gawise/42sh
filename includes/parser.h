#ifndef PARSER_H
# define PARSER_H

typedef struct			s_assignment
{
	char			*var;
	char			*val;
}				t_assignment;

typedef struct			s_redir
{
	int			io_num;
	int 			type; // enum
	char			*file;
}				t_redir;

typedef struct			s_simple_cmd
{
	char			*cmd_name;
	t_list			*args;
	char			**arguments
	t_list			*redir;
	t_redir			*curr_redir;
	t_list			*assign;
	t_assignment		*curr_assign;
}				t_simple_cmd;

typedef struct			s_and_or
{
	t_list			*s_cmd;
	t_simple_cmd		*curr_s_cmd;
	int			type; //enum
	int			background;
}				t_and_or;

typedef struct			s_cmd_table
{
	t_list			*and_or_lst;
	t_and_or		*curr_and_or;
}				t_cmd_table;

typedef struct			s_parser
{
	int			state;
	int			prev_state;
	t_list			*table;
	t_cmd_table		*curr_table;
}				t_parser;

int				ft_parser(t_lexer *lexer, t_parser *parser);


// init.c

t_list				*init_cmd_table(void);
t_list				*init_and_or(void);
t_list				*init_simple_cmd(void);
t_word_list			*init_word_list(void);
t_list				*init_redir(void);

// word.c

int				p_cmd_name(t_token *token, t_parser *parser);
int				p_file_name(t_token *token, t_parser *parser);
int				p_add_redir(t_token *token, t_parser *parser);
int				p_add_arg(t_token *token, t_parser *parser);

// assign.c

int				p_add_assign(t_token *token, t_parser *parser);
int				p_add_assign_val(t_token *token, t_parser *parser);

// and_or.c

int				p_add_and_or(t_token *token, t_parser *parser);

// cmd.c

int				p_add_cmd(t_token *token, t_parser *parser);

// debug/parser.c

void				print_cmd_table(t_cmd_table *table);
void				print_parser(t_parser *parser);

// io_number.c

int				p_add_io_num(t_token *token, t_parser *parser);

// newline.c

int				p_add_table(t_token *token, t_parser *parser);

// amp.c

int				p_add_amp(t_token *token, t_parser *parser);

#endif
