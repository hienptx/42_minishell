/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:59:22 by hipham            #+#    #+#             */
/*   Updated: 2024/10/08 19:36:36 bcdy hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char	**environ;

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 1); // Replace current input with empty string
		rl_on_new_line();       // Signal that we are on a new line
		printf("\n");
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		;
	}
}
t_cmd	*process_tokens(char **tok, t_param *param, t_parse_data parse)
{
	int		i;
	char	*ptr;
	t_cmd	*ast;

	ast = NULL;
	i = 0;
	while (tok[i] != NULL)
	{
		// printf("%s\n", tok[i]);
		ptr = tok[i];
		tok[i] = expansion_handling(tok[i], param);
		if (ptr != tok[i])
			free(ptr);
		ptr = tok[i];
		tok[i] = quote_handling(tok[i]);
		if (ptr != tok[i])
			free(ptr);
		i++;
	}
	ast = parse_cmd(tok);
	if (ast == NULL)
		return (NULL);
	parse.ast = ast;	
	iterate_ast(ast, param, parse);
	return (ast);
}

void	init_param(t_param *param)
{
	param->env_list = NULL;
	param->special.question_mark = 0;
	set_env(&param->env_list);
	return ;
}

void	process_input(char *input, t_param *param)
{
	t_parse_data	parse;
	// char	**tok;
	// t_cmd	*ast;
	// size_t	nbr_tokens;

	parse.input = input;
	parse.ast = NULL;
	if (input && *input)
	{
		add_history(parse.input);
		parse.tok = get_tokens(parse.input, &parse.nbr_tokens);
		if (parse.tok == NULL)
		{
			free(parse.input);
			return ;
		}
		if (unclosed_quote(parse.tok) || check_syntax(parse.tok))
		{
			free_tokens(parse.tok, parse.nbr_tokens);
			free(parse.input);
		}
		else
		{
			parse.ast = process_tokens(parse.tok, param, parse);	//pass ast, nbr_tokens, input
			if (parse.ast)
				free_ast(parse.ast);
			free_tokens(parse.tok, parse.nbr_tokens);
			free(parse.input);
		}
	}
}

void	shell_loop(t_param *param)
{
	char	*input;

	while (1)
	{
		input = readline("$> ");
		if (input == NULL)
		{
			printf("EOF, exiting shell\n");
			exit(0);
		}
		if (only_space(input))
		{
			free(input);
			continue ;
		}
		process_input(input, param);
	}
}

int	main(void)
{
	t_param	param;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	init_param(&param);
	shell_loop(&param);
	ft_lstclear(&param.env_list, free);
	return (0);
}

// int	main(void)
// {
// 	t_param	param;
// 	size_t	nbr_tokens;
// 	char	*input;
// 	char	**tok;
// 	t_cmd	*ast;

// 	signal(SIGINT, signal_handler);
// 	signal(SIGQUIT, signal_handler);
// 	init_param(&param);
// 	ast = NULL;
// 	nbr_tokens = 0;
// 	while (1)
// 	{
// 		input = readline("$> ");
// 		if (input == NULL) // Check for Ctrl-D (EOF)
// 			panic_sms("EOF, exiting shell", 2);
// 		if (only_space(input))
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		if (input && *input)
// 		{
// 			add_history(input);
// 			tok = get_tokens(input, &nbr_tokens);
// 			if (tok == NULL)
// 			{
// 				free(input);
// 				continue ;
// 			}
// 			if (unclosed_quote(tok) || check_syntax(tok))
// 			{
// 				free_tokens(tok, nbr_tokens);
// 				free(input);
// 			}
// 			else
// 			{
// 				ast = process_tokens(tok, &param);
// 				if (ast == NULL)
// 				{
// 					free_tokens(tok, nbr_tokens);
// 					free(input);
// 					continue ;
// 				}
// 				free_ast(ast);
// 				free_tokens(tok, nbr_tokens);
// 			}
// 			continue ;
// 		}
// 		free(input);
// 	}
// 	return (0);
// }
