NAME = minishell

SRCS_DIR = ./srcs
OBJS_DIR = ./objs
LIBFT_DIR = ./libft
INCL_DIR = ./includes
LIBFT_H = 

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INCL_DIR)
LDFLAGS = -lreadline 

LIBFT = $(LIBFT_DIR)/libft.a

SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

all: $(OBJS_DIR) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@echo "Creating $(NAME)"
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

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
