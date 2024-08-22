#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// #include "minishell/libft/includes/libft.h"
#include "../libft/includes/libft.h"

#define EXEC 1
#define PIPE 2
#define REDIR 3

typedef void (*sighandler_t)(int);

struct pipe
{
    int type;
    char *left;
    char *right;
};

#endif