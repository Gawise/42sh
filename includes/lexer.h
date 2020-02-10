#ifndef LEXER_H
# define LEXER_H

# define L_BUFF_SIZE	256
typedef enum			e_token_type
{
	TOKEN,
	WORD,
	ASSIGNMENT_WORD,
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
	DLESSDASH,		// <<-
	/* Reserved word */
}				t_token_type;

typedef enum			e_char_type
{
	C_INHIBITOR,	// 0
	C_CONTROL,	// 1
	C_REDIR,	// 2
	C_NEWLINE,	// 3
	C_DIGIT,	// 4
	C_EXP,		// 5
	C_BLANK,	// 6
	C_EOI,		// 7
	C_BRACK,	// 8
	C_EQU,		// 9
	C_OTHER		// 10
}				t_char_type;

typedef enum			e_lexer_flag
{
	F_BLANK = 0,
	F_DQUOTE,	// 1
	F_SQUOTE,	// 2
	F_BSLASH,	// 4
	F_HEREDOC,	// 8
	F_PAREXP,	// 16
	F_BRACKEXP,	// 32
	F_HD_DELIM	// 64
}				t_lexer_flag;

typedef enum			e_lexer_state
{
	S_TK_START, // Debut de token	0
	S_HD_BODY, // Body de Heredoc	1
	S_AMP_PIPE, // Token avec & | ;	2
	S_TK_REDIR, // Token avec < >	3
	S_EXP, // Expansion en cours	4
	S_TK_WORD, // Token word	5
	S_IO_NUMBER, // io_number token	6
	S_FLAG // Flag en cours		7
}				t_lexer_state;

typedef struct			s_token
{
	char			*str;
	t_token_type		type;
	size_t			len;
}				t_token;

typedef struct			s_here_queue
{
	t_token			*token;
	char			*delim;
}				t_here_queue;

typedef struct			s_lexer
{
	char			*src;
	char			*curr;
	t_lexer_state		state;
	t_list			*token_lst;
	t_token			*curr_token;
	char			buffer[L_BUFF_SIZE];
	int			buff_i;
	int			flags;
	t_list			*here_queue;
	t_here_queue		*curr_here;
	t_list			*flag_queue;
	t_lexer_flag		*curr_flag;
}				t_lexer;

int	ft_lexer(char *str, t_lexer *lexer);
#endif
