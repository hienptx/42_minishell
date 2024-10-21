/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:21:36 by hipham            #+#    #+#             */
/*   Updated: 2024/10/21 16:30:26 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_process(char *arg)
{
	char	*str;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		panic_sms("Pipe failed", 1);
	while (1)
	{
		ft_putstr_fd("> ", STDIN_FILENO);
		str = get_next_line(STDIN_FILENO);
		if (str != NULL)
		{
			if (ft_strncmp(str, arg, ft_strlen(arg)) == 0
				&& str[ft_strlen(arg)] == '\n')
			{
				free(str);
				break ;
			}
			write(pipe_fd[1], str, ft_strlen(str));
		}
		free(str);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

t_cmd	*parse_here_doc(t_cmd *command, char **tokens, int i, char **args)
{
	char	*delimiters;
	int		fd;

	if (tokens[i + 1] == NULL)
		panic_sms("syntax error near unexpected token `newline'", 2);
	delimiters = tokens[i + 1];
	free(tokens[i]);
	tokens[i] = NULL;
	fd = heredoc_process(delimiters);
	if (fd < 0)
		panic_sms("Error processing heredoc", 2);
	if (command == NULL)
		command = construct_redir(args, fd, "here_doc");
	else
		command->cmd.redir = append_redir_list(command->cmd.redir, args,
				"here_doc", fd);
	return (command);
}

size_t	count_operator_tokens(char **str)
{
	if (ft_strncmp(*str, ">>", 2) == 0 || ft_strncmp(*str, "<<", 2) == 0
		|| ft_strncmp(*str, "||", 2) == 0)
	{
		*str += 2;
		return (1);
	}
	return (0);
}
