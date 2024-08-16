#include "minishell.h"

// Ctrl-D End of Transmission
// Ctrl-C Signal Interrupt - SIGINT
// Ctrl-\ Quit - SIGQUIT

int main(void) 
{
    char *input;

    while(1)
    {
        // Read input from the user
        input = readline("Prompt> ");
        if (input && *input) 
        {
            add_history(input); // Add input to the history
            // rl_replace_line("New input\n", 1); // Replace the current line with "New input"
            // rl_on_new_line(); // Signal that we are on a new line
            // rl_redisplay(); // Redisplay to show the updated input
        }
        else
        {
            perror("Exiting shell");
            exit(1);
        }
        printf("%s\n", input);
    }
    free(input);     // Free the input buffer
    return 0;
}