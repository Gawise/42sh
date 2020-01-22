#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
typedef enum			e_type
{
	TOKEN,
	WORD,
	ASSIGNMENT_WORD,
	//NAME,			// uniquement pour case
	NEWLINE,
	IO_NUMBER,
	/* Control Operators */
	AND_IF,		 	// &&
	AMP,			// &
	OR_IF,			// ||
	PIPE,			// |
	SEMI,			// ;
	/* Redirection Operators */
	LESS,			// <
	DLESS,			// <<
	GREAT,			// >
	DGREAT,			// >>
	LESSAND,		// <&
	GREATAND,		// >&
	LESSGREAT,		// <>
	DLESSDASH,		// <<-
	CLOBBER,		// >|
	/* Reserved word */
	LBRACE,			// {
	RBRACE,			// }
	BANG,			// !
}				t_type;

typedef struct			s_token
{
	char			*str;
	t_type			type;
	size_t			len;
	struct s_token		*next;
}				t_token;

typedef struct			s_lexer
{
	t_token			*token_lst;
	t_token			*curr_token;
}				t_lexer;

void		get_ctrl_op_token(char *str, t_token *token, int *i);
void		get_redir_op_token(char *str, t_token *token, int *i);
void		get_ionumber_token(char *str, t_token *token, int *i);
void		get_reserved_word(char *str, t_token *token, int *i);
void		get_assignment_word(char *str, t_token *token, int *i);
void		get_word_token(char *str, t_token *token, int *i);
void		handle_quoting(t_token *token, char *str, int *i);
void		set_token(t_token *token, t_type type, char *str, size_t len);
void		handle_param_expansion(char *str, int *i);
int		is_separator(char c);
int		is_reserved(char c);
int		is_operator(char c);
int		ft_lexer(char *str, t_lexer *lexer);

#endif
