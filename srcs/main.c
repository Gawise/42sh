#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "lexer.h"
#include "parser.h"
#include "line_edition.h"
#include "exec.h"
#include "sh.h"

void	print_debug(t_list *elem);

void	init_lexer(t_lexer *lexer)
{
	lexer->src = NULL;
	lexer->curr = NULL;
	lexer->state = S_TK_START;
	lexer->token_lst = NULL;
	lexer->curr_token = NULL;
	ft_bzero(lexer->buffer, L_BUFF_SIZE);
	lexer->buff_i = 0;
	lexer->flags = 0;
	lexer->here_queue = NULL;
	lexer->curr_here = NULL;
	lexer->flag_queue = NULL;
	lexer->curr_flag = NULL;
}

int		ft_lexer(char *str, t_lexer *lexer);

int		main(int ac, char **av, char **env)
{
	ssize_t		ret;
	char		*line;
	t_lexer		*lexer;
	t_parser	parser;
	uint8_t		debug;

	debug = init_shell(env, av);
	(void)ac;
	lexer = (t_lexer *)ft_memalloc(sizeof(t_lexer));
	ret = 0;
	while (1)  //recup PS1
	{
		if (!(line = ft_prompt(NAME_SH)))
			continue ;
		else if (line && (!line[0] || ft_strcmp("exit\n", line) == 0))
			break ;
		set_signal_ign();
		init_lexer(lexer);
		ft_lexer(line, lexer);
		if (debug)
			ft_lstiter(lexer->token_lst, print_debug);
		ft_parser(lexer, &parser);
		if (parser.state != S_PARSER_SYNTAX_ERROR)
		{
			if (debug)
				print_parser(&parser);
			ft_eval(parser.table);
		}
	}
	ft_putendl("exit");
	exit(0);
}

