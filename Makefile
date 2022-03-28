NAME = minishell

SRC = main.c \
	parsing.c \
	exec.c \
	cmd_list.c \
	parse_cmd.c \
	check_builtins.c \
	builtin.c \
	utils.c \
	init.c \
	ft_split.c \
	ctrl.c \
	check_var.c \
	var_utils.c \
	env.c \
	dollar.c \
	quote.c

INC_HEADER = includes

DIR_S = srcs

SRCS = $(addprefix ${DIR_S}/, ${SRC})

OBJS = $(SRCS:.c=.o)

CC = clang

CFLAGS = -Wall -Wextra -Werror -g

READLINE = -lreadline

$(NAME) :	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE) -I$(INC_HEADER)


all	: $(NAME)

bonus :  $(NAME_BONUS)

clean	:
			rm -rf $(OBJS) $(OBJS_BONUS)

fclean	: clean
			rm -rf $(NAME) $(NAME_BONUS)

re : fclean all

.PHONY: re clean fclean all
