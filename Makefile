NAME = minishell

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
LIBFT_DIR = ./libft
INCL_DIR = ./includes
LIBFT_H = 

CC = cc
ifeq ($(shell uname -s), Darwin)
	CFLAGS = -g -Wall -Wextra -Werror -I$(INCL_DIR) -I$(shell brew --prefix readline)/include
else
	CFLAGS = -Wall -Wextra -Werror -I$(INCL_DIR)
endif

LDFLAGS = -L$(shell brew --prefix readline)/lib -lreadline 

LIBFT = $(LIBFT_DIR)/libft.a

SRCS += $(wildcard $(SRCS_DIR)/*.c) $(wildcard $(SRCS_DIR)/builtin/*.c) 
# OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))
OBJS = $(SRCS:.c=.o)

all: $(OBJS_DIR) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Creating $(NAME)"
	@if [ "$(shell uname)" = "Darwin" ]; then \
		$(CC) -v $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME); \
	else \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME); \
	fi
# -L$(shell brew --prefix readline)/lib
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
