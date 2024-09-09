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

void parse_token(char **tok)
{
    t_redir **redirs;
    int i;
    int redir_idx;
    int nbr_redir;
    int nbr_pipes;

    i = 0;
    redir_idx = 0;
    nbr_pipes = 0;
    while (tok[i] != NULL)
    {
        if (ft_strcmp(tok[i], "<") == 0 || ft_strcmp(tok[i], ">") || ft_strcmp(tok[i], ">>") == 0)
        {
            redirs[redir_idx] = populate_struct_cmd(tok, i);
            redir_idx++;
        }
        else if (ft_strcmp(tok[i], "|") == 0)
        {
            
        }


    }
    populate_struct_redir(tok);
}

int main(void) 
{
    char *input;
    char **tok;
    int i;
 
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
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
            // printf("%li\n", count_tokens(input));
            add_history(input); // Add input to the history
            tok = get_tokens(input);
            i = 0;
            while (tok[i] != NULL)
            {
                tok[i] = expansion_handling(tok[i]);
                tok[i] = quote_handling(tok[i]);
                printf("%s\n", tok[i]);
                i++;
            }
            parse_tokens(tok);
            free_tokens(tok);
        }
        free(input);
    }
    return 0;
}

// cmd < infile > outfile

//         echo    "Path is '$PATH'"   ''          |           grep -o       "/usr[^']*"        >           path_output.txt
// left   1.node   2.node           3.skip     int type 2
// right                                                    1.node            2.node          

//                                                                   command              int type 3           file_name        
//                                                                   fd = open("output file"), flag=O_WRONLY, O_CREAT
// // cat "'my file.txt'" | grep 'hello "$USER"' >> "$HOME/user_log.txt" "'$PATH'/user_log.txt"

// // cat Makefile >> grep 'NAME' | wc -l 
// Node 1        cat Makefile | grep 'hello'
// Node 2        grep 'hello'| wc -l
// ....

// ls -l

// echo |"$PATH" << '$USER' "Hello,>> '$USER' is me" 
// sort < Makefile | grep "FLAG" | uniq > output.txt
// sort < Makefile| grep "FLAG" | grep 'FLAGS' > output.txt
// sort < Makefile | grep "FLAG" | grep 'FLAGS' >> output.txt
// cat <<EOF | grep "pattern" > output.txt
// grep 'pattern' << EOF > output.txt
// echo "'$USER'" $PATH '$USER' $?