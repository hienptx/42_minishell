#ifndef BUILTIN_H
# define BUILTIN_H

#include "includes/minishell.h"

int	echo(char **x);
int	pwd(char **x);
int	export(t_list *env_list, const char **x);
int	cd(t_list *env_list, char **x);
int	change_dir(t_list *env_list, const char *path, const char *oldpwd);
void print_env_arr(char **env_arr);
char **mk_env_list(t_list *env_list);
char *(*ft_getenv)(const char *key);

char	*get_env_key(const char *env_var);
char	*get_env_value(const char *env);
char	*get_key_or_value(char *key_or_val, const char *env);
int		update_env(t_list *env_list, const char *key, const char *new_env);
int		add_env(t_list **env_list, const char *env);
void	mod_env(t_list *env_list, const char *new_env);
int		rm_env(t_list **env_list, const char *key_to_remove);
void	rm_node(t_list **env_list, t_list **prev, t_list **cur);
#endif