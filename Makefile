NAME = minishell

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
LIBFT_DIR = ./libft
INCL_DIR = ./includes
LIBFT_H = 

CC = cc

CFLAGS = -g -Wall -Wextra -Werror -I$(INCL_DIR)

LDFLAGS = -Llibft -lreadline 

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = constructor \
		execution_utils \
		execution \
		expansion \
		free \
		ft_strsjoin \
		here_doc \
		minishell_utils \
		minishell \
		parser \
		pipe \
		quotes \
		redirect \
		syntax_check \
		tokenizer

BUILTINS = builtin_change_env \
			builtin_get_env \
			builtin_sort_env \
			builtin \
			builtins_utils \
			more_builtins \
			qsort \

SRCS := $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(SRCS)))
SRCS += $(addprefix $(SRCS_DIR)/builtin/, $(addsuffix .c, $(BUILTINS)))

OBJS = $(SRCS:.c=.o)

all: $(OBJS_DIR) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Creating $(NAME)"
	$(CC) -v $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME);

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(LIBFT):
	@echo "Creating archive $(LIBFT)"
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all
