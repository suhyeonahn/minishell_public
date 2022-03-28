/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:34:31 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:12:49 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	new_node(t_node *list, char *str, int quote)
{
	t_node	*new_node;

	if (!str)
		return ;
	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return ;
	new_node->is_quote = quote;
	new_node->s = str;
	new_node->next = NULL;
	while (list->next)
	{
		list = list->next;
	}
	list->next = new_node;
}

void	init_l(t_list *list, char *str, int quote)
{
	list->start->s = str;
	list->start->is_quote = quote;
}

t_list	*create_list(void)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (0);
	list->start = malloc(sizeof(t_node));
	list->start->is_quote = 0;
	list->start->s = NULL;
	list->start->next = NULL;
	return (list);
}

char	**init_new_env_table(char **env)
{
	char	**envp;
	char	**new;
	int		i;

	envp = env;
	i = 0;
	while (*envp != 0)
	{
		envp++;
		i++;
	}
	new = malloc(sizeof(char *) * i + 1);
	new[i] = NULL;
	envp = env;
	i = 0;
	while (*envp != 0)
	{
		new[i] = ft_strcpy(*envp);
		envp++;
		i++;
	}
	return (new);
}
