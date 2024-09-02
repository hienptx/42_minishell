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
// Parsecmd - recursive descent parser
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

void replace_substring(char *str, char *newsub, char *oldsub)
{
    char *pos;
    size_t oldlen;
    size_t newlen;
    size_t taillen;

    pos = ft_strstr(str, oldsub);
    if (pos == NULL)
        return ;
    oldlen = ft_strlen(oldsub);
    newlen = ft_strlen(newsub);
    taillen = ft_strlen(pos + oldlen);
    if (newlen != oldlen)
        ft_memmove(pos + newlen, pos + oldlen, taillen + 1);
    ft_memcpy(pos, newsub, newlen);
}

// expansion_handling $ -> use getenv
char *expansion_handling(char *str)
{
    char *ret_env;
    char *s;
    char *path;
    size_t len;
    unsigned int start;

    len = 0;
    s = ft_strchr(str, '$'); // TODO -> FREE
    start = s - str;
    while (*s != '\0' && *s != ' ' && *s != '\'' && *s != '\"')
    {
        len++;
        s++;
    }
    path = ft_substr(str, start, len); //TODO -> FREE
    if (getenv(path + 1) != NULL)
        ret_env = getenv(path+ 1);
    else
        ret_env = "";
    if (ft_strchr(str, '\"') != NULL)
        replace_substring(str, ret_env, path);
    else
        replace_substring(str, ret_env, path);
    free (path);
    // free(ret_env);
    return (str);
}   

void quote_handling(char **token)
{
    char *ptr;
    char *squote_str;
    int i;
    
    i = 0;
    while (token[i] != NULL)
    {
        ptr = token[i];
        squote_str = ft_strchr(token[i], '\''); 
        if (squote_str != NULL && ft_strcmp(squote_str, token[i]) == 0)
        {
            token[i] = ft_strtrim(token[i], "\'\'");
            free(ptr);
        }
        else if (ft_strchr(token[i], '\"') != NULL)
        {
            token[i] = ft_strtrim(token[i], "\"\"");
            if (ft_strchr(token[i], '$') != NULL)
            {
                token[i] = expansion_handling(token[i]);
                free(ptr);
            }
        }
        else
        {
            ;        
        }
        i++;
    }
}

int main(void) 
{
    char *input;
    char **tok;

    signal(SIGINT, signal_handler);  // Set the SIGINT (Ctrl+C) signal handler
    signal(SIGQUIT, signal_handler); // Set the SIGINT (Ctrl+\) signal handler
    while(1)
    {
        input = readline("$> "); // Read input from the user
        if (input == NULL) // Check for Ctrl-D (EOF)
        {
            printf("EOF, exiting shell\n");
            break; // Exit the shell loop
        }
        if (input && *input)
        {
            // printf("%li\n", count_tokens(input));
            add_history(input); // Add input to the history
            tok = trim_cmd(input);
            int i = 0;
            quote_handling(tok);
            while (tok[i] != NULL)
                printf("%s\n", tok[i++]);

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

// // cat Makefile>>grep 'hello'|wc -l 
// Node 1        cat Makefile | grep 'hello'
// Node 2        grep 'hello'| wc -l
// ....

// ls -l

// echo |"$USER" <'$USER' "Hello,>> '$USER' is me" 