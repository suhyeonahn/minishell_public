#include "../includes/minishell.h"

int	has_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

int	has_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (i);
}

char	*get_var_value(char **env, char *name)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(name, env[i], ft_strlen(name)))
			return (ft_substr(env[i], ft_strlen(name) + 1,
					ft_strlen(env[i]) - ft_strlen(name) - 1));
		i++;
	}
	return (NULL);
}

char	**add_var(char **env, char *var)
{
	char	**tmp;
	int		i;

	if (!env)
	{
		tmp = malloc(sizeof(char *) * 2);
		tmp[0] = ft_strcpy(var);
		tmp[1] = 0;
	}
	else
	{
		i = 0;
		while (env && env[i])
			i++;
		tmp = malloc(sizeof(char *) * (i + 2));
		tmp[i + 1] = 0;
		i = 0;
		while (env && env[i])
		{
			tmp[i] = ft_strcpy(env[i]);
			i++;
		}
		tmp[i] = ft_strcpy(var);
		free_var(env);
	}
	return (tmp);
}

char	**delete_var(char **env, char *var)
{
	char	**new;
	int		i;
	int		j;

	j = 0;
	while (env && env[j])
		j++;
	new = malloc(sizeof(char *) * j);
	new[j - 1] = 0;
	i = 0;
	j = 0;
	while (env && env[j])
	{
		if (ft_strncmp(var, env[j], ft_strlen(var)))
		{
			new[i] = ft_strcpy(env[j]);
			i++;
		}
		j++;
	}
	i = 0;
	free_var(env);
	return (new);
}

void	free_var(char **var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		free(var[i]);
		i++;
	}
	free(var[i]);
	free(var);
}
