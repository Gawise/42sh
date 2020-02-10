#include "libft.h"
#include "lexer.h"
#include "parser.h"

int	p_cmd_name(t_token *token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;

	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	if (!(cmd->cmd_name = ft_strdup(token->str)))
		return (0);
	parser->state = 2;
	return (1);
}

int	p_add_arg(t_token *token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;
	char		*str;

	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	if (!(str = ft_strdup(token->str))
	|| !ft_lstpush(&cmd->args, str, sizeof(char *)))
		return (0);
	return (1);
}

int	p_file_name(t_token *token, t_parser *parser)
{
	t_cmd_table	*table;
	t_and_or	*and_or;
	t_simple_cmd	*cmd;
	t_redir		*redir;

	table = (t_cmd_table *)parser->curr_table->data;
	and_or = (t_and_or *)table->curr_and_or->data;
	cmd = (t_simple_cmd *)and_or->curr_s_cmd->data;
	redir = (t_redir *)cmd->curr_redir->data;
	if (!(redir->file = ft_strdup(token->str)))
		return (0);
	parser->state = parser->prev_state;
	return (1);
}
