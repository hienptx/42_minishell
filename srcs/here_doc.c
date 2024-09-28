#include "minishell.h"

int heredoc_process(char *arg)
{
	char *str;
    int hd_fd;

    hd_fd = open("here_doc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (hd_fd < 0)
    {
        panic_sms("Open failed");
    }
	while(1)
	{
		ft_putstr_fd("> ", 0);
		str = get_next_line(0);
		if (str != NULL)
		{
			if (ft_strncmp(str, arg, ft_strlen(arg)) == 0 && str[ft_strlen(arg)] == '\n')
			{
				free(str);
				break;
			}
			ft_putstr_fd(str, hd_fd);
			free(str);
		}
	}
    close(hd_fd);   
    return(hd_fd);
}

t_cmd *parse_here_doc(t_cmd *command, char **tokens, int i)
{
    char *file_name;
    int fd;
    
    if (tokens[i + 1] == NULL)
        panic_sms("Bad syntax: missing heredoc delimiter");
    file_name = tokens[i + 1]; // File name is the delimiter
    fd = heredoc_process(file_name); // Process the heredoc input
    if (fd < 0)
        panic_sms("Error processing heredoc");
    free(tokens[i]);
    tokens[i] = NULL; // Set "<<"
    free(tokens[i + 1]);
    tokens[i + 1] = NULL; // Set heredoc delimiter
    if (!command)
        command = parse_exec(tokens); // Parse the command if it's not done yet
    if (command)
        command = construct_redir(command, fd, "here_doc.txt"); // Redirect stdin (fd 0) to heredoc file
    return (command);
}
