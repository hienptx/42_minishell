#include "minishell.h"

// int heredoc_process(char *arg)
// {
// 	char *str;
//     int hd_fd;

//     hd_fd = open("here_doc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (hd_fd < 0)
//     {
//         panic_sms("Open failed");
//     }
// 	while(1)
// 	{
// 		ft_putstr_fd("> ", 0);
// 		str = get_next_line(0);
// 		if (str != NULL)
// 		{
// 			if (ft_strncmp(str, arg, ft_strlen(arg)) == 0 && str[ft_strlen(arg)] == '\n')
// 			{
// 				free(str);
// 				break;
// 			}
// 			ft_putstr_fd(str, hd_fd);
// 			free(str);
// 		}
// 	}
//     close(hd_fd);   
//     return(hd_fd);
// }

void *ft_realloc(void *ptr, size_t new_size)
{
    size_t old_size;
    size_t size_to_copy;
    void *new_ptr;

    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }
    if (ptr == NULL)
    {
        ptr = malloc(new_size);
        if (ptr == NULL)
            panic_sms("Malloc failed");
        return (ptr);
    }
    old_size = ft_strlen(ptr);
    new_ptr = malloc(new_size + 1);
    if (new_ptr == NULL)
        panic_sms("Malloc failed");
    if(old_size > new_size)
        size_to_copy = old_size;
    else
        size_to_copy = new_size;
    ft_memcpy(new_ptr, ptr, size_to_copy);
    return (new_ptr);
}

char *store_input(char *str, char *buffer, size_t *total_size)
{
    size_t len;
    char *new_buffer;

    len = ft_strlen(str);
    new_buffer = ft_realloc(buffer, *total_size + len + 1);
    if (!new_buffer)
    {
        free(buffer);
        panic_sms("Realloc failed");
    }
    buffer = new_buffer;
    ft_strcpy(buffer + *total_size, str);
    *total_size += len;
    free(str);
    return(buffer);
}

int heredoc_process(char *arg)
{
    char *str;
    char *buffer;
    // size_t buffer_size;
    size_t total_size;
    int pipe_fd[2];

    // buffer_size = 0;
    total_size = 0;
    buffer = NULL;
    if (pipe(pipe_fd) == -1)
        panic_sms("Pipe failed");
    while (1)
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
            buffer = store_input(str, buffer, &total_size);
        }
    }
    int i = 0;
    while(buffer[i] != '\0')
    {
        write(1, &buffer[i], 1);
        i++;
    }
    write(pipe_fd[1], buffer, total_size);
    close(pipe_fd[1]);
    free(buffer); 
    return (pipe_fd[0]); 
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
    // printf("hd_fd = %i\n", fd);
    free(tokens[i]);
    tokens[i] = NULL; // Set "<<"
    free(tokens[i + 1]);
    tokens[i + 1] = NULL; // Set heredoc delimiter
    if (!command)
        command = parse_exec(tokens); // Parse the command if it's not done yet
    if (command)
        command = construct_redir(command, fd, "here_doc"); // Redirect stdin (fd 0) to heredoc file
    return (command);
}
