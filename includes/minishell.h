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

// typedef void (*sighandler_t)(int);

// struct pipe
// {
//     int type;
//     t_simple_cmd  *left;
//     t_simple_cmd *right;
// };

// // XXX > filename
// struct redir
// {
//     char *cmd; //XXX
//     int type; //3
//     char *file_name; //"text.c"
//     int flag; //O_WRONLY, O_CREATE
//     int fd; // 0=stdin, 1=stdout
// };

// // simple command struct
// typedef struct simple_command
// {
//     char *cmd; //primary
//     char *flag; // secondary
//     char *argument; //secondary
// } t_simple_cmd;

// minishell_utils.c
int ft_strcmp(char *s1, char *s2);
char *ft_malloc(char *str, size_t size);

#endif