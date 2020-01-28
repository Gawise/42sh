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


SRCS +=
SRCS +=
SRCS +=
SRCS +=
SRCS +=

CC = Clang
COMPILE = $(CC) -c
COMPILEDB = $(CC) -g

MKDIR = mkdir -p
CLEANUP = rm -rf

DSYN = $(NAMEDB).dSYM


PATHO = obj/
SPATH = srcs/
IPATH = includes/
LPATH = libft/
LIPATH = libft/includes/
LIB = $(LPATH)libft.a
LIBDB = $(LPATH)libft_db.a

WFLAGS = -Wall -Werror -Wextra
IFLAGS = -I$(IPATH) -I$(LIPATH)
CFLAGS = $(WFLAGS) $(IFLAGS)
DBFLAGS = -fsanitize=address


OBJS = $(patsubst %.c, $(PATHO)%.o, $(SRCS))


all : $(NAME)

$(NAME) : $(LIB) $(PATHO) $(OBJS)
	$(CC) -o $@ $< $(OBJS)
	printf "$(GREEN)$@ is ready.\n$(NC)"

$(OBJS) : $(PATHO)%.o : $(SPATH)%.c
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
	printf "$(RED)minishell is delete\n$(NC)"

re : fclean all

norme:
	norminette srcs
	norminette includes
	norminette libft/srcs
	norminette libft/includes

.PHONY: all clean fclean norme re
.SILENT:
