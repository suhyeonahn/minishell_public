/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:37:24 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 14:41:45 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_var_name(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[0] < 65 || (str[0] > 90 && str[0] < 95)
			|| str[0] == 96 || str[0] > 122)
			return (1);
		while (str[i] && str[i] != '=')
		{
			if ((str[i] >= 48 && str[i] <= 57)
				|| (str[i] >= 65 && str[i] <= 90)
				|| (str[i] >= 97 && str[i] <= 122)
				|| str[i] == '_')
				i++;
			else
				return (1);
		}
		return (0);
	}
	return (0);
}

int	check_dollar_var_name(char *token)
{
	int	i;

	if (token[0] < 65 || (token[0] > 90 && token[0] < 95)
		|| token[0] == 96 || token[0] > 122)
		return (1);
	i = 1;
	while (token[i])
	{
		if ((token[i] >= 48 && token[i] <= 57)
			|| (token[i] >= 65 && token[i] <= 90)
			|| (token[i] >= 97 && token[i] <= 122)
			|| token[i] == '_')
			i++;
		else
			return (i);
	}
	return (i + 1);
}
