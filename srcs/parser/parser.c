#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"
#include "var.h"
#include "line_edition.h"
#include "sh.h"

int		syn_err(t_token *token, t_parser *parser)
{
	ft_printf("syntax error near unexpected token `%s\' state=%d\n", token->str, parser->state);
	parser->state = S_PARSER_SYNTAX_ERROR;
	return (1);
}

int		p_skip(t_token *token, t_parser *parser)
{
	(void)token;
	(void)parser;
	return (1);
}

int		p_set_start_state(t_token* token, t_parser *parser)
{
	(void)token;
	parser->state = S_PARSER_TABLE_START;
	return (1);
}

int		p_add_redir_delim(t_token* token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;
	t_redir		*redir;

	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	redir = (t_redir *)cmd->curr_redir->data;
	if (!(redir->delim = ft_strdup(token->str)))
		return (0);
	parser->state = S_PARSER_REDIR;
	return (1);
}

void		init_parser(t_parser *parser)
{
	parser->state = S_PARSER_TABLE_START;
	parser->prev_state = S_PARSER_TABLE_START;
	parser->table = NULL;
	parser->curr_table = NULL;
}

int	p_process_token(t_token *token, t_parser *parser, int (*table_builder[10][17])(t_token *, t_parser *))
{
	if (!table_builder[parser->state][token->type](token, parser))
		return (0);
	return (1);
}

int	p_tokeniter(t_list *token, t_parser *parser, int (*table_builder[10][17])(t_token *, t_parser *))
{
	while (token)
	{
		if (!p_process_token(token->data, parser, table_builder))
			return (0);
		token = token->next;
	}
	return (1);
}

void	p_init_state_machine(int (*table_builder[10][17])(t_token *, t_parser *))
{
	p_init_start_state(table_builder);
	p_init_cmd_wait_state(table_builder);
	p_init_args_wait_state(table_builder);
	p_init_redir_state(table_builder);
	p_init_assign_state(table_builder);
	p_init_io_nbr_state(table_builder);
	p_init_delim_state(table_builder);
	p_init_assign_state(table_builder);
	p_init_arg_assign_state(table_builder);
	p_init_andif_pipe_state(table_builder);
	p_init_syn_err_state(table_builder);
}









char	*p_get_prompt_prefix(t_parser *parser)
{
	if (parser->pmt_prefix == AND_IF)
		return (ft_strdup("cmdand"));
	else if (parser->pmt_prefix == OR_IF)
		return (ft_strdup("cmdor"));
	else if (parser->pmt_prefix == PIPE)
		return (ft_strdup("pipe"));
	return (ft_strdup(""));
}

int		ft_parser(t_lexer *lexer, t_parser *parser)
{
	int	(*table_builder[10][17])(t_token *, t_parser *);
	char	*pmt;
	char	*pmt_prefix;

	if (!lexer || !parser)
		return (0);
	p_init_state_machine(table_builder);
	p_tokeniter(lexer->token_lst, parser, table_builder);
	while (parser->state == S_PARSER_ANDIF_PIPE)
	{
		ft_lstdel(&lexer->token_lst, del_token);
		if (!(pmt_prefix = p_get_prompt_prefix(parser))
		|| ft_printf("%s", pmt_prefix) == -1
		|| !(pmt = ft_prompt(find_var_value(cfg_shell()->intern, "PS2")
		, COLOR_SUBPROMPT)))
			return (0);
		ft_lexer(&pmt, lexer);
		ft_strdel(&pmt);
		ft_strdel(&pmt_prefix);
		p_tokeniter(lexer->token_lst, parser, table_builder);
	}
	p_make_args_tab(parser);
	return (1);
}
