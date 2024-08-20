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

// echo "Path is '$PATH'" | grep -o "/usr[^']*" > path_output.txt
// cat 'my file.txt' | grep "$USER" >> "$HOME/user_log.txt"


int main(void) 
{
    char *input;
    char *tok;

    signal(SIGINT, signal_handler);  // Set the SIGINT (Ctrl+C) signal handler
    signal(SIGQUIT, signal_handler); // Set the SIGINT (Ctrl+\) signal handler
    while(1)
    {
        input = readline("$> "); // Read input from the user
        if (input && *input)
        {
            add_history(input); // Add input to the history
            tok = strtok(input, " |><"); // TODO: FREE TOKENS
            while (tok != NULL)
            {
                printf("%s\n", tok);
                tok = strtok(NULL, " |<>");
            }
        }
        if (input == NULL)
        {
            perror("EOF, exiting shell");
            break;
        }
        free(input); // Free the input buffer after use
    }
    return 0;
}