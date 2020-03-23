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

int		lexer_routine(char **line, t_lexer *lexer)
{
	set_signal_ign();
	init_lexer(lexer);
	if (!ft_lexer(line, lexer))
	{
		ft_strdel(line);
		return (0);
	}
	if (cfg_shell()->debug)
		ft_lstiter(lexer->token_lst, print_debug);
	ft_strdel(line);
	return (1);
}

int		parser_routine(t_lexer *lexer,t_parser *parser)
{
	if (!ft_parser(lexer, parser)
	|| (parser->state == S_PARSER_TABLE_START
	&& !parser->table))
	{
		ft_lstdel(&parser->table, del_cmd_table);
		ft_lstdel(&lexer->token_lst, del_token);
		return (0);
	}
	ft_lstdel(&lexer->token_lst, del_token);
	return (1);
}

int		main(int ac, char **av, char **env)
{
	ssize_t		ret;
	char		*line;
	t_lexer		lexer;
	t_parser	parser;
	uint8_t		debug;

	debug = init_shell(env, av);
	(void)ac;
	ret = 0;
	while (1)  //recup PS1
	{
		if (!(line = ft_prompt(NAME_SH, COLOR_SH)))
			continue ;
		else if (line && (!line[0] || ft_strcmp("exit\n", line) == 0))
			break ;
		if (!lexer_routine(&line, &lexer)
		|| !parser_routine(&lexer, &parser))
			continue ;
		if (parser.state != S_PARSER_SYNTAX_ERROR)
		{
			if (debug)
				print_parser(&parser);
			ft_eval(parser.table);
		}
		ft_lstdel(&parser.table, del_cmd_table);
		ft_strdel(&line);
	}
	ft_putendl("\e[0;31m exit\e[0;0m");
	exit(0);
}
