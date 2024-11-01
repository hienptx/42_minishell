/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:35:49 by hipham            #+#    #+#             */
/*   Updated: 2024/10/25 21:52:26 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/builtin.h"
# include "../libft/includes/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum cmd_type
{
	EXEC,
	REDIR,
	PIPE
}					t_type;

typedef struct exec
{
	char			**arg;
}					t_exec;

typedef struct redir
{
	t_exec			*cmd;
	char			*file_name;
	int				fd;
	struct redir	*next;
}					t_redir;

typedef struct pipe
{
	void			*left;
	void			*right;
}					t_pipe;

typedef struct s_cmd
{
	t_type			type;
	union
	{
		t_pipe		*pipe;
		t_redir		*redir;
		t_exec		*exec;
	} u_cmd;
}					t_cmd;

typedef struct s_parse_data
{
	t_cmd			*ast;
	char			**tok;
	size_t			nbr_tokens;
	char			*input;
}					t_parse_data;

typedef struct s_special_param
{
	int				question_mark;
}					t_special_param;

typedef struct s_param
{
	t_list			*env_list;
	t_special_param	special;
}					t_param;

// minishell.c
void				signal_handler(int sig);
t_cmd				*process_tokens(char **tok, t_param *param,
						t_parse_data parse);

// expansion.c
char				*expansion_handling(char *str, t_param *param);
char				*replace_substring(char *str, char *newsub, char *oldsub);
char				*expand_str(char *str, t_param *param);

// quotes.c
char				*quote_handling(char *token);
size_t				get_strlen(char *token);
int					unclosed_quote(char **token);

// tokenizer.c
char				**get_tokens(char *str, size_t *nbr_tokens);
char				*cpy_str(char **ret, char *str, size_t *pos);
size_t				count_tokens(char *str);

// minishell_utils.c
void				*ft_malloc(void *data, size_t size);
void				*panic_sms(char *s, int i);
int					only_space(char *input);
int					get_fd(char *token);
size_t				count_non_operators(char **tokens);
int					update_env(t_list *env_list, const char *key,
						char *new_env);

// constructor.c
t_cmd				*construct_pipe(char *left, char *right);
t_cmd				*construct_redir(char **tokens, int fd, char *fn);
t_redir				*append_lst(t_redir *ast, char **tokens, char *filename,
						int fd);
t_exec				*construct_exec(char **tokens, t_exec *data);
char				**mk_env_list(t_list *env_list);

// parser.c
t_cmd				*parse_cmd(char **tokens);
t_cmd				*parse_redir(char **tokens);
t_cmd				*parse_exec(char **tokens);
void				print_command_tree(t_cmd *cmd, int level);
void				iterate_ast(t_cmd *cmd, t_param *param, t_parse_data parse);

// here_doc.c
int					heredoc_process(char *arg2);
t_cmd				*parse_here_doc(t_cmd *command, char **tokens, int i,
						char **args);
size_t				count_operator_tokens(char **str);

// pipe.c
void				left_pipe(t_pipe *pipe_cmd, int *fd, t_param *param,
						t_parse_data parse);
void				right_pipe(t_pipe *pipe_cmd, int *fd, t_param *param,
						t_parse_data parse);
int					set_pipe(t_pipe *pipe_cmd, t_param *param,
						t_parse_data parse);

// redirect.c
void				set_redir(t_redir *redir_cmd, t_param *param,
						t_parse_data parse);
int					get_file_fd(t_redir *redir_cmd);
int					dup_fd(t_redir *redir_cmd);

// execution.c
int					call_exec(t_exec *exec_cmd, t_list *env_list,
						t_parse_data parse, char *file);
void				run_exec(t_exec *exec_cmd, t_param *param,
						t_parse_data parse);
int					set_exec(t_exec *exec_cmd, t_param *param,
						t_parse_data parse);
char				*get_executable_path(char *env_path, char *prog_name);
char				**get_all_path(char *env_path);

void				set_env(t_list **env_list);
int					ck_builtin(char *executable_name);
int					find_env(t_list *env_list, char *key);
int					call_builtin(t_exec *exec_cmd, t_param *param,
						t_parse_data parse);

// ft_strsjoin.c
char				*ft_strsjoin(const char *delimiter, ...);
int					get_full_len(va_list args, const char *delimiter);
char				*cp_strs(char *result, va_list args, const char *delimiter);
char				**get_args(char **tokens);
size_t				get_len(char c);

// free.c
void				free_tokens(char **tokens, size_t nbr_tokens);
void				free_parse(t_parse_data parse);
void				init_param(t_param *param);
void				free_ast(t_cmd *ast);

// syntax_check.c
int					check_syntax(char **tokens);
int					is_operator(const char *token);
int					invalid_syntax_sms(char *s);

// builtin_change_env.c
int					add_env(t_list **env_list, char *new_env);
void				mod_env(t_list *env_list, const char *new_env);
void				rm_node(t_list **env_list, t_list **prev, t_list *cur);
int					rm_env(t_list **env_list, const char *key_to_remove);

// builtin_get_env.c
char				*get_env_key(char *env);
char				*get_env_value(char *env_key, t_list *env_list);
char				*get_key_or_value(char *key_or_val, char *env,
						size_t key_len);
char				*get_env(char *env_key, t_list *env_list);
int					find_env(t_list *env_list, char *key);

// execution_utils.c
char				**get_all_path(char *env_path);
char				*get_full_path(t_exec *exec_cmd, t_list *env_list);
int					is_executable(const char *path);
char				*get_executable_path(char *env_path, char *prog_name);
void				ft_exit(char *sms, int ret, t_param *param,
						t_parse_data parse);
// qsort.c
void				quick_sort(char **arr, int low, int high);
int					partition(char **arr, int low, int high);
void				get_exit_param(t_param *param, t_parse_data parse,
						char *args1, char *args2);
int					export_syntax_valid(char *s);
// builtin_sort_env.c
void				display_sorted_env(t_list *env_list);
char				**mk_env_list_cp(t_list *env_list);
void				free_pre_allocated(char **env_arr, size_t i);
size_t				split_len(char **split);

#endif
