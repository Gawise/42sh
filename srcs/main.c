#include "libft.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include "lexer.h"
#include "parser.h"
#include "job_control.h"
#include "analyzer.h"
#include "exec.h"
#include "sh.h"
#include "var.h"
#include "line_edition.h"
#include <unistd.h>
#include <stdio.h>


void	print_debug(t_list *elem);

void	ft_ex(char *error)
{
	ft_dprintf(STDERR_FILENO,"%s", error);
	exit(EXIT_FAILURE);
}

int		lexer_routine(char **line, t_lexer *lexer)
{
	set_signal_ign();
	ft_bzero(lexer, sizeof(t_lexer));
	if (!ft_lexer(line, lexer))
	{
		ft_strdel(line);
		ft_lstdel(&lexer->token_lst, del_token);
		ft_lstdel(&lexer->here_queue, del_here_queue);
		return (0);
	}
	if (cfg_shell()->debug)
		ft_lstiter(lexer->token_lst, print_debug);
	ft_strdel(line);
	return (1);
}

int		parser_routine(t_lexer *lexer,t_parser *parser)
{
	check_child(cfg_shell(), cfg_shell()->job);
	if (cfg_shell()->debug)
		ft_dprintf(cfg_shell()->debug, "\n----------- parsing -----------\n\n");
	init_parser(parser);
	if (!ft_parser(lexer, parser)
	|| (parser->state == S_PARSER_TABLE_START
	&& !parser->table))
	{
		ft_lstdel(&parser->table, del_cmd_table);
		ft_lstdel(&lexer->token_lst, del_token);
		return (0);
	}
	ft_lstdel(&lexer->token_lst, del_token);
	if (cfg_shell()->debug)
		print_parser(parser);
	return (1);
}

int		line_edition_routine(char **line)
{
	if (!(*line = ft_prompt(find_var_value(cfg_shell()->intern, "PS1")
	, COLOR_SH)))
		return (0);
	else if (*line && (!line[0][0]))
		return (-1);
	return (1);
}

int		eval_routine(t_parser *parser)
{
	if (parser->state != S_PARSER_SYNTAX_ERROR)
		ft_eval(parser->table);
	ft_lstdel(&parser->table, del_cmd_table);
	return (1);
}

int		analyzer_routine(t_parser *parser)
{
	if (a_make_args_tab(parser) < 0
	|| (parser->state != S_PARSER_SYNTAX_ERROR
	&& !a_set_jobs_str(parser)))
	{
		ft_lstdel(&parser->table, del_cmd_table);
		return (0);
	}
	a_remove_leading_tabs(parser);
	return (1);
}

int		main(int ac, char **av, char **env)
{
	int		ret;
	char		*line;
	t_lexer		lexer;
	t_parser	parser;

	init_shell(env, av, ac);
	while (1)
	{
		if ((ret = line_edition_routine(&line)) <= 0
		|| (ret = lexer_routine(&line, &lexer)) <= 0
		|| (ret = parser_routine(&lexer, &parser)) <= 0
		|| (ret = analyzer_routine(&parser)) <= 0
		|| (ret = eval_routine(&parser)) <= 0)
		{
			if (ret == -1)
			{
				ft_dprintf(2, "\e[0;31m exit\e[0;0m\n");
				break ;
			}
		}
	}
	clean_cfg(cfg_shell());
	exit(0);
}
