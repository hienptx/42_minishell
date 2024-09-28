#include "../includes/minishell.h"

extern char **environ;

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        rl_replace_line("", 1); // Replace current input with empty string
        rl_on_new_line(); // Signal that we are on a new line
        printf("\n"); // Redisplay to show the updated (empty) input prompt
        rl_redisplay();
    }
    if (sig == SIGQUIT)
    {
        ;
    }
}
void process_tokens(char **tok, t_list *env_list)
{
    int i;
    char *ptr;
    t_cmd *ast;

    ast = NULL;
    i = 0;
    while (tok[i] != NULL)
    {
        // printf("%s\n", tok[i]);
        ptr = tok[i];
        tok[i] = expansion_handling(tok[i]);
        if (ptr != tok[i])
            free(ptr);
        ptr = tok[i];
        tok[i] = quote_handling(tok[i]);
        if (ptr != tok[i])
            free(ptr);
        i++;
    }
    ast = parse_cmd(tok);
    // print_command_tree(ast, 0); //print abstract syntax tree from root
    iterate_ast(ast, env_list);
    free_ast(ast);
    free_tokens(tok);
}
// int check_validation(char *input)
// {
//     if (only_space(input) || unclosed_quote(input))
//         return(1);
//     return(0);
// }

int main(void) 
{
    t_list  *env_list;
    char *input;
    char **tok;
 
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    set_env(&env_list);
    // while (env_list != NULL)
    // {
    //     printf("initial env %s\n", (char *)env_list->content);
    //     env_list = env_list->next;
    // }
    while(1)
    {
        input = readline("$> ");
        if (input == NULL) // Check for Ctrl-D (EOF)
        {
            printf("EOF, exiting shell\n");
            break;
        }
        if (only_space(input))
        {
            free(input);
            continue;
        }
        if (input && *input)
        {
            add_history(input);
            tok = get_tokens(input);
            if (unclosed_quote(tok))
            {
                free_tokens(tok);
                free(input);
                continue;
            }
            else
                process_tokens(tok, env_list);
        }
        free(input);
    }
    return 0;
}

// void	set_env(t_list **env_list)
// {

// 	*env_list = cp_env_list();
// 	ft_getenv = getenv;
// 	return ;
// }