/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:39:11 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:00:10 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    *remove_quote(char *str)
{
	char *new = NULL;
	int i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
			{
				new = write_line(new, str[i]);
				i++;
			}
		}
		else if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
			{
				new = write_line(new, str[i]);
				i++;
			}
		}
		else
			new = write_line(new, str[i]);
		i++;
	}
	return (new);
}
