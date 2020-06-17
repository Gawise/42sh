#include "libft.h"
#include "ft_printf.h"
#include "sh.h"
#include "exec.h"
#include "var.h"
#include "lexer.h"
#include "parser.h"
#include "analyzer.h"
#include "line_edition.h"
#include "job_control.h"

int		lexer_routine(char **line, t_lexer *lexer)
{
	set_signal_ign();
	ft_bzero(lexer, sizeof(t_lexer));
	if (!ft_lexer(line, lexer))
	{
		ft_strdel(line);
		ft_lstdel(&lexer->token_lst, del_token);
		ft_lstdel(&lexer->here_queue, del_here_queue);
		ft_lstdel(&lexer->flag_queue, del_flag_queue);
		return (0);
	}
	if (cfg_shell()->debug)
		ft_lstiter(lexer->token_lst, print_debug);
	ft_strdel(line);
	return (1);
}

int		parser_routine(t_lexer *lexer, t_parser *parser)
{
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
	if (parser->state != S_PARSER_SYNTAX_ERROR
	&& ft_eval(parser->table))
	{
		ft_lstdel(&parser->table, del_cmd_table);
		return (0);
	}
	ft_lstdel(&parser->table, del_cmd_table);
	protect_job(1);
	return (1);
}

int		analyzer_routine(t_and_or *and_or)
{
	if (a_make_args_tab(and_or) < 0)
		return (0);
	a_set_jobs_str(and_or);
	a_remove_leading_tabs(and_or);
	return (1);
}
