NAME = 21sh
NAMEDB = 21sh_db

# Reset
NC=\033[0m

# Regular Colors
BLACK=\033[0;30m
RED=\033[0;31m
GREEN =\033[32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
WHITE=\033[0;37m


SRCS += main.c
SRCS += lexer.c
SRCS += parser.c
SRCS += init.c
SRCS += word.c
SRCS += redir.c
SRCS += assign.c
SRCS += and_or.c
SRCS += cmd.c
SRCS += amp.c
SRCS += newline.c
SRCS += args_tab.c
SRCS += lst_to_tab.c
SRCS += parserdb.c

vpath %.c srcs
vpath %.c srcs/lexer
vpath %.c srcs/parser
vpath %.c srcs/evaluator
vpath %.c srcs/debug
vpath %.h includes

CC = clang
COMPILE = $(CC) -c
COMPILEDB = $(CC) -g

MKDIR = mkdir -p
CLEANUP = rm -rf

DSYN = $(NAMEDB).dSYM


PATHO = objs/
SPATH = srcs/
IPATH = includes/
LPATH = libft/
LIPATH = libft/includes/
LIB = $(LPATH)libft.a
LIBDB = $(LPATH)libft_db.a

WFLAGS = -Wall -Werror -Wextra
IFLAGS = -I $(IPATH) -I $(LIPATH)
CFLAGS = $(WFLAGS) $(IFLAGS)
DBFLAGS = -fsanitize=address


OBJS = $(patsubst %.c, $(PATHO)%.o, $(SRCS))


all : $(NAME)

$(NAME) : $(LIB) $(PATHO) $(OBJS)
	printf "$< $(OBJS)\n"
	$(CC) -o $@ $(OBJS) $<
	printf "$(GREEN)$@ is ready.\n$(NC)"

$(OBJS) : $(PATHO)%.o : %.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO) :
	$(MKDIR) $@

$(LIB) :
	$(MAKE) -C $(LPATH)

$(LIBDB) :
	$(MAKE) -C $(LPATH) debug


debug : $(LIBDB)
	$(COMPILEDB) $(DBFLAGS) $(CFLAGS) -o $(NAMEDB) $^ srcs/*.c
	printf "$(GREEN)$(NAMEDB) is ready.\n$(NC)"

clean :
	$(MAKE) -C $(LPATH) clean
	$(CLEANUP) $(OBJS)
	$(CLEANUP) $(PATHO)
	$(CLEANUP) $(DSYN)
	printf "$(RED)Directory $(NAME) is clean\n$(NC)"

fclean : clean
	$(MAKE) -C $(LPATH) fclean
	$(CLEANUP) $(PATHO)
	$(CLEANUP) $(NAME)
	$(CLEANUP) $(NAMEDB)
	printf "$(RED)$(NAME) is delete\n$(NC)"

re : fclean all

norme:
	norminette srcs
	norminette includes
	norminette libft/srcs
	norminette libft/includes

.PHONY: all clean fclean norme re debug
.SILENT:
