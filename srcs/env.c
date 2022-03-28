/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:38:36 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 10:57:38 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    modify_shlvl(char***  env)
{
	char	*str;
	int		i;
	int		n;
	char	*itoa;

	i = 0;
	while ((*env) && (*env)[i])
	{
		if (!ft_strncmp("SHLVL", (*env)[i], 5))
		{
			str = ft_substr((*env)[i], 6, ft_strlen((*env)[i]) - 6);
			n = ft_atoi(str) + 1;
			free(str);
			free((*env)[i]);
			itoa = ft_itoa(n);
			(*env)[i] = ft_strjoin("SHLVL=", itoa);
			free(itoa);
		}
		i++;
	}
}

void    modify_oldpwd(char***  env, char *str)
{
	int i;

	i = 0;
	while ((*env) && (*env)[i])
	{
		if (!ft_strncmp("OLDPWD", (*env)[i], 6))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("OLDPWD=", str);
		}
		i++;
	}
}

void    modify_pwd(char***  env, char *str)
{
	int i;

	i = 0;
	while ((*env) && (*env)[i])
	{
		if (!ft_strncmp("PWD", (*env)[i], 3))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("PWD=", str);
		}
		i++;
	}
}
