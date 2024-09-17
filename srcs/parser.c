#include "minishell.h"

// t_cmd *parse_exec(char **cmd)

// t_cmd *parse_simple_cmd(char **cmd)
// {

// }

void construct_pipe(t_cmd *ast, char *left, char *right)
{
    ast->cmd.pipe = malloc(sizeof(t_pipe));
    if (ast->cmd.pipe == NULL)
        panic_sms("Malloc failed to allocate memory");
    ast->cmd.pipe->left = left;
    ast->cmd.pipe->right = right;
}

t_cmd *parse_cmd(char **tokens)
{
    int i;
    t_cmd *left;
    t_cmd *right;
    t_cmd *ast;

    i = 0;
    ast = NULL;
    while(tokens[i] != NULL)
    {
        printf("Parser loop\n");
        if (ft_strcmp(tokens[i], "|") != 0)
        {
            tokens[i] = NULL;
            left = parse_cmd(tokens);
            right = parse_cmd(&tokens[i + 1]);
            printf("after recursion\n");
            if (!left || !right)
                break ;
            ast = malloc(sizeof(t_cmd));
            if (ast == NULL)
                panic_sms("Malloc failed to allocate memory");
            ast->type = PIPE;
            printf("before Contruct\n");
            construct_pipe(ast, (char *)left, (char*)right);
            printf("after Contruct\n");
            printf("%s\n", ast->cmd.pipe->left);
            printf("%s\n", ast->cmd.pipe->right);
            return(ast);
        }
        i++;
    }
    // ast = parse_simple_cmd(tokens);
    return(ast);
    // return (parse_simple_cmd(tokens));
}       
