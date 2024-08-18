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
// runcmd - walk the tree recursively. "execute" the nodes, create child processes as needed

// Signal handler for SIGINT (^C)

void signal_handler(int sig)
{
    (void)sig;
    // Signal that we are on a new line
    rl_on_new_line();
    printf("\n");
    // Redisplay to show the updated (empty) input prompt
    rl_redisplay();
}

int main(void) 
{
    char *input;

    // Set the SIGINT (Ctrl+C) signal handler
    signal(SIGINT, signal_handler);

    while(1)
    {
        // Read input from the user
        input = readline("Prompt> ");
        if (input && *input)
        {
            add_history(input); // Add input to the history
            printf("%s\n", input); // Echo the input
        }
        else
        {
            perror("Exiting shell");
            exit(1);
        }
        free(input); // Free the input buffer after use
    }
    return 0;
}