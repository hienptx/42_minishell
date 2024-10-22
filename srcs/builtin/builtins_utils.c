/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:23:02 by hipham            #+#    #+#             */
/*   Updated: 2024/10/22 13:25:51 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtin.h"
#include "../../includes/minishell.h"

extern char	**environ;

void	print_env_arr(char **env_arr)
{
	size_t	i;

	i = 0;
	while (env_arr[i] != NULL)
	{
		printf("%s\n", env_arr[i]);
		i++;
	}
}

int	change_dir(t_list *env_list, const char *path, const char *oldpwd)
{
	char	*cwd;
	char	*env_oldpwd;
	char	*env_pwd;
	int		ret_external;

	ret_external = cd_external(path, &cwd);
	if (ret_external != 0)
		return (ret_external);
	env_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
	env_pwd = ft_strjoin("PWD=", cwd);
	if (env_pwd == NULL || env_oldpwd == NULL)
		panic_sms("malloc failed", 1);
	if (update_env(env_list, "OLDPWD", env_oldpwd) != 0 || update_env(env_list,
			"PWD", env_pwd) != 0)
		return (1);
	free(cwd);
	cwd = NULL;
	return (0);
}

t_list	*cp_env_list(void)
{
	char	**env;
	t_list	*env_list;
	t_list	*new_node;
	char	*env_malloc;

	env_list = NULL;
	env = environ;
	while (*env)
	{
		env_malloc = ft_strdup(*env);
		new_node = ft_lstnew(env_malloc);
		if (new_node == NULL || env_malloc == NULL)
			panic_sms("malloc", 1);
		ft_lstadd_back(&env_list, new_node);
		env++;
	}
	return (env_list);
}

int	env(t_list *env_list)
{
	while (env_list != NULL)
	{
		printf("%s\n", (char *)env_list->content);
		env_list = env_list->next;
	}
	return (0);
}
