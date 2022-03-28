/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:38:24 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:07:38 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*var_without_white_space(char *src)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!src)
		return (NULL);
	while (src[i])
	{
		if (src[i] == ' ')
		{
			j++;
			i++;
			while (src[i] == ' ')
				i++;
			if (!src[i])
				break ;
		}
		i++;
		j++;
	}
	new = malloc(j + 1);
	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] == ' ')
		{
			new[j] = src[i];
			j++;
			i++;
			while (src[i] == ' ')
				i++;
			if (!src[i])
				break ;
		}
		new[j] = src[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

int		inside_sq(char *str, int dollar)
{
	int		i;
	char	quote;

	quote = '\0';
	i = 0;
	while(str[i])
	{
		if (quote == '\'' && i == dollar)
			return (1);
		else if (quote == '\0' && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (quote != '\0' && str[i] == quote)
			quote = '\0';
		i++;
	}
	return (0);
}

char	*handle_dollar(char *str, char **env)
{
	char	*new;
	char	*tmp;
	char	*var;
	char	*new_var;
	int		i;
	int		start;

	i = 0;
	start = i;
	new = NULL;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			//if (i > 1 && str[i - 2] == '\'')
			if (inside_sq(str, i - 1))
				;
			// verify if there are more letters like this
			// if yes, do not interpret'$'
			else if (!str[i] || str[i] == ' ' || str[i] == ':' || str[i] == '\"' || str[i] == '=')
				i++;
			else if (str[i] == '?')
			{
				if (new == NULL)
					new = ft_strcpy(ft_itoa(g_sig.exit_status));
				else
				{
					tmp = ft_strjoin(new, ft_strcpy(ft_itoa(g_sig.exit_status)));
					free(new);
					new = ft_strcpy(tmp);
					free(tmp);
				}
				i++;
				start = i;
				//printf("new: %s\n", new);
			}
			else
			{
				var = get_var_value(env, ft_substr(str, i, check_dollar_var_name(&str[i])));
				if (i < 1 || str[i - 2] != '\"')
				{
					new_var = var_without_white_space(var);
					free(var);
					var = new_var;
				}
				if (new == NULL)
					new = ft_strcpy(var);
				else if (var)
				{
					tmp = ft_strjoin(new, var);
					free(new);
					new = ft_strcpy(tmp);
					free(tmp);
					free(var);
				}
				i += check_dollar_var_name(&str[i]);
				start = i;
			}
		}
		while (str[i] && str[i] != '$')
			i++;
		// write regular letters in new
		if (start != i)
		{
			if (new == NULL)
				new = ft_substr(str, start, i - start);
			else
			{
				tmp = ft_strjoin(new, ft_substr(str, start, i - start));
				free(new);
				new = ft_strcpy(tmp);
				free(tmp);
			}
			start = i;
		}
	}
	return (new);
}
