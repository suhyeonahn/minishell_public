/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 20:00:12 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:29:37 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_sig	g_sig;

void	test_list(t_node *node)
{
	int	i;

	i = 0;
	while (node->next)
	{
		printf("node n %d : %s- || quote : %d\n",
			i, node->s, node->is_quote); // '-' after %s is there to spot spaces before end of line
		i++;
		node = node->next;
	}
	printf("node n %d : %s || quote : %d-\n", i, node->s, node->is_quote);
}

t_cmd_node	*extract_cmd_node(t_mini *mini, int start, int end, char **env)
{
	t_cmd_node	*node;
	char		*substr;

	node = new_t_cmd_node();
	free(node->cmd->argv);
	free(node->cmd);
	substr = ft_substr(mini->line, start, (end - start));
	mini->cmd_i++;
	node->cmd = parse_command(substr, env, mini->cmd_i);
	if (g_sig.syntax_error == 1)
		return (NULL);
	node->cmd->content = substr;
	return (node);
}

// Parse the various "commands" in the given line.
// Takes care of checking for quotes
// and separating each commands when we see a pipe (|)
int	parse_commands(t_mini *mini, char **env)
{
	// On utilise cette struct pour limiter 
	// le nombre de lignes principalement
	t_parse_cmd	*p;

	p = new_t_parse_cmd();
	mini->cmd_i = 0;
	while (mini->line[p->i] != '\0')
	{
		if (p->i == p->last && (mini->line[p->i] == ' '
				|| mini->line[p->i] == '|'))
		{
			while (mini->line[p->i] == ' ')
				p->i++;
			if (mini->line[p->i] == '|')
			{
				write(2, "minishell: syntax error near unexpected token `|`\n", 50);
				//printf("minishell: syntax error near unexpected token `|`\n");
				g_sig.exit_status = 2;
				g_sig.syntax_error = 1;
				return (2);
			}
		}
		// Si on a pas de open_quote et qu'on voit un ' ou ", 
		// on le stock comme open_quote
		if (p->quote == '\0' && (mini->line[p->i] == '\'' || mini->line[p->i] == '\"'))
			p->quote = mini->line[p->i];
		// Si on a une open_quote, et qu'on voit la meme,
		// on la ferme
		else if (p->quote != '\0' && p->quote == mini->line[p->i])
			p->quote = '\0';
		// Si on a pas d'open_quote et qu'on voit un pipe, 
		// on garde l'interval [last, i[ de notre
		// string `line` comme command
		else if (p->quote == '\0' && mini->line[p->i] == '|')
		{
			add_cmd_node_to_list(p->cmds, extract_cmd_node(mini, p->last, p->i, env));
			p->last = p->i + 1;
		}
		if (g_sig.syntax_error == 1)
			return (2);
		p->i++;
	}
	if (p->quote != '\0')
	{
		write(2, "minishell: syntax error: unclosed quote\n", 40);
		//printf("Minishell: syntax error: missing %c\n", p->quote);
		g_sig.exit_status = 2;
		g_sig.syntax_error = 1;
		mini->ret = 2;
	}
	// Dans tous les cas a la fin on insert la derniere command qu'on etait en train de checker
	if (g_sig.syntax_error == 1)
	{
		free(p);
		return (2);
	}
	add_cmd_node_to_list(p->cmds, extract_cmd_node(mini, p->last, p->i, env));
	mini->cmds = p->cmds;
	free(p);
	return (mini->ret);
}

void	free_t_mini(t_mini *mini)
{
	if (mini->cmds)
		free_t_cmd_list(mini->cmds);
	free(mini->line);
	free(mini);
}

void	free_env(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	exit_with_message(t_mini *mini, char **env, int ret, char *message)
{
	free_t_mini(mini);
	free_env(env);
	write(2, message, ft_strlen(message));
	exit(ret);
}

t_mini	*new_t_mini(void)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	mini->ret = 0;
	mini->cmd_i = 0;
	mini->line = NULL;
	mini->cmds = NULL;
	return (mini);
}

void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}

void	prompt_line(t_mini *mini, char **env)
{
	rl_outstream = stderr;
	if (mini->line != NULL)
		free(mini->line);
	// TODO: Maybe improve the readline to include the previous return status
	mini->line = readline("\e[0;32mminishell $\e[0m ");
	//mini->line = readline("minishell $ ");
	if (!mini->line)
	{
		write(2, "\033[1A", 0);
		write(2, "\033[10C", 0);
		exit_with_message(mini, env, 0, "exit\n");
	}
}

// TODO:
// 1. Error management => Everything needed in t_mini + 
// way to stop when required
// 2. Signal handling with children programs
// 3. Cleanup legacy
// 4. Close file descriptor
// 5. Update return value with $?

// DONE:
// Builtins updated, using a new signature 
// `(char **argv, char ***env)`, instead of
// `(char **argv, char **env)` 
// cuz needed to use env pointer to update its value
// Env variables interpretaiton with $
int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_mini			*mini;
	char			**new_env;
	struct termios	new;

	tcgetattr(STDIN_FILENO, &new);
	new.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	mini = new_t_mini();
	check_signal();
	new_env = init_new_env_table(env);
	modify_shlvl(&new_env);
	sig_init();
	while (1)
	{
		prompt_line(mini, new_env);
		g_sig.syntax_error = 0;
		if (ft_strlen(mini->line) > 0)
		{
			add_history(mini->line);
			if (parse_commands(mini, new_env) == 0)
			{
				//print_cmd_list(mini->cmds);
				if (mini->ret == 0)
					exec_commands(mini, &new_env);
				//free_t_cmd_list(mini->cmds);
			}
		}
	}
	free_t_mini(mini);
	return (0);
}

// int	main(int argc, char **argv, char **env)
// {
//     (void)argc;
//     (void)argv;
// 	char    *line;
//     int     ret;
// 	t_list	*list;
// 	t_v	v;
// 	
//     check_signal();
// 	v = init_var();
// 	while (1)
// 	{
// 		list = create_list();
//         line = NULL;
// 		line = readline("minishell> ");
// 		if (!line)
// 		{
// 			printf("exit\n");
// 			free_list(list);
// 			break;
// 		}
// 		put_strs_in_list(list, line);
// 		test_list(list->start);
// 		ret = command_line_management(list, env, &v);
// 		free_list(list);
// 		if (ft_strlen(line) > 0)
// 			add_history(line);
// 		free(line);
// 	}
// 	return (0);
// }
