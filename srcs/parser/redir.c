#include "libft.h"
#include "lexer.h"
#include "parser.h"


int	p_create_redir(t_simple_cmd *cmd)
{
	t_list		*lst;
	t_redir		*redir;

	if (!(lst = ft_lstnew(NULL, 0))
	|| !(redir = init_redir()))
		return (0);
	lst->data = redir;
	if (!cmd->redir)
		cmd->redir = lst;
	else
		cmd->curr_redir->next = lst;
	cmd->curr_redir = lst;
	return (1);
}

int	p_add_io_num(t_token *token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;
	t_redir		*redir;

	(void)token;
	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	if (!p_create_redir(cmd))
		return (0);
	redir = (t_redir *)cmd->curr_redir->data;
	redir->io_num = ft_atoi(token->str);
	parser->prev_state = parser->state;
	parser->state = 5;
	return (1);
}

int	p_add_redir(t_token *token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;
	t_redir		*redir;

	(void)token;
	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	if (parser->state != 5 && !p_create_redir(cmd))
		return (0);
	redir = (t_redir *)cmd->curr_redir->data;
	redir->type = token->type;
	if (parser->state != 5)
		parser->prev_state = parser->state;
	parser->state = 3;
	return (1);
}
