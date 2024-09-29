
#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../includes/builtin.h"
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
    // int here_doc;
}               t_redir;

typedef struct pipe
{

    void  *left;
    void *right;
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
void signal_handler(int sig);

// expansion.c
char *expansion_handling(char *str, t_list *env_list);
char *replace_substring(char *str, char *newsub, char *oldsub);

// quotes.c
char *quote_handling(char *token);
size_t get_strlen(char *token);
int unclosed_quote(char **token);

// tokenizer_input.c
char **get_tokens(char *str, size_t *nbr_tokens);
char *cpy_str(char **ret, char *str, size_t *pos);
size_t count_tokens(char *str);
char *walk_string(char *str, char c);

//minishell_utils.c
void *ft_malloc(void *data, size_t size);
void free_tokens(char **tokens, size_t nbr_tokens);
void panic_sms(char *s);
int only_space(char *input);

//constructor.c
t_cmd *construct_pipe(char *left, char *right);
t_cmd *construct_redir(t_cmd *command, int fd, char *file_name);
t_exec *construct_exec(char **tokens, t_exec *data);
void free_ast(t_cmd *ast);

//parser.c
t_cmd *parse_cmd(char **tokens);
t_cmd *parse_redir(char **tokens);
t_cmd *parse_exec(char **tokens);
void print_command_tree(t_cmd *cmd, int level);
void iterate_ast(t_cmd *cmd, t_list *env_list);

//here_doc.c
int heredoc_process(char *arg2);
t_cmd *parse_here_doc(t_cmd *command, char **tokens, int i);

//pipe.c
void    left_pipe(t_pipe *pipe_cmd, int *fd, t_list *env_list);
void    right_pipe(t_pipe *pipe_cmd, int *fd, t_list *env_list);
int set_pipe(t_pipe *pipe_cmd, t_list *env_list);

//redirect.c
void    set_redir(t_redir *redir_cmd, t_list *env_list);

//execution.c
int call_exec(t_exec *exec_cmd, t_list *env_list);
void    run_exec(t_exec *exec_cmd, t_list *env_list);
int set_exec(t_exec *exec_cmd, t_list *env_list);
char    *get_executable_path(char *env_path, char *prog_name);
char    **get_all_path(char *env_path);


void	set_env(t_list **env_list);
void    run_exec(t_exec *exec_cmd, t_list *env_list);
int     ck_builtin(char *executable_name);
// char *(*ft_getenv)(const char *key);
int	find_env(t_list *env_list, char *key);
int call_builtin(t_exec *exec_cmd, t_list *env_list);

char    *ft_strsjoin(const char *delimiter, ...);
int get_full_len(va_list args, const char *delimiter);
char    *cp_strs(char *result, va_list args, const char *delimiter);

#endif