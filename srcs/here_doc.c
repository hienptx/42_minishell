#include "minishell.h"

int heredoc_process(char *arg)
{
	char *str;
	int		i;
    int hd_fd;

    hd_fd = open("../minishell/here_doc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(hd_fd < 0)
    {
        panic_sms("Open failed");
    }
	i = ft_strlen(arg);
	while(1)
	{
		ft_putstr_fd("> ", 0);
		str = get_next_line(0);
		if (str != NULL)
		{
			if (ft_strncmp(str, arg, i) == 0 && str[i] == '\n')
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
    {
        printf("Bad syntax: missing heredoc delimiter\n");
        exit(1);
    }
    file_name = tokens[i + 1]; // File name is the delimiter
    fd = heredoc_process(file_name); // Process the heredoc input
    if (fd < 0)
    {
        printf("Error processing heredoc\n");
        exit(1);
    }
    tokens[i] = NULL; // Set "<<"
    tokens[i + 1] = NULL; // Set heredoc delimiter
    if (!command)
        command = parse_exec(tokens); // Parse the command if it's not done yet
    if (command)
        command = construct_redir(command, fd, "here_doc.txt"); // Redirect stdin (fd 0) to heredoc file
    return (command);
}