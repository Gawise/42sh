#ifndef STRUCT_H
# define STRUCT_H

# include <stdint.h>
# include <sys/types.h>
# include <termios.h>

# define L_BUFF_SIZE	256


typedef struct s_list	t_list;
typedef struct s_dlist	t_dlist;


typedef struct	s_cfg
{
	struct termios *term_origin;      /* origin terminal modes */
	uint8_t interactive;		/*True Or False*/
	pid_t	pid;				/* pid's 21	*/
	t_list	*env;
	t_list	*intern;
	t_list	*job;
	uint8_t		debug;		/* set for print debug */
}				t_cfg;


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
	LESS,			// < 10
	DLESS,			// <<
	GREAT,			// > 12
	DGREAT,			// >>
	LESSAND,		// <&
	GREATAND,		// >&
	DLESSDASH,		// <<-
}				t_token_type;

typedef enum			e_char_type
{
	C_INHIBITOR,	// 0
	C_CONTROL,		// 1
	C_REDIR,		// 2
	C_NEWLINE,		// 3
	C_DIGIT,		// 4
	C_EXP,			// 5
	C_BLANK,		// 6
	C_EOI,			// 7
	C_BRACK,		// 8
	C_EQU,			// 9
	C_OTHER			// 10
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

/*
** LEXER
*/

typedef struct			s_token
{
	char			*str;
	t_token_type		type;
	size_t			len;
}				t_token;

typedef enum			e_lexer_state
{
	S_TK_START,		// Debut de token		0
	S_HD_BODY,		// Body de Heredoc		1
	S_AMP_PIPE,		// Token avec & | ;		2
	S_TK_REDIR,		// Token avec < >		3
	S_EXP,			// Expansion en cours	4
	S_TK_WORD,		// Token word			5
	S_IO_NUMBER,	// io_number token		6
	S_FLAG			// Flag en cours		7
}				t_lexer_state;

typedef struct			s_here_queue
{
	t_token			*token;
	char			*delim;
}				t_here_queue;

typedef struct			s_lexer
{
	char			**src;
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

/*
** PARSER
*/

typedef enum			e_parser_state
{
	S_PARSER_SYNTAX_ERROR,	// Syntax error rencontree
	S_PARSER_TABLE_START,	// Debut de cmd_table
	S_PARSER_CMD_START,	// En attente de cmd_name
	S_PARSER_CMD_ARGS,	// En attente d'args
	S_PARSER_REDIR,		// Redirection en attente de filename
	S_PARSER_ASSIGN,	// Assignation en attente de valeur
	S_PARSER_IO_NUMBER,	// Io_number en attente de redirection
	S_PARSER_DELIM,		// Redirection DLESS en attente de delim
	S_PARSER_ARG_ASSIGN,	// Completion d'arg apres ASSIGNMENT_WORD
	S_PARSER_ANDIF_PIPE	// Debut de pipe ou and_or
}				t_parser_state;

typedef struct			s_assignment
{
	char			*var;
	char			*val;
}				t_assignment;

typedef struct			s_redir
{
	char			*delim;
	char			*io_num;
	t_token_type	type; // enum
	char			*file;
}				t_redir;

typedef struct			s_simple_cmd
{
	char			*cmd_name;
	t_list			*args;
	char			**av;
	t_list			*redir;
	t_list			*curr_redir;
	t_list			*assign;
	t_list			*curr_assign;
}				t_simple_cmd;

typedef struct			s_and_or
{
	char			*str;
	t_list			*s_cmd;
	t_list			*curr_s_cmd;
	t_token_type		type; //enum
	int			background;
}				t_and_or;

typedef struct			s_cmd_table
{
	t_list			*and_or;
	t_list			*curr_and_or;
}				t_cmd_table;

typedef struct			s_parser
{
	int				state;
	int				prev_state;
	t_token_type	pmt_prefix;
	t_list			*table;
	t_list			*curr_table;
}					t_parser;

/*
** SELECT
*/

typedef struct	s_point
{
	int x;
	int y;
}				t_point;

typedef struct	s_line_lst
{
	struct s_line_lst	*prev;
	struct s_line_lst	*next;
	char				*str;
	int					len;
	int					nb_lines;
}				t_line_lst;

typedef struct	s_cs_line
{
	int             line_col;
    int             col;
    int             row;
    int             min_col;
    int             min_row;
    int             scroll;
    int             max_scroll;
    int             tty;
    t_point         screen;
    char            *input;
    int             sig_int;
	int				sig_eof;
    int             cr;
    char            *prompt;
	char			*prompt_color;
	char			*clipboard;
	t_dlist         *history;
	  t_point			clipb; //start et end de la selection
}				t_cs_line;

/*
** EXEC
*/

typedef struct	s_var
{
	char		**ctab;
}				t_var;

typedef struct	s_pipe
{
	int32_t		fd[2];
	uint8_t		tmp;
}				t_pipe;

typedef struct	s_process
{
	char 		*cmd;                /* cmd name */
	char 		**av;                /* for exec */
	char 		*path;				/* path's exec */
	pid_t 		pid;                /* process ID */
	uint8_t 	ret;				/* WEXITSTATUS  */
	uint8_t 	status;             /* reported status value */
	int8_t 		std[3];				/* stdin out err*/
	uint32_t	setup;				/* info du process */
	t_list		*redir;				/* list of redirs */
	t_list		*env;				/* VAR env  */
}				t_process;

typedef struct	s_job
{
	char		*cmd;		        /* command line, used for messages */
	t_list		*process;     		/* list of processes in this job */
	pid_t		pgid;               /* process group ID */
	uint8_t		fg;					/* foreground */
	t_pipe		pipe;				/* pipeline */
	uint8_t 	status;          	/* reported status value */
	uint8_t		ret;				/* retour last process */
	uint8_t		std[3];				/* stdin out err*/
	struct		termios term_eval;     /* saved terminal modes */
} 				t_job;

#endif
