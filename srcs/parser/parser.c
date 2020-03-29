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
	{
		ft_printf("echec parsing\n");
		return (0);
	}
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

char		*get_args_str(char **args)
{
	int	i;
	char	*str;
	char	*tmp;

	if (args && args[0])
	{
		if (!(str = ft_strdup(args[0])))
		{
			ft_printf("erreur get_args_str \n");
			exit(1);
		}
	}
	else
		return (0);
	i = 1;
	while (args[i])
	{
		if (ft_asprintf(&tmp, "%s %s", str, args[i]) == -1)
		{
			ft_printf("erreur get_args str\n");
			exit(1);
		}
		ft_strdel(&str);
		str = tmp;
		i++;
	}
	return (str);
}

char		*get_redir_type_str(t_token_type type)
{
	if (type == LESS)
		return (ft_strdup("<"));
	else if (type == DLESS)
		return (ft_strdup("<<"));
	else if (type == GREAT)
		return (ft_strdup(">"));
	else if (type == DGREAT)
		return (ft_strdup(">>"));
	else if (type == LESSAND)
		return (ft_strdup("<&"));
	else if (type == GREATAND)
		return (ft_strdup(">&"));
	else if (type == DLESSDASH)
		return (ft_strdup("<<-"));
	return (NULL);
}

char		*get_redir_fmt(t_redir *redir)
{
	char	*str;

	if (!redir->io_num && !(redir->io_num = ft_strdup("")))
	{
		ft_printf("erreur asprintf get_redir_fmt\n");
		exit(1);
	}
	if (redir->type == DLESS || redir->type == DLESSDASH)
	{
		if (!(str = ft_strdup("%s%s%s\n%s")))
		{
			ft_printf("erreur asprintf get_redir_fmt\n");
			exit(1);
		}
	}
	else
	{
		if (!(str = ft_strdup("%s%s%s %s"))
		|| !(redir->delim = ft_strdup("")))
		{
			ft_printf("erreur asprintf get_redir_fmt\n");
			exit(1);
		}
	}
	return (str);
}

char		*get_redir_str(t_list *redir_lst)
{
	t_redir	*redir;
	char	*res;
	char	*fmt;
	char	*str;
	char	*tmp;

	if (!redir_lst || !redir_lst->data)
		return (NULL);
	redir = (t_redir *)redir_lst->data;
	if (ft_asprintf(&res, (fmt = get_redir_fmt(redir)), redir->io_num,
	(tmp = get_redir_type_str(redir->type)), redir->delim, redir->file) == -1)
	{
		ft_printf("erreur asprintf get_redir_str\n");
		exit(1);
	}
	ft_strdel(&tmp);
	ft_strdel(&fmt);
	redir_lst = redir_lst->next;
	while (redir_lst && redir_lst->data)
	{
		redir = (t_redir *)redir_lst->data;
		if (!(fmt = get_redir_fmt(redir)))
			return (NULL);
		if (ft_asprintf(&str, fmt, redir->io_num,
		(tmp = get_redir_type_str(redir->type)),
		redir->delim, redir->file) == -1)
		{
			ft_printf("erreur asprintf get_redir_str\n");
			exit(1);
		}
		ft_strdel(&fmt);
		ft_strdel(&tmp);
		redir_lst = redir_lst->next;
		if (ft_asprintf(&tmp, "%s %s", res, str) == -1)
		{
			ft_printf("erreur asprintf get_redir_str\n");
			exit(1);
		}
		ft_strdel(&res);
		ft_strdel(&str);
		res = tmp;
	}
	return (res);
}

char		*get_assign_str(t_list *assign_lst)
{
	t_assignment	*assign;
	char		*new;
	char		*res;
	char		*tmp;

	if (!assign_lst || !assign_lst->data)
		return (NULL);
	assign = (t_assignment *)assign_lst->data;
	if (ft_asprintf(&res, "%s=%s", assign->var, assign->val) == -1)
	{
		ft_printf("erreur asprintf get_assign_str\n");
		exit(1);
	}
	assign_lst = assign_lst->next;
	while (assign_lst && assign)
	{
		assign = (t_assignment *)assign_lst->data;
		if (ft_asprintf(&new, "%s=%s", assign->var, assign->val) == -1)
		{
			ft_printf("erreur asprintf get_assign_str\n");
			exit(1);
		}
		if (ft_asprintf(&tmp, "%s %s", res, new) == -1)
		{
			ft_printf("erreur asprintf get_assign_str 2\n");
			exit(1);
		}
		ft_strdel(&res);
		ft_strdel(&new);
		res = tmp;
		assign_lst = assign_lst->next;
	}
	return (res);
}

char		*add_str_to_job(char **str, char **new)
{
	char	*res;

	res = NULL;
	if (!str || !new)
		return (NULL);
	if (!*str && *new)
	{
		if (!(res = ft_strdup(*new)))
			return (NULL);
	}
	else if (*str && *new)
	{
		if (ft_asprintf(&res, "%s %s", *str, *new) == -1)
			return (NULL);
	}
	else if (*str && !*new)
	{
		if (!(res = ft_strdup(*str)))
			return (NULL);
	}
	if (*new)
		ft_strdel(new);
	if (*str)
		ft_strdel(str);
	return (res);
}

char		*get_cmd_str(t_simple_cmd *cmd)
{
	char	*str;
	char	*assign;
	char	*args;
	char	*redir;

	str = NULL;
	assign = get_assign_str(cmd->assign);
	str = add_str_to_job(&str, &assign);
	args = get_args_str(cmd->av);
	str = add_str_to_job(&str, &args);
	redir = get_redir_str(cmd->redir);
	str = add_str_to_job(&str, &redir);
	return (str);
}

int		p_and_or_job_str(t_and_or *and_or)
{
	t_list	*cmd_lst;
	char	*str;
	char	*res;
	char	*tmp;

	res = NULL;
	cmd_lst = and_or->s_cmd;
	if (cmd_lst->data && !(res = get_cmd_str((t_simple_cmd *)cmd_lst->data)))
		return (0);
	cmd_lst = cmd_lst->next;
	while (cmd_lst)
	{
		if (cmd_lst->data && !(str = get_cmd_str((t_simple_cmd *)cmd_lst->data)))
			return (0);
		if (cmd_lst->data && ft_asprintf(&tmp, "%s | %s", res, str) == -1)
			return (0);
		ft_strdel(&str);
		ft_strdel(&res);
		res = tmp;
		cmd_lst = cmd_lst->next;
	}
	and_or->str = res;
	return (1);
}

int		p_set_jobs_str(t_parser *parser)
{
	t_list		*lst;
	t_cmd_table	*cmd_table;
	t_list		*and_or;


	lst = parser->table;
	while (lst)
	{
		if (!(cmd_table = (t_cmd_table *)lst->data))
			return (1);
		and_or = cmd_table->and_or;
		while (and_or)
		{
			p_and_or_job_str((t_and_or *)and_or->data);
			and_or = and_or->next;
		}
		lst = lst->next;
	}
	return (1);
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

	if (cfg_shell()->debug)
		ft_printf("\n----------- parsing -----------\n\n");
	if (!lexer || !parser)
		return (0);
	init_parser(parser);
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
	if (parser->state != S_PARSER_SYNTAX_ERROR
	&& !p_set_jobs_str(parser))
		return (0);
	return (1);
}
