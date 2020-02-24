/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:43:11 by hmerieux          #+#    #+#             */
/*   Updated: 2020/02/24 16:29:08 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <fcntl.h>

# define CVTBUFSZ 200

typedef struct  s_dlist
{
    void            *data;
    size_t			size;
    struct s_dlist	*next;
	struct s_dlist	*prev;
}				t_dlist;

typedef struct	s_list
{
	void			*data;
	size_t			size;
	struct s_list	*next;
}				t_list;

typedef union	u_modf
{
	double				f;
	long long unsigned	i;
}				t_modf;

int				ft_isdir(char *path, int link);
void			ft_lst_join(t_list *lst1, t_list **lst2, int refresh);
t_list			*ft_lstnew(void const *data, size_t size);
t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
int				ft_recursive_power(int nb, int pw);
int				ft_sqrt(int nb);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
int				ft_strchri(const char *s, int c);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_strequ(char const *s1, char const *s2);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
int				ft_getopt(int *ac, int *i, char **const av, char *flags);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strdup(const char *s1);
char			*ft_strcpy(char *dst, const char *src);
char			*ft_strncpy(char *dst, const char *src, size_t len);
char			*ft_strstr(const char *haystack, const char *needle);
char			*ft_strnstr(const char *h, const char *n, size_t len);
char			*ft_strcat(char *s1, const char *s2);
char			*ft_strncat(char *s1, const char *s2, size_t n);
char			*ft_strnew(size_t size);
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strjoin(int nb, ...);
char			*ft_strtrim(char const *s);
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_itoa(int n);
char			*ft_strselect(char *str, char c);
char			*ft_which(char *paths, char *exec);
char			**ft_strsplit(char const *s, char *c);
void			*ft_memset(void *b, int c, size_t len);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_memalloc(size_t size);
void			ft_lstadd(t_list **alst, t_list *new);
void			ft_memdel(void **ap);
void			ft_bzero(void *s, size_t n);
void			ft_putchar(char c);
void			ft_putstr(char const *s);
void			ft_putendl(char const *s);
void			ft_putchar_fd(char c, int fd);
void			ft_strclr(char *s);
void			ft_striter(char *s, void (*f)(char *));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
void			ft_putstr_fd(char const *s, int fd);
void			ft_putendl_fd(char const *s, int fd);
void			ft_strdel(char **as);
void			ft_putnbr(int n);
void			ft_putnbr_fd(int n, int fd);
void			ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void			ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void			ft_lstiter(t_list *lst, void (*f)(t_list *elem));
void			ft_print_bits(unsigned char nb);
void			ft_del_tab(void **tabi);
size_t			ft_strlcat(char *dst, const char *src, size_t size);
size_t			ft_strlen(const char *s);
unsigned char	ft_swap_bits(unsigned char octet);
void			ft_lst_push_front(t_list **begin, void *data,
		size_t size);
void			ft_lst_push_back(t_list **begin, void *data,
		size_t size);
int				ft_lstdelif(t_list **begin, void *data,
		int (*f)(void *, void *), void (*del)(void *, size_t));
size_t			ft_wcharlen(unsigned wc);
size_t			ft_wstrlen(unsigned *str);
void			ft_ftoa(double val, int prec, char res[CVTBUFSZ]);
int				ft_strichr(char *s, int c);
t_list			*ft_lstpush(t_list **alst, void *data, size_t size);
char			*ft_strndup(const char *s1, size_t n);
void			ft_lstaddtail(t_list **alst, t_list *new);
t_list			*ft_lstgettail(t_list *alst);
void			ft_lstdeltail(t_list **alst, void (*del)(void *, size_t));
char			*ft_strnlclnjoin(char const *s1, char const *s2, size_t n);
char			*ft_strclnjoin(char const *s1, char const *s2);
void			ft_lstdelhead(t_list **alst, void (*del)(void*, size_t));
char			*ft_fcvt(double val, int ndigits, int *decpt, char *buf);
double			ft_modf(double val, double *integral);
t_dlist			*ft_dlstnew(void *content, size_t content_size);
void			ft_dlstdelone(t_dlist **alst);
void			ft_dlstdel(t_dlist **alst);
void			ft_dlstaddtail(t_dlist **alst, t_dlist *new);

#endif
