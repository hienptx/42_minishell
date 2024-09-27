#include "../includes/minishell.h"

//  SIGNAL
// Ctrl-D exit shell -> 
// Ctrl-C Signal Interrupt - signum=SIGINT
// Ctrl-\ does nothing - signum=SIGQUIT

// BUILT-IN
// ◦echo with option -n
// ◦cd with only a relative or absolute path
// ◦pwd with no options
// ◦export with no options
// ◦unset with no options
// ◦env with no options or arguments
// ◦exit with no options

// APPROACH
// Parsecmd - recursive descent parser, also means Top-Down Parser
// ‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’.
// runcmd - walk the tree recursively. "execute" the nodes, create child processes as needed

// Signal handler for SIGINT (^C)
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

char *replace_substring(char *str, char *newsub, char *oldsub)
{
    char *ret;
    char *ptr;
    size_t len;
    size_t newsub_len;
    size_t oldsub_len;

    newsub_len = ft_strlen(newsub);
    oldsub_len = ft_strlen(oldsub);
    len = ft_strlen(str) - oldsub_len + newsub_len;
    ret = ft_malloc(NULL, len + 1);
    ptr = ret;
    while (*str != '\0' && *str != '$')
        *ret++ = *str++;
    str = str + oldsub_len;
    while (*newsub != '\0')
        *ret++ = *newsub++;
    while (*str != '\0')
        *ret++ = *str++;
    *ret = '\0';
    return (ptr);
}

char *expansion_handling(char *str)
{
    size_t len;
    char *s;
    unsigned int start;
    char *path;
    char *ret_env;
    char *ptr;

    s = ft_strchr(str, '$');
    if (s == NULL || (str[0] == '\'' && str[strlen(str) - 1] == '\'')) 
        return str;
    start = s - str;
    len = 0;
    while (*s != '\0' && *s != ' ' && *s != '\'' && *s != '\"') 
    {
        len++;
        s++;
    }
    path = ft_substr(str, start, len);
    ret_env = getenv(path + 1);
    if (ret_env == NULL) 
        ret_env = "";
    ptr = replace_substring(str, ret_env, path);
    free(path);
    return ptr;
}

char *quote_handling(char *token)
{
    char *str;
    int i;
    char *non_quote;
    char *ptr;

    i = 0;
    ptr = token;
    str = NULL;
    if (!ft_strchr(ptr, '\'') && !ft_strchr(ptr, '\"'))
        return(ptr);
    while(*ptr != '\0')
    {
        if (*ptr != '\'' && *ptr != '\"')
            i++;
        ptr++;
    }
    str = ft_malloc(str, i);
    non_quote = str;
    ptr = token;
    while(*ptr != '\0')
    {
        if(*ptr != '\'' && *ptr != '\"')
        {
            *str = *ptr;
            str++;
        }
        ptr++;
    }
    *str = '\0';
    return(non_quote);
}

// void init_syntax_tree(t_cmd *ast)
// {
//     ast->type = ft_malloc(ast->type, 3);
//     ast->execcmd = NULL;
//     ast->pipecmd = NULL;
//     ast->redircmd = NULL;
// }


extern char **environ;

// void	set_env(t_list **env_list)
// {

// 	*env_list = cp_env_list();
// 	ft_getenv = getenv;
// 	return ;
// }

int main(void) 
{
    t_list  *env_list;
    char *input;
    char **tok;
    char *ptr;
    t_cmd *ast;
    int i;
 
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    ast = NULL;
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
        if (input && *input)
        {
            add_history(input);
            tok = get_tokens(input);
            i = 0;
            while (tok[i] != NULL)
            {
                ptr = tok[i];
                tok[i] = expansion_handling(tok[i]);
                if (ptr != tok[i])
                    free(ptr);
                ptr = tok[i];
                tok[i] = quote_handling(tok[i]);
                if (ptr != tok[i])
                    free(ptr);
                // printf("%s\n", tok[i]);
                i++;
            }
            ast = parse_cmd(tok);
            // print_command_tree(ast, env_list); //print abstract syntax tree from root
            iterate_ast(ast, env_list);
            free_ast(ast);
            free_tokens(tok);
        }
        free(input);
    }
    return 0;
}

// cat <<|EOF | grep "hello" -> test this case
// cat <<EOF | grep "hello"
// cmd < filename1 | cmd1 > filename2
// cmd 1 | cmd 2 | cmd 3
// e'c'ho |"$PATH" << '$USER' "Hello,>> '$USER' is me" 
// sort < Makefile| grep "FLAG" | grep 'FLAGS' > output.txt
// sort < Makefile | grep "FLAG" | grep 'FLAGS' >> output.txt
// sort < Makefile | grep "FLAG" | uniq > output.txt
// cat <<EOF | grep "pattern" > output.txt
// grep 'pattern' << EOF > output.txt
// echo "'$USER'" $PATH '$USER' $?
