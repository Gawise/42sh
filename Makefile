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
LEXSRCS += free.c
LEXSRCS += heredoc.c
LEXSRCS += inhib.c
LEXSRCS += lexer.c
LEXSRCS += misc.c
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
LEXSRCS += state/init.c
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
PARSRCS += free.c
PARSRCS += init.c
PARSRCS += lst_to_tab.c
PARSRCS += misc.c
PARSRCS += newline.c
PARSRCS += parser.c
PARSRCS += redir.c
PARSRCS += tools.c
PARSRCS += word.c
PARSRCS += state/andif_pipe.c
PARSRCS += state/arg_assign.c
PARSRCS += state/assign.c
PARSRCS += state/cmd_args.c
PARSRCS += state/cmd_start.c
PARSRCS += state/delim.c
PARSRCS += state/error.c
PARSRCS += state/init.c
PARSRCS += state/io_number.c
PARSRCS += state/redir.c
PARSRCS += state/table_start.c

## ANALYZER ##

ANASRCS += jobs_str/core.c
ANASRCS += jobs_str/redir.c
ANASRCS += heredoc.c

## EVAL ##

EVALSRCS += launcher.c
EVALSRCS += process_routines.c
EVALSRCS += exec_type.c
EVALSRCS += cleaner.c
EVALSRCS += constructor.c
EVALSRCS += bridge_step.c
EVALSRCS += wait.c
EVALSRCS += redir.c
EVALSRCS += tools_var.c
EVALSRCS += tools_redir.c
EVALSRCS += tools_termios.c
EVALSRCS += errors_handling.c
EVALSRCS += ft_lst.c
EVALSRCS += debug.c

## JOB CONTROL ##

JBSRCS += tools_job.c
JBSRCS += ft_lst.c
JBSRCS += routine_check_child.c

## COMMON TOOLS  ##

TOOLSRCS += path_errors.c
TOOLSRCS += tools_path.c
TOOLSRCS += setvar_add.c
TOOLSRCS += tools_var.c
TOOLSRCS += all_signal.c

## BUILTIN ##

BTSRCS += exit.c
BTSRCS += hash.c
BTSRCS += env.c
BTSRCS += setenv.c
BTSRCS += unsetenv.c

## INCLUDES ##

INCLUDES += analyzer.h
INCLUDES += exec.h
INCLUDES += job_control.h
INCLUDES += lexer.h
INCLUDES += line_edition.h
INCLUDES += parser.h
INCLUDES += sh.h
INCLUDES += struct.h
INCLUDES += var.h


## DEBUG ##

DBSRCS += lexer.c
DBSRCS += parser.c

SRC += main.c
SRC += init_shell.c
SRC += init_cfg.c
SRC += $(addprefix line_edition/,$(LESRCS))
SRC += $(addprefix lexer/,$(LEXSRCS))
SRC += $(addprefix parser/,$(PARSRCS))
SRC += $(addprefix analyzer/,$(ANASRCS))
SRC += $(addprefix evaluator/,$(EVALSRCS))
SRC += $(addprefix job_control/,$(JBSRCS))
SRC += $(addprefix builtins/,$(BTSRCS))
SRC += $(addprefix tools/,$(TOOLSRCS))
SRC += $(addprefix debug/,$(DBSRCS))


OPATHS += $(OPATH)line_edition
OPATHS += $(OPATH)lexer
OPATHS += $(OPATH)lexer/state
OPATHS += $(OPATH)parser
OPATHS += $(OPATH)parser/state
OPATHS += $(OPATH)analyzer
OPATHS += $(OPATH)analyzer/jobs_str
OPATHS += $(OPATH)evaluator
OPATHS += $(OPATH)builtins
OPATHS += $(OPATH)debug
OPATHS += $(OPATH)tools
OPATHS += $(OPATH)job_control

CC = clang
COMPILE = $(CC) -c
COMPILEDB = $(CC) -g3

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

INCS = $(addprefix $(IPATH),$(INCLUDES))

all : $(NAME)

$(NAME) : $(LIB) $(OPATHS) $(OBJS) Makefile
	$(CC) -lncurses -o $@ $(OBJS) $<
	printf "$(GREEN)$@ is ready.\n$(NC)"

$(OBJS) : $(OPATH)%.o : $(SPATH)%.c $(INCS)
	$(COMPILE) $(CFLAGS) $< -o $@

$(OPATHS) :
	$(MKDIR) $@

$(LIB) :
	$(MAKE) -C $(LPATH)

$(LIBDB) :
	$(MAKE) -C $(LPATH) debug

debug : $(LIBDB)
	$(COMPILEDB) $(DBFLAGS) -lncurses $(CFLAGS) -o $(NAMEDB) $(SRCS) $^
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
