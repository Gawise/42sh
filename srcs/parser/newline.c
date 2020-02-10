#include "libft.h"
#include "lexer.h"
#include "parser.h"

int	p_add_table(t_token *token, t_parser *parser)
{
	(void)token;
	if (!(parser->curr_table->next = ft_lstnew(NULL, 0))
	|| !(parser->curr_table->next->data = init_cmd_table()))
		return (0);
	parser->curr_table = parser->curr_table->next;
	parser->state = 0;
	return (1);
}
