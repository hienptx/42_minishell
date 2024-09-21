#include "minishell.h"

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
    free(str);
    return ptr;
}

char *quote_handling(char *token)
{
    char *squote_str;
    char *ptr;

    ptr = token;
    squote_str = ft_strchr(token, '\''); 
    if (squote_str != NULL && ft_strcmp(squote_str, token) == 0)
    {
        token = ft_strtrim(token, "\'\'");
        free(ptr);
    }
    else if (ft_strchr(token, '\"') != NULL)
    {
        token = ft_strtrim(token, "\"\"");
        free(ptr);
    }
    else
    {
        ;        
    }
    return(token);
}

int main(void) 
{
    char *input;
    char **tok;
    t_cmd *ast;
    int i;
 
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    ast = NULL;
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
                tok[i] = expansion_handling(tok[i]);
                tok[i] = quote_handling(tok[i]);
                printf("%s\n", tok[i]);
                i++;
            }
            ast = parse_cmd(tok);
            print_command_tree(ast, 0); //print abstract syntax tree from root
            free_tokens(tok);
        }
        free(input);
    }
    return 0;
}

// cat <<EOF | grep "hello"
// cmd < infile > outfile
// cmd 1 | cmd 2 | cmd 3
// echo |"$PATH" << '$USER' "Hello,>> '$USER' is me" 
// sort < Makefile| grep "FLAG" | grep 'FLAGS' > output.txt
// sort < Makefile | grep "FLAG" | grep 'FLAGS' >> output.txt
// sort < Makefile | grep "FLAG" | uniq > output.txt
// cat <<EOF | grep "pattern" > output.txt
// grep 'pattern' << EOF > output.txt
// echo "'$USER'" $PATH '$USER' $?