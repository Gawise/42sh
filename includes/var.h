#ifndef VAR_H
# define VAR_H


# include <stdint.h>


typedef struct s_list t_list;






t_list	*find_var(t_list *lst, char *var);
char	*find_var_value(t_list *env, char *name);
void	cpy_var_list(void *dst, void *src);
void	create_lst_var(t_list **lst, char **tab);
char	**create_tab_var(t_list *list, int count);
int		ft_setvar(t_list **lst, char *name, char *value);
uint8_t	several_setvar(t_list  **var, char *str);

#endif
