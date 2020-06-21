#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"
#include "var.h"
#include "sh.h"
#include "exec.h"

static t_var	*remove_alias_item(t_list **alias_lst, char *name)
{
	t_list	*res;
	t_list	*tmp;
	t_var	*var;

	res = *alias_lst;
	var = res->data;
	if (ft_strequ(var->ctab[0], name))
	{
		*alias_lst = res->next;
		free(res);
		return (var);
	}
	while (res->next)
	{
		var = res->next->data;
		if (ft_strequ(var->ctab[0], name))
		{
			tmp = res->next;
			res->next = res->next->next;
			free(tmp);
			return (var);
		}
		res = res->next;
	}
	return (NULL);
}

char			get_last_char(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (str[--i]);
}

int				p_expand_alias(t_token *token, t_parser *parser)
{
	t_var	*cpy;
	t_lexer	lexer;
	int		(*table_builder[10][17])(t_token *, t_parser *);

	ft_bzero(&lexer, sizeof(t_lexer));
	cpy = remove_alias_item(&cfg_shell()->alias, token->str);
	ft_lexer(&cpy->ctab[1], &lexer);
	p_init_state_machine(table_builder);
	p_tokeniter(lexer.token_lst, parser, table_builder, 0);
	ft_lstdel(&lexer.token_lst, del_token);
	ft_lstpush(&cfg_shell()->alias, cpy, sizeof(t_var));
	parser->space_flag = 0;
	if (ft_strchr(" \t", get_last_char(cpy->ctab[1])))
		parser->space_flag = 1;
	return (1);
}
