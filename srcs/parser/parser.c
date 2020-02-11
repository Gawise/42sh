#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"

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
int	(*g_table_builder[22][6])(t_token *, t_parser *) = {
	{ syn_err,	syn_err,	syn_err,	syn_err,	syn_err,	syn_err }, // TOKEN
	{ p_cmd_name,	p_cmd_name,	p_add_arg,	p_file_name,	p_assign_val,	syn_err }, // WORD
	{ p_add_assign,	p_add_assign,	p_add_arg,	p_file_name,	syn_err,	syn_err }, // ASSIGNMENT
	{ p_skip,	p_add_table,	p_add_table,	syn_err,	p_add_table,	syn_err }, // NEWLINE
	{ p_add_io_num,	p_add_io_num,	p_add_io_num,	syn_err,	syn_err,	syn_err }, // IO_NUMBER
	{ syn_err,	p_add_and_or,	p_add_and_or,	syn_err,	p_add_and_or,	syn_err }, // AND_IF
	{ syn_err,	p_add_amp,	p_add_amp,	syn_err,	p_add_amp,	syn_err }, // AMP
	{ syn_err,	p_add_and_or,	p_add_and_or,	syn_err,	p_add_and_or,	syn_err }, // OR_IF
	{ syn_err,	p_add_cmd,	p_add_cmd,	syn_err,	p_add_cmd,	syn_err }, // PIPE
	{ syn_err,	p_add_table,	p_add_table,	syn_err,	p_add_table,	syn_err }, // SEMI
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

void		p_init_start_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[0][TOKEN]		=	syn_err;
	table_builder[0][WORD]		=	p_cmd_name;
	table_builder[0][ASSIGNMENT_WORD] =	p_add_assign;
	table_builder[0][NEWLINE]	=	syn_err;
	table_builder[0][IO_NUMBER]	=	p_add_io_num;
	table_builder[0][AND_IF]	=	p_add_and_or;
	table_builder[0][AMP]		=	syn_err;
	table_builder[0][OR_IF]		=	p_add_and_or;
	table_builder[0][PIPE]		=	syn_err;
	table_builder[0][SEMI]		=	p_skip;
	table_builder[0][LESS]		=	p_add_redir;
	table_builder[0][DLESS]		=	p_add_redir;
	table_builder[0][GREAT]		=	p_add_redir;
	table_builder[0][DGREAT]	=	p_add_redir;
	table_builder[0][LESSAND]	=	p_add_redir;
	table_builder[0][GREATAND]	=	p_add_redir;
//	table_builder[0][LESSGREAT]	=	p_add_redir;
	table_builder[0][DLESSDASH]	=	p_add_redir;
//	table_builder[0][CLOBBER]	=	p_add_redir;
}

void		p_init_cmd_wait_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[1][TOKEN]		=	syn_err;
	table_builder[1][WORD]		=	p_cmd_name;
	table_builder[1][ASSIGNMENT_WORD] =	p_add_assign;
	table_builder[1][NEWLINE]	=	p_add_table;
	table_builder[1][IO_NUMBER]	=	p_add_io_num;
	table_builder[1][AND_IF]	=	p_add_and_or;
	table_builder[1][AMP]		=	p_add_amp;
	table_builder[1][OR_IF]		=	p_add_and_or;
	table_builder[1][PIPE]		=	p_add_cmd;
	table_builder[1][SEMI]		=	p_add_table;
	table_builder[1][LESS]		=	p_add_redir;
	table_builder[1][DLESS]		=	p_add_redir;
	table_builder[1][GREAT]		=	p_add_redir;
	table_builder[1][DGREAT]	=	p_add_redir;
	table_builder[1][LESSAND]	=	p_add_redir;
	table_builder[1][GREATAND]	=	p_add_redir;
//	table_builder[1][LESSGREAT]	=	p_add_redir;
	table_builder[1][DLESSDASH]	=	p_add_redir;
//	table_builder[1][CLOBBER]	=	p_add_redir;
}

void		p_init_args_wait_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[2][TOKEN]		=	syn_err;
	table_builder[2][WORD]		=	p_add_arg;
	table_builder[2][ASSIGNMENT_WORD] =	p_add_arg;
	table_builder[2][NEWLINE]	=	p_add_table;
	table_builder[2][IO_NUMBER]	=	p_add_io_num;
	table_builder[2][AND_IF]	=	p_add_and_or;
	table_builder[2][AMP]		=	p_add_amp;
	table_builder[2][OR_IF]		=	p_add_and_or;
	table_builder[2][PIPE]		=	p_add_cmd;
	table_builder[2][SEMI]		=	p_add_table;
	table_builder[2][LESS]		=	p_add_redir;
	table_builder[2][DLESS]		=	p_add_redir;
	table_builder[2][GREAT]		=	p_add_redir;
	table_builder[2][DGREAT]	=	p_add_redir;
	table_builder[2][LESSAND]	=	p_add_redir;
	table_builder[2][GREATAND]	=	p_add_redir;
//	table_builder[2][LESSGREAT]	=	p_add_redir;
	table_builder[2][DLESSDASH]	=	p_add_redir;
//	table_builder[2][CLOBBER]	=	p_add_redir;
}

void		p_init_redir_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[3][TOKEN]		=	syn_err;
	table_builder[3][WORD]		=	p_file_name;
	table_builder[3][ASSIGNMENT_WORD] =	p_file_name;
	table_builder[3][NEWLINE]	=	syn_err;
	table_builder[3][IO_NUMBER]	=	p_file_name;
	table_builder[3][AND_IF]	=	syn_err;
	table_builder[3][AMP]		=	syn_err;
	table_builder[3][OR_IF]		=	syn_err;
	table_builder[3][PIPE]		=	syn_err;
	table_builder[3][SEMI]		=	syn_err;
	table_builder[3][LESS]		=	syn_err;
	table_builder[3][DLESS]		=	syn_err;
	table_builder[3][GREAT]		=	syn_err;
	table_builder[3][DGREAT]	=	syn_err;
	table_builder[3][LESSAND]	=	syn_err;
	table_builder[3][GREATAND]	=	syn_err;
//	table_builder[3][LESSGREAT]	=	syn_err;
	table_builder[3][DLESSDASH]	=	syn_err;
//	table_builder[3][CLOBBER]	=	syn_err;
}

void		p_init_assign_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[4][TOKEN]		=	syn_err;
	table_builder[4][WORD]		=	p_assign_val;
	table_builder[4][ASSIGNMENT_WORD] =	syn_err;
	table_builder[4][NEWLINE]	=	p_add_table;
	table_builder[4][IO_NUMBER]	=	syn_err;
	table_builder[4][AND_IF]	=	p_add_and_or;
	table_builder[4][AMP]		=	p_add_amp;
	table_builder[4][OR_IF]		=	p_add_and_or;
	table_builder[4][PIPE]		=	p_add_cmd;
	table_builder[4][SEMI]		=	p_add_table;
	table_builder[4][LESS]		=	syn_err;
	table_builder[4][DLESS]		=	syn_err;
	table_builder[4][GREAT]		=	syn_err;
	table_builder[4][DGREAT]	=	syn_err;
	table_builder[4][LESSAND]	=	syn_err;
	table_builder[4][GREATAND]	=	syn_err;
//	table_builder[4][LESSGREAT]	=	syn_err;
	table_builder[4][DLESSDASH]	=	syn_err;
//	table_builder[4][CLOBBER]	=	syn_err;
}

void		p_init_io_nbr_state(int (*table_builder[6][22])(t_token *, t_parser *))
{
	table_builder[5][TOKEN]		=	syn_err;
	table_builder[5][WORD]		=	syn_err;
	table_builder[5][ASSIGNMENT_WORD] =	syn_err;
	table_builder[5][NEWLINE]	=	syn_err;
	table_builder[5][IO_NUMBER]	=	syn_err;
	table_builder[5][AND_IF]	=	syn_err;
	table_builder[5][AMP]		=	syn_err;
	table_builder[5][OR_IF]		=	syn_err;
	table_builder[5][PIPE]		=	syn_err;
	table_builder[5][SEMI]		=	syn_err;
	table_builder[5][LESS]		=	p_add_redir;
	table_builder[5][DLESS]		=	p_add_redir;
	table_builder[5][GREAT]		=	p_add_redir;
	table_builder[5][DGREAT]	=	p_add_redir;
	table_builder[5][LESSAND]	=	p_add_redir;
	table_builder[5][GREATAND]	=	p_add_redir;
//	table_builder[5][LESSGREAT]	=	p_add_redir;
	table_builder[5][DLESSDASH]	=	p_add_redir;
//	table_builder[5][CLOBBER]	=	p_add_redir;
}

void		init_parser(t_parser *parser)
{
	parser->state = 0;
	parser->prev_state = 0;
	parser->table = NULL;
	parser->curr_table = NULL;
}

int	p_process_token(t_token *token, t_parser *parser, int (*table_builder[6][22])(t_token *, t_parser *))
{
	if (!table_builder[parser->state][token->type](token, parser))
	{
		ft_printf("echec parsing\n");
		return (0);
	}
	return (1);
}

int	p_tokeniter(t_list *token, t_parser *parser, int (*table_builder[6][22])(t_token *, t_parser *))
{
	while (token)
	{
		if (!p_process_token(token->data, parser, table_builder))
			return (0);
		token = token->next;
	}
	return (1);
}

void	p_init_state_machine(int (*table_builder[6][22])(t_token *, t_parser *))
{
	p_init_start_state(table_builder);
	p_init_cmd_wait_state(table_builder);
	p_init_args_wait_state(table_builder);
	p_init_redir_state(table_builder);
	p_init_assign_state(table_builder);
	p_init_io_nbr_state(table_builder);
}

int		ft_parser(t_lexer *lexer, t_parser *parser)
{
	int	(*table_builder[6][22])(t_token *, t_parser *);

	ft_printf("\n----------- parsing -----------\n\n");
	if (!lexer || !parser)
		return (0);
	init_parser(parser);
	if (!(parser->table = ft_lstnew(NULL, 0))
	|| !(parser->table->data = init_cmd_table()))
		return (0);
	parser->curr_table = parser->table;
	p_init_state_machine(table_builder);
	p_tokeniter(lexer->token_lst, parser, table_builder);
	print_parser(parser);
	p_make_args_tab(parser);
	return (1);
}
