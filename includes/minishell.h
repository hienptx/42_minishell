
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

// simple command struct
// typedef struct s_cmd
// {
//     int type;
// }               t_cmd;

// typedef struct exec
// {
//     int type;
//     char **arg;
// }               t_exec;

// // XXX > filename
// typedef struct redir
// {
//     t_cmd cmd; //XXX
//     int type; //3
//     char *file_name; //"text.c"
//     int flag; //O_WRONLY, O_CREATE
//     int fd; // 0=stdin, 1=stdout
// }               t_redir;

// struct pipe
// {
//     int type;
//     t_cmd  *left;
//     t_cmd *right;
// };

// minishell_utils.c
char *quote_handling(char *token);
char *expansion_handling(char *str);
char *replace_substring(char *str, char *newsub, char *oldsub);

// tokenizer_input.c
char **get_tokens(char *str);
char *cpy_str(char **ret, char *str, size_t *pos);
size_t count_tokens(char *str);
char *walk_string(char *str, char c);

//minishell_utils.c
int ft_strcmp(char *s1, char *s2);
char *ft_malloc(char *str, size_t size);
void free_tokens(char **tokens);
void panic_sms(char *s);

#endif