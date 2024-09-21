
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "builtin.h"
// #include "../libft/includes/libft.h"

typedef enum cmd_type
{
    EXEC, //signed as 0
    REDIR, //signed as 1
    PIPE //signed as 2
}       t_type;


typedef struct exec
{
    char **arg;
}               t_exec;

typedef struct redir
{
    t_exec *cmd;
    char *file_name;
    int fd; // 0=stdin, 1=stdout
}               t_redir;

typedef struct pipe
{

    char  *left;
    char *right;
}               t_pipe;

typedef struct s_cmd
{
    t_type type;
    union 
    {
        t_pipe *pipe;
        t_redir *redir;
        t_exec *exec;
    } cmd;
    
}               t_cmd;

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
void *ft_malloc(void *data, size_t size);
void free_tokens(char **tokens);
void panic_sms(char *s);

//constructor.c
t_cmd *construct_pipe(char *left, char *right);
t_cmd *construct_redir(t_cmd *command, int fd, char *file_name);
t_exec *construct_exec(char **tokens, t_exec *data);

//parser.c
t_cmd *parse_cmd(char **tokens);
t_cmd *parse_redir(char **tokens);
t_cmd *parse_construct_exec(char **tokens);
void print_command_tree(t_cmd *cmd, int level); 

#endif