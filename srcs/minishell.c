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

size_t count_tokens(char *str)
{
    int count = 0;
    char *ptr = str;
    char quote;

    while (*ptr != '\0')
    {
        while (*ptr != '\0' && *ptr == ' ')
            ptr++;
        if (*ptr == '"' || *ptr == '\'')
        {
            quote = *ptr;
            ptr++; // Skip the opening quote
            while (*ptr != '\0' && *ptr != quote)
                ptr++;
            if (*ptr == quote)
                ptr++; // Skip the closing quote
        }
        else
        {
            while (*ptr != '\0' && *ptr != ' ' && *ptr != '"' && *ptr != '\'')
                ptr++;
        }
        count++;
    }
    return count;
}

char *cpy_str(char **ret, char *str, size_t *pos)
{
    char quote;
    char *ptr = str;
    size_t wordlen;

    while (*str != '\0' && *str == ' ')
        str++;
    ptr = str;
    // Handle quoted tokens
    if (*str == '"' || *str == '\'')
    {
        quote = *str;
        str++;  // Skip the opening quote
        while (*str != '\0' && *str != quote)
            str++;
        if (*str == quote)
            str++;  // Include the closing quote
        wordlen = str - ptr;  // Calculate the word length including quotes
        ret[*pos] = ft_malloc(ret[*pos], wordlen);
        ret[*pos][wordlen] = '\0';
        ft_strncpy(ret[*pos], ptr, wordlen);
    }
    else
    {
        // Handle unquoted tokens
        while (*str != '\0' && *str != ' ')
            str++;
        wordlen = str - ptr;  // Calculate the word length
        ret[*pos] = ft_malloc(ret[*pos], wordlen);  // Allocate memory
        ret[*pos][wordlen] = '\0';  // Null-terminate the string
        ft_strncpy(ret[*pos], ptr, wordlen);
    }
    *pos += 1;
    return str;
}

char **trim_cmd(char *str)
{
    char **ret;
    char *ptr;
    size_t nbr_tokens;
    size_t pos;

    ptr = str;
    nbr_tokens = count_tokens(str);
    ret = malloc((nbr_tokens + 1) * sizeof(char *));
    if (ret == NULL)
    {
        perror("Malloc failed to allocate memory\n");
        return NULL;
    }
    pos = 0;
    while (pos < nbr_tokens)
        ptr = cpy_str(ret, ptr, &pos);
    ret[nbr_tokens] = NULL;
    return (ret);
}

 // expansion_handling $ -> use getenv

void quote_handling(char **token)
{
    // char *ret_env;
    // char **tokens_arr;
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
            // printf("%s\n", ft_strchr(token[i], '\''));
            token[i] = ft_strtrim(token[i], "\'\'");
            free(ptr);
        }
        else if (ft_strchr(token[i], '\"') != NULL)
        {
            token[i] = ft_strtrim(token[i], "\"\"");
            // expansion_handling $
            // if (ft_strchr(token[i], '$') != NULL)
            //     token[i] = expansion_handling(token[i]);
            free(ptr);
        }
        // exp: cmd|cmd infile>
        // else if  (| < > >>)
        else
        {
            ;        
        }
        i++;
    }
    // return (token);
}

int main(void) 
{
    char *input;
    char **tok;

    signal(SIGINT, signal_handler);  // Set the SIGINT (Ctrl+C) signal handler
    signal(SIGQUIT, signal_handler); // Set the SIGINT (Ctrl+\) signal handler
    char *ret_env = getenv("PATH");
    if (ret_env != NULL)
        printf("%s\n", ret_env);
    else
        printf("\n");
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
            add_history(input); // Add input to the history
            tok = trim_cmd(input);
            int i = 0;
            quote_handling(tok);
            while (tok[i] != NULL)
            {
                printf("%s\n", tok[i]);
                i++;
            }
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
// // cat "'my file.txt'" | grep 'hello "$USER"' >> "$HOME/user_log.txt"

// // cat Makefile | grep 'hello'| wc -l 
// Node 1        cat Makefile | grep 'hello'
// Node 2        grep 'hello'| wc -l
// ....

// ls -l