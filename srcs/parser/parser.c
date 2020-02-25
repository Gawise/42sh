#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"
#include "line_edition.h"

int		syn_err(t_token *token, t_parser *parser)
{
	ft_printf("syntax error: %4d %s\n", parser->state, token->str);
	return (1);
}

int		p_skip(t_token *token, t_parser *parser)
{
	(void)token;
	(void)parser;
	return (1);
}

/*
**	Parser states :
**	0: Debut de parsing
**	1: en attente d'un cmd_name
**	2: cmd_name renseigne
**	3: redirection en attente du filename
**	4: assignation en attente de valeur
**	5: io_number en attente de redirection
*/
/*
int	(*g_table_builder[17][7])(t_token *, t_parser *) = {
	{ syn_err,	syn_err,	syn_err,	syn_err,	syn_err,	syn_err }, // TOKEN
	{ p_cmd_name,	p_cmd_name,	p_add_arg,	p_file_name,	p_assign_val,	syn_err }, // WORD
	{ p_add_assign,	p_add_assign,	p_add_arg,	p_file_name,	syn_err,	syn_err }, // ASSIGNMENT
	{ p_skip,	p_set_start_state,	p_set_start_state,	syn_err,	p_set_start_state,	syn_err }, // NEWLINE
	{ p_add_io_num,	p_add_io_num,	p_add_io_num,	syn_err,	syn_err,	syn_err }, // IO_NUMBER
	{ syn_err,	p_add_and_or,	p_add_and_or,	syn_err,	p_add_and_or,	syn_err }, // AND_IF
	{ syn_err,	p_add_amp,	p_add_amp,	syn_err,	p_add_amp,	syn_err }, // AMP
	{ syn_err,	p_add_and_or,	p_add_and_or,	syn_err,	p_add_and_or,	syn_err }, // OR_IF
	{ syn_err,	p_add_cmd,	p_add_cmd,	syn_err,	p_add_cmd,	syn_err }, // PIPE
	{ syn_err,	p_set_start_state,	p_set_start_state,	syn_err,	p_set_start_state,	syn_err }, // SEMI
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// LESS
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// DLESS
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// GREAT
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// DGREAT
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// LESSAND
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// GREATAND
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// LESSGREAT
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// DLESSDASH
	{ p_add_redir,	p_add_redir,	p_add_redir, 	syn_err,	syn_err,	p_add_redir },// CLOBBER
//	{ syn_err,	syn_err,	syn_err,	syn_err,	syn_err,	syn_err }, // LBRACE
//	{ syn_err,	syn_err,	syn_err,	syn_err,	syn_err,	syn_err }, // RBRACE
//	{ p_add_bang,	syn_err,	syn_err,	syn_err,	syn_err,	syn_err } // BANG
};
*/


/*

t_token		*lookahead(t_lexer *lexer)
{
	return (lexer->curr_token);
}

t_token		*eat_token(t_lexer *lexer)
{
	t_token		*token;

	token = lexer->curr_token;
	if (lexer->curr_token)
		lexer->curr_token = lexer->curr_token->next;
	return (token);
}

*/

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

void		p_init_start_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_TABLE_START][TOKEN] = syn_err;
	table_builder[S_PARSER_TABLE_START][WORD] = p_cmd_name;
	table_builder[S_PARSER_TABLE_START][ASSIGNMENT_WORD] = p_add_assign;
	table_builder[S_PARSER_TABLE_START][NEWLINE] = syn_err;
	table_builder[S_PARSER_TABLE_START][IO_NUMBER] = p_add_io_num;
	table_builder[S_PARSER_TABLE_START][AND_IF] = p_add_and_or;
	table_builder[S_PARSER_TABLE_START][AMP] = syn_err;
	table_builder[S_PARSER_TABLE_START][OR_IF] = p_add_and_or;
	table_builder[S_PARSER_TABLE_START][PIPE] = syn_err;
	table_builder[S_PARSER_TABLE_START][SEMI] = p_set_start_state;
	table_builder[S_PARSER_TABLE_START][LESS] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][DLESS] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][GREAT] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][DGREAT] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][LESSAND] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][GREATAND] = p_add_redir;
	table_builder[S_PARSER_TABLE_START][DLESSDASH] = p_add_redir;
}

void		p_init_cmd_wait_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_CMD_START][TOKEN] = syn_err;
	table_builder[S_PARSER_CMD_START][WORD] = p_cmd_name;
	table_builder[S_PARSER_CMD_START][ASSIGNMENT_WORD] = p_add_assign;
	table_builder[S_PARSER_CMD_START][NEWLINE] = p_skip;
	table_builder[S_PARSER_CMD_START][IO_NUMBER] = p_add_io_num;
	table_builder[S_PARSER_CMD_START][AND_IF] = p_add_and_or;
	table_builder[S_PARSER_CMD_START][AMP] = p_add_amp;
	table_builder[S_PARSER_CMD_START][OR_IF] = p_add_and_or;
	table_builder[S_PARSER_CMD_START][PIPE] = p_add_cmd;
	table_builder[S_PARSER_CMD_START][SEMI] = p_set_start_state;
	table_builder[S_PARSER_CMD_START][LESS] = p_add_redir;
	table_builder[S_PARSER_CMD_START][DLESS] = p_add_redir;
	table_builder[S_PARSER_CMD_START][GREAT] = p_add_redir;
	table_builder[S_PARSER_CMD_START][DGREAT] = p_add_redir;
	table_builder[S_PARSER_CMD_START][LESSAND] = p_add_redir;
	table_builder[S_PARSER_CMD_START][GREATAND] = p_add_redir;
	table_builder[S_PARSER_CMD_START][DLESSDASH] = p_add_redir;
}

void		p_init_args_wait_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_CMD_ARGS][TOKEN] = syn_err;
	table_builder[S_PARSER_CMD_ARGS][WORD] = p_add_arg;
	table_builder[S_PARSER_CMD_ARGS][ASSIGNMENT_WORD] = p_add_arg;
	table_builder[S_PARSER_CMD_ARGS][NEWLINE] = p_set_start_state;
	table_builder[S_PARSER_CMD_ARGS][IO_NUMBER] = p_add_io_num;
	table_builder[S_PARSER_CMD_ARGS][AND_IF] = p_add_and_or;
	table_builder[S_PARSER_CMD_ARGS][AMP] = p_add_amp;
	table_builder[S_PARSER_CMD_ARGS][OR_IF] = p_add_and_or;
	table_builder[S_PARSER_CMD_ARGS][PIPE] = p_add_cmd;
	table_builder[S_PARSER_CMD_ARGS][SEMI] = p_set_start_state;
	table_builder[S_PARSER_CMD_ARGS][LESS] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][DLESS] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][GREAT] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][DGREAT] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][LESSAND] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][GREATAND] = p_add_redir;
	table_builder[S_PARSER_CMD_ARGS][DLESSDASH] = p_add_redir;
}

void		p_init_redir_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_REDIR][TOKEN] = syn_err;
	table_builder[S_PARSER_REDIR][WORD] = p_file_name;
	table_builder[S_PARSER_REDIR][ASSIGNMENT_WORD] = p_file_name;
	table_builder[S_PARSER_REDIR][NEWLINE] = syn_err;
	table_builder[S_PARSER_REDIR][IO_NUMBER] = p_file_name;
	table_builder[S_PARSER_REDIR][AND_IF] = syn_err;
	table_builder[S_PARSER_REDIR][AMP] = syn_err;
	table_builder[S_PARSER_REDIR][OR_IF] = syn_err;
	table_builder[S_PARSER_REDIR][PIPE] = syn_err;
	table_builder[S_PARSER_REDIR][SEMI] = syn_err;
	table_builder[S_PARSER_REDIR][LESS] = syn_err;
	table_builder[S_PARSER_REDIR][DLESS] = syn_err;
	table_builder[S_PARSER_REDIR][GREAT] = syn_err;
	table_builder[S_PARSER_REDIR][DGREAT] = syn_err;
	table_builder[S_PARSER_REDIR][LESSAND] = syn_err;
	table_builder[S_PARSER_REDIR][GREATAND] = syn_err;
	table_builder[S_PARSER_REDIR][DLESSDASH] = syn_err;
}

void		p_init_assign_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_ASSIGN][TOKEN] = syn_err;
	table_builder[S_PARSER_ASSIGN][WORD] = p_assign_val;
	table_builder[S_PARSER_ASSIGN][ASSIGNMENT_WORD] = syn_err;
	table_builder[S_PARSER_ASSIGN][NEWLINE] = p_set_start_state;
	table_builder[S_PARSER_ASSIGN][IO_NUMBER] = syn_err;
	table_builder[S_PARSER_ASSIGN][AND_IF] = p_add_and_or;
	table_builder[S_PARSER_ASSIGN][AMP] = p_add_amp;
	table_builder[S_PARSER_ASSIGN][OR_IF] = p_add_and_or;
	table_builder[S_PARSER_ASSIGN][PIPE] = p_add_cmd;
	table_builder[S_PARSER_ASSIGN][SEMI] = p_set_start_state;
	table_builder[S_PARSER_ASSIGN][LESS] = syn_err;
	table_builder[S_PARSER_ASSIGN][DLESS] = syn_err;
	table_builder[S_PARSER_ASSIGN][GREAT] = syn_err;
	table_builder[S_PARSER_ASSIGN][DGREAT] = syn_err;
	table_builder[S_PARSER_ASSIGN][LESSAND] = syn_err;
	table_builder[S_PARSER_ASSIGN][GREATAND] = syn_err;
	table_builder[S_PARSER_ASSIGN][DLESSDASH] = syn_err;
}

void		p_init_io_nbr_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_IO_NUMBER][TOKEN] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][WORD] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][ASSIGNMENT_WORD] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][NEWLINE] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][IO_NUMBER] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][AND_IF] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][AMP] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][OR_IF] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][PIPE] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][SEMI] = syn_err;
	table_builder[S_PARSER_IO_NUMBER][LESS] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][DLESS] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][GREAT] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][DGREAT] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][LESSAND] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][GREATAND] = p_add_redir;
	table_builder[S_PARSER_IO_NUMBER][DLESSDASH] = p_add_redir;
}

void		p_init_delim_state(int (*table_builder[7][17])(t_token *, t_parser *))
{
	table_builder[S_PARSER_DELIM][TOKEN] = syn_err;
	table_builder[S_PARSER_DELIM][WORD] = p_add_redir_delim;
	table_builder[S_PARSER_DELIM][ASSIGNMENT_WORD] = syn_err;
	table_builder[S_PARSER_DELIM][NEWLINE] = syn_err;
	table_builder[S_PARSER_DELIM][IO_NUMBER] = syn_err;
	table_builder[S_PARSER_DELIM][AND_IF] = syn_err;
	table_builder[S_PARSER_DELIM][AMP] = syn_err;
	table_builder[S_PARSER_DELIM][OR_IF] = syn_err;
	table_builder[S_PARSER_DELIM][PIPE] = syn_err;
	table_builder[S_PARSER_DELIM][SEMI] = syn_err;
	table_builder[S_PARSER_DELIM][LESS] = syn_err;
	table_builder[S_PARSER_DELIM][DLESS] = syn_err;
	table_builder[S_PARSER_DELIM][GREAT] = syn_err;
	table_builder[S_PARSER_DELIM][DGREAT] = syn_err;
	table_builder[S_PARSER_DELIM][LESSAND] = syn_err;
	table_builder[S_PARSER_DELIM][GREATAND] = syn_err;
	table_builder[S_PARSER_DELIM][DLESSDASH] = syn_err;
}

void		init_parser(t_parser *parser)
{
	parser->state = S_PARSER_TABLE_START;
	parser->prev_state = S_PARSER_TABLE_START;
	parser->table = NULL;
	parser->curr_table = NULL;
}

int	p_process_token(t_token *token, t_parser *parser, int (*table_builder[7][17])(t_token *, t_parser *))
{
	if (!table_builder[parser->state][token->type](token, parser))
	{
		ft_printf("echec parsing\n");
		return (0);
	}
	return (1);
}

int	p_tokeniter(t_list *token, t_parser *parser, int (*table_builder[7][17])(t_token *, t_parser *))
{
	while (token)
	{
		if (!p_process_token(token->data, parser, table_builder))
			return (0);
		token = token->next;
	}
	return (1);
}

void	p_init_state_machine(int (*table_builder[7][17])(t_token *, t_parser *))
{
	p_init_start_state(table_builder);
	p_init_cmd_wait_state(table_builder);
	p_init_args_wait_state(table_builder);
	p_init_redir_state(table_builder);
	p_init_assign_state(table_builder);
	p_init_io_nbr_state(table_builder);
	p_init_delim_state(table_builder);
}

void		del_token(void *data, size_t size)
{
	t_token		*token;

	(void)size;
	token = (t_token *)data;
	ft_strdel(&token->str);
}

int		ft_parser(t_lexer *lexer, t_parser *parser)
{
	int	(*table_builder[7][17])(t_token *, t_parser *);

	ft_printf("\n----------- parsing -----------\n\n");
	if (!lexer || !parser)
		return (0);
	init_parser(parser);
	p_init_state_machine(table_builder);
	p_tokeniter(lexer->token_lst, parser, table_builder);
	while (parser->state == S_PARSER_CMD_START)
	{
		ft_lstdel(&lexer->token_lst, del_token);
		ft_lexer(ft_prompt("> "), lexer);
		p_tokeniter(lexer->token_lst, parser, table_builder);
	}
	print_parser(parser);
	p_make_args_tab(parser);
	return (1);
}
