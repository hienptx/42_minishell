/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:21:36 by hipham            #+#    #+#             */
/*   Updated: 2024/10/03 16:33:31 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int heredoc_process(char *arg)
{
    char *str;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
        panic_sms("Pipe failed");
    while (1)
    {
        ft_putstr_fd("> ", STDIN_FILENO);
        str = get_next_line(STDIN_FILENO);
        if (str != NULL)
        {
            if (ft_strncmp(str, arg, ft_strlen(arg)) == 0 && str[ft_strlen(arg)] == '\n')
            {
                free(str);
                break;
            }
            write(pipe_fd[1], str, ft_strlen(str));
        }
        free(str);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]); 
}

t_cmd *parse_here_doc(t_cmd *command, char **tokens, int i)
{
    char *delimiters;
    int fd;
    
    if (tokens[i + 1] == NULL)
        panic_sms("Bad syntax: missing heredoc delimiter");
    delimiters = tokens[i + 1]; // File name is the delimiter
    free(tokens[i]);
    tokens[i] = NULL;
    fd = heredoc_process(delimiters);
    if (fd < 0)
        panic_sms("Error processing heredoc");
    if (command == NULL)
        command = construct_redir(tokens, fd, "here_doc");
    else
        command->cmd.redir = append_redir_list(command->cmd.redir, tokens, "here_doc", fd);
    return (command);
}
