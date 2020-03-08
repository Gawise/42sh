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

## LINE EDITION ##

LESRCS += cursor.c
LESRCS += ft_prompt.c
LESRCS += history.c
LESRCS += print_cmd.c
LESRCS += keys2.c
LESRCS += move_word.c
LESRCS += signals.c
LESRCS += term_init.c
LESRCS += utils.c
LESRCS += clipboard.c
LESRCS += cursor_position.c
LESRCS += input.c
LESRCS += arrow_keys.c
LESRCS += maj_arrow_keys.c
LESRCS += del_keys.c
LESRCS += other_keys.c
## LEXER ##

LEXSRCS += buffer.c
LEXSRCS += control.c
LEXSRCS += digit.c
LEXSRCS += equ.c
LEXSRCS += exp.c
LEXSRCS += flag.c
LEXSRCS += heredoc.c
LEXSRCS += inhib.c
LEXSRCS += lexer.c
LEXSRCS += newline.c
LEXSRCS += quote.c
LEXSRCS += redir.c
LEXSRCS += token.c
LEXSRCS += tools.c
LEXSRCS += type.c
LEXSRCS += word.c
LEXSRCS += state/control.c
LEXSRCS += state/exp.c
LEXSRCS += state/flag.c
LEXSRCS += state/hdbody.c
LEXSRCS += state/ionumber.c
LEXSRCS += state/redir.c
LEXSRCS += state/start.c
LEXSRCS += state/word.c

## PARSER ##

PARSRCS += amp.c
PARSRCS += and_or.c
PARSRCS += args_tab.c
PARSRCS += assign.c
PARSRCS += cmd.c
PARSRCS += init.c
PARSRCS += lst_to_tab.c
PARSRCS += newline.c
PARSRCS += parser.c
PARSRCS += redir.c
PARSRCS += word.c

## EVAL ##

EVALSRCS += launcher.c
EVALSRCS += pipe.c
EVALSRCS += exec_type.c
EVALSRCS += tools_var.c
EVALSRCS += setenv.c
EVALSRCS += cleaner.c
EVALSRCS += building_struct.c
EVALSRCS += leveling.c
EVALSRCS += wait.c
EVALSRCS += path_err.c
EVALSRCS += path_err_tools.c

## DEBUG ##

DBSRCS += lexer.c
DBSRCS += parser.c

SRC += main.c
SRC += init_shell.c
SRC += init_cfg.c
SRC += $(addprefix line_edition/,$(LESRCS))
SRC += $(addprefix lexer/,$(LEXSRCS))
SRC += $(addprefix parser/,$(PARSRCS))
SRC += $(addprefix evaluator/,$(EVALSRCS))
SRC += $(addprefix debug/,$(DBSRCS))

OPATHS += $(OPATH)line_edition
OPATHS += $(OPATH)lexer
OPATHS += $(OPATH)lexer/state
OPATHS += $(OPATH)parser
OPATHS += $(OPATH)evaluator
OPATHS += $(OPATH)debug

vpath %.h includes

CC = clang
COMPILE = $(CC) -c
COMPILEDB = $(CC) -g

MKDIR = mkdir -p
CLEANUP = rm -rf

DSYN = $(NAMEDB).dSYM

OPATH = objs/
SPATH = srcs/
IPATH = includes/
LPATH = libft/
LIPATH = libft/includes/
LIB = $(LPATH)libft.a
LIBDB = $(LPATH)libft_db.a

WFLAGS = -g -Wall -Werror -Wextra
IFLAGS = -I $(IPATH) -I $(LIPATH)
CFLAGS = $(WFLAGS) $(IFLAGS)
DBFLAGS = -fsanitize=address

OBJ = $(patsubst %.c, %.o, $(SRC))

SRCS = $(addprefix $(SPATH),$(SRC))

OBJS = $(addprefix $(OPATH),$(OBJ))

all : $(NAME)

$(NAME) : $(LIB) $(OPATHS) $(OBJS)
	$(CC) -lncurses -o $@ $(OBJS) $<
	printf "$(GREEN)$@ is ready.\n$(NC)"

$(OBJS) : $(OPATH)%.o : $(SPATH)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(OPATHS) :
	$(MKDIR) $@

$(LIB) :
	$(MAKE) -C $(LPATH)

$(LIBDB) :
	$(MAKE) -C $(LPATH) debug

debug : $(LIBDB)
	$(COMPILEDB) $(DBFLAGS) -lncurses $(CFLAGS) -o $(NAMEDB) $^ $(SRCS)
	printf "$(GREEN)$(NAMEDB) is ready.\n$(NC)"

clean :
	$(MAKE) -C $(LPATH) clean
	$(CLEANUP) $(OBJS)
	$(CLEANUP) $(OPATH)
	$(CLEANUP) $(DSYN)
	printf "$(RED)Directory $(NAME) is clean\n$(NC)"

fclean : clean
	$(MAKE) -C $(LPATH) fclean
	$(CLEANUP) $(OPATH)
	$(CLEANUP) $(NAME)
	$(CLEANUP) $(NAMEDB)
	printf "$(RED)$(NAME) is delete\n$(NC)"

re : fclean all

norme:
	norminette $(SPATH)
	norminette $(IPATH)
	norminette $(LPATH)/$(SPATH)
	norminette $(LPATH)/$(IPATH)

.PHONY: all clean fclean norme re debug
.SILENT:
