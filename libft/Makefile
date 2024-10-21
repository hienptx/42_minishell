# _*_ Makefile _*_

# target: dependencies
#	action

NAME = libft.a

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I./includes

SRC_PATH = srcs
OBJ_PATH = objs

SOURCES = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
ft_memset.c ft_strchr.c ft_strrchr.c ft_strlen.c ft_tolower.c ft_toupper.c ft_strcmp.c\
ft_strncmp.c ft_bzero.c ft_memcpy.c ft_memchr.c ft_memcmp.c ft_memmove.c ft_strlcpy.c \
ft_strlcat.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
ft_itoa.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
ft_split.c ft_strmapi.c ft_striteri.c ft_strtrim.c ft_free.c ft_strncpy.c \
get_next_line.c ft_strstr.c ft_strcpy.c ft_realloc.c ft_isspace.c
BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

OBJ_O = $(SOURCES:.c=.o)
BONUS_O = $(BONUS:.c=.o)

OBJS = $(addprefix $(OBJ_PATH)/, $(OBJ_O) $(BONUS_O))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Creating archive: $(NAME)"
	@ar -rcs $(NAME) $(OBJS)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	@$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
