/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:16:10 by hipham            #+#    #+#             */
/*   Updated: 2024/10/22 20:56:36 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/includes/libft.h"

int		echo(char **x);
int		pwd(void);
int		export(t_list *env_list, char **x);
int		cd(t_list *env_list, char **x);
int		unset(char **args, t_list *env_list);
int		env(t_list *env_list);
int		change_dir(t_list *env_list, const char *path, const char *oldpwd);
void	print_env_arr(char **env_arr);
char	**mk_env_list(t_list *env_list);
// char *(*ft_getenv)(const char *key);
t_list	*cp_env_list(void);
int		cd_external(const char *path, char **cwd);

char	*get_env_key(char *env);
char	*get_env(char *env_key, t_list *env_list);
char	*get_env_value(char *env_key, t_list *env_list);
char	*get_key_or_value(char *key_or_val, char *env, size_t key_len);

int		update_env(t_list *env_list, const char *key, char *new_env);
int		add_env(t_list **env_list, char *env);
void	mod_env(t_list *env_list, const char *new_env);
int		rm_env(t_list **env_list, const char *key_to_remove);
void	rm_node(t_list **env_list, t_list **prev, t_list *cur);
#endif