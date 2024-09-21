#include "../../includes/builtin.h"
// #include "includes/minishell.h"

char	*get_env_key(const char *env)
{
	return (get_key_or_value("key", env));
}

char	*get_env_value(const char *env)
{
	return (get_key_or_value("value", env));
}

char *get_key_or_value(char *key_or_val, const char *env)
{
	char	*delim;
	size_t	key_len;
	size_t	val_len;
	char	*key;
	char	*val;

	delim = ft_strchr(env, '=');
	if (!delim)
		return (ft_strdup(env));
	else
	{
		key_len = delim - env;
		if (*key_or_val == 'k')
		{
			key = ft_substr(env, 0, key_len);
			return (key);
		}
		else
		{
			val_len = ft_strlen(env) - key_len - 1;
			val = ft_substr(env, key_len + 1, val_len);
			return (val);
		}
	}
}

int	update_env(t_list *env_list, const char *key, char *new_env)
{
	char	*cur_key;
	t_list	*node;

	node = env_list;
	while (env_list)
	{
		cur_key = get_env_key((char *)env_list->content);
		if (cur_key == NULL)
			return (1);
		if (ft_strcmp(cur_key, key) == 0)
		{
			mod_env(env_list, new_env);
			free(cur_key);
			cur_key = NULL;
			return (0);
		}
		free(cur_key);
		cur_key = NULL;
		env_list = env_list->next;
	}
	if (add_env(&node, new_env) == 1)
		return (1);
	return (0);
}

int	add_env(t_list **env_list, char *new_env)
{
	t_list	*new_node;

	new_node = ft_lstnew(new_env);
	if (new_node == NULL)
		return (1);
	ft_lstadd_back(env_list, new_node);
	// while (*env_list)
	// {
	// 	// printf("testss\n");
	// 	printf("%s\n", (*env_list)->content);
	// 	(*env_list) = (*env_list)->next;
	// }
	return (0);
}

void	mod_env(t_list *env_list, const char *new_env)
{
	free(env_list->content);
	env_list->content = NULL;
	env_list->content = (char *)new_env;
	return ;
}

void	rm_node(t_list **env_list, t_list **prev, t_list **cur)
{
	if (*prev)
		(*prev)->next = (*cur)->next;
	else
		*env_list = (*cur)->next;
	free(*cur);
	(*cur) = NULL;
}

int	rm_env(t_list **env_list, const char *key_to_remove)
{
	t_list	*cur;
	t_list	*prev;
	char	*cur_key;

	cur = *env_list;
	prev = NULL;
	while (cur)
	{
		cur_key = get_env_key((char *)cur->content);
		if (cur_key == NULL)
			return (1);
		if (ft_strcmp(cur_key, key_to_remove) == 0)
		{
			rm_node(env_list, &prev, &cur);
			free(cur_key);
			cur_key = NULL;
			return (0);
		}
		free(cur_key);
		cur_key = NULL;
		prev = cur;
		cur = cur->next;
	}
	return (0);
}

char **mk_env_list(t_list *env_list)
{
	t_list	*cur;
	size_t	len_env_list;
	char	**env_arr;
	size_t	i;

	i = 0;
	cur = env_list;
	len_env_list = ft_lstsize(env_list);
	env_arr = (char **)malloc((len_env_list + 1) * sizeof(char *));
	if (!env_arr)
		return (NULL);
	while (cur != NULL)
	{
		env_arr[i] = (char *)cur->content;			//
		// printf("%s\n", (char *)cur->content);	//
		cur = cur->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

