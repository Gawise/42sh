#ifndef PARSER_H
# define PARSER_H

typedef struct			s_word_list
{
	char			*word;
	struct s_word_list	*next;
}				t_word_list;

typedef struct			s_assignment
{
	char			*var;
	char			*val;
	struct s_assignment	*next;
}				t_assignment;

typedef struct			s_redir
{
	int			io_num;
	int 			type; // enum
	char			*file;
	struct s_redir		*next;
}				t_redir;

typedef struct			s_simple_cmd
{
	t_assignment		*assign;
	t_assignment		*curr_assign;
	char			*cmd_name;
	t_word_list		*args;
	t_redir			*redir;
	t_redir			*curr_redir;
	struct s_simple_cmd	*next;
}				t_simple_cmd;

typedef struct			s_and_or
{
	t_simple_cmd		*s_cmd;
	t_simple_cmd		*curr_s_cmd;
	int			bang;
	int			type; //enum
	struct s_and_or		*next;
}				t_and_or;

typedef struct			s_cmd_table
{
	t_and_or		*and_or;
	t_and_or		*curr_and_or;
	int			background;
	struct s_cmd_table	*next;
}				t_cmd_table;

typedef struct			s_parser
{
	int			state;
	int			prev_state;
	t_cmd_table		*table;
	t_cmd_table		*curr_table;
}				t_parser;

int				ft_parser(t_lexer *lexer, t_parser *parser);


// init.c

t_cmd_table			*init_cmd_table(void);
t_and_or			*init_and_or(void);
t_simple_cmd			*init_simple_cmd(void);
t_word_list			*init_word_list(void);
t_redir				*init_redir(void);

// word.c

int				p_cmd_name(t_token *token, t_parser *parser);
int				p_file_name(t_token *token, t_parser *parser);
int				p_add_redir(t_token *token, t_parser *parser);
int				p_add_arg(t_token *token, t_parser *parser);

// bang.c

int				p_add_bang(t_token *token, t_parser *parser);

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
