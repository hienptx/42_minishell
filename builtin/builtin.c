#include "builtin.h"
#include "includes/minishell.h"

extern char **environ;

t_list	*cp_env_list()
{
	char **env = environ;
	t_list	*env_list;
	t_list	*new_node;
	char	*env_malloc;
	int		i;

	i = 0;
	env_list = NULL;
	while (*env)
	{
		env_malloc = ft_strdup(*env);
		new_node = ft_lstnew(env_malloc);
		if (new_node == NULL || env_malloc == NULL)
			return (NULL);
		ft_lstadd_back(&env_list, new_node);
		env++;
	}
	return (env_list);
}

void print_env_arr(char **env_arr)
{
	size_t	i;

	i = 0;
	while (env_arr[i] != NULL)
	{
		printf("%s\n", env_arr[i]);
		i++;
	}
}

int main()
{
	t_list	*env_list;

	env_list = cp_env_list();
	ft_getenv = getenv;
	char	*cmd = ft_strdup("cd");
	char	*arg = ft_strdup("./tmp");
	// char *cd_arg[] = {cmd, arg, NULL};
	char *cd_arg[] = {cmd, NULL};
	cd(env_list, cd_arg);
	cmd = ft_strdup("export");
	arg = ft_strdup("e=r");
	char	*arg1 = ft_strdup("e=r");
	char	*arg2 = ft_strdup("o=i");
	char	*arg4 = ft_strdup("e=pp");
	char	*arg3 = ft_strdup("b");
	char *new_env[] = {cmd, arg, arg1, arg2, arg3, arg4, NULL};
	export(env_list, new_env);
	char **envp = mk_env_list(env_list);
	print_env_arr(envp);
	char *argv[] = {"/bin/bash", NULL};	//diff char *[], char **
	execve("/bin/bash", argv, envp);
    return 0;
}

int	cd_external(const char *path, char **cwd)
{
	if (chdir(path) == -1)
	{
		perror("chdir");
		return (-1);
	}
	*cwd = getcwd(NULL, 0);
	if (*cwd == NULL)
		return (2);
	return (0);
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
		return (1);
	if (update_env(env_list, "OLDPWD", env_oldpwd) != 0 || \
		update_env(env_list, "PWD", env_pwd) != 0)
		return (1);
	free(cwd);
	cwd = NULL;
	return (0);
}

int	cd(t_list *env_list, char *x[]) //{"cd", "tmp", NULL}
{
	const char	*path;
	char		*oldpwd;

	path = x[1];
	if (path == NULL)	//just 'cd'
		path = ft_getenv("HOME");
	if (path)
	{
		oldpwd = getcwd(NULL, 0);
		if (oldpwd == NULL)
			return (1);
		if (change_dir(env_list, path, oldpwd) != 0)
			return (1);
		free(oldpwd);
		oldpwd = NULL;
	}
	else
	{
		perror("minishell: cd: HOME is not set");
		return (1);
	}
	return (0);
}

void	echo_n_option(char **x, int start_idx)
{
	int	i;

	i = start_idx + 1;
	while (x[i])
	{
		write(STDOUT_FILENO, x[i], ft_strlen(x[i]));
		if (x[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (start_idx == 0)
		write(STDOUT_FILENO, "\n", 1);
}

int	echo(char **x)
{
	if (ft_strcmp("-n", x[1]) == 0)
		echo_n_option(x, 1);
	else
		echo_n_option(x, 0);
	return (0);
}

int	pwd(char **x)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (2);
		// perror("pwd error");
	write(1, cwd, strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	cwd = NULL;
	return (0);
}

int	export(t_list *env_list, const char *x[])
{
	size_t		i;
	const char	*key;

	i = 1;
	if (x[i] == NULL)
	{
		write(1, " ", 1);
		return (0);
	}
	while (x[i])
	{
		key = get_env_key(x[i]);
		if (key == NULL)
			return (1);
		if (update_env(env_list, key, x[i]) == 1)
			return (1);
		free(key);
		key = NULL;
		i++;		
	}
	ft_getenv = get_env_value;
	return (0);
}
