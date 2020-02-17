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
	struct termios term_origin;      /* origin terminal modes */
	struct termios term_eval;   		/*last exec tmodes */
	uint8_t interactive;		/*True Or False*/
	uint8_t lr;					/* last return */
	pid_t	pid;				/* pid's 21	*/
	t_list	*var;
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
	S_TK_START, // Debut de token		0
	S_HD_BODY, // Body de Heredoc		1
	S_AMP_PIPE, // Token avec & | ;		2
	S_TK_REDIR, // Token avec < >		3
	S_EXP, // Expansion en cours		4
	S_TK_WORD, // Token word		5
	S_IO_NUMBER, // io_number token		6
	S_FLAG // Flag en cours			7
}				t_lexer_state;



/*LEXER*/

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



/*PARSER_H*/

typedef struct			s_assignment
{
	char			*var;
	char			*val;
}				t_assignment;

typedef struct			s_redir
{
	int			io_num;
	t_token_type		type; // enum
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
	int			state;
	int			prev_state;
	t_list			*table;
	t_list			*curr_table;
}				t_parser;



/*SELEC*/

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
    int             cr;
    char            *prompt;
	  char			*clipboard;
    t_dlist         *history;
	  t_point			clipb; //start et end de la selection
}				t_cs_line;







/*EXEC*/

typedef struct	s_var
{
	char		**ctab;
	uint8_t		type;
}				t_var;

typedef struct	s_pipe
{
	int32_t		fd[2];
	uint8_t		tmp;
}				t_pipe;

typedef struct	s_process
{
	char *cmd;                  /* cmd name */
	char **av;                  /* for exec */
	char *path;					/* path's exec */
	pid_t pid;                  /* process ID */
	uint8_t completed;          /* true if process has completed */
	uint8_t stopped;            /* true if process has stopped */
	uint8_t retour;				/* WEXITSTATUS  */
	int status;                 /* reported status value */
	uint8_t std[3];				/* stdin out err*/
	uint8_t setup;
}				t_process;

typedef struct	s_job
{
	char		*command;           /* command line, used for messages */
	t_list		*process;     		/* list of processes in this job */
	t_list		*var;				/* VAR env | locale | tmp */
	pid_t		pgid;               /* process group ID */
	uint8_t		fg;					/* foreground */
	t_pipe		pipe;				/* pipeline */
	char 		notified;           /* true if user told about stopped job */
	uint8_t		std[3];				/* stdin out err*/
	struct		termios tmodes;     /* saved terminal modes */
} 				t_job;

#endif
