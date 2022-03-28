#include "../includes/minishell.h"

// Create a new empty cmd_list and returns a pointer
t_cmd_list	*new_t_cmd_list(void)
{
	t_cmd_list	*list;

	list = malloc(sizeof(t_cmd_list));
	list->start = NULL;
	return (list);
}

// Add a cmd_node to the cmd_list. Handles empty and non-empty lists
void	add_cmd_node_to_list(t_cmd_list *list, t_cmd_node *cmd_node)
{
	t_cmd_node	*n;

	if (list->start == NULL)
		list->start = cmd_node;
	else
	{
		n = list->start;
		while (1)
		{
			if (n->next == NULL)
			{
				n->next = cmd_node;
				break ;
			}
			n = n->next;
		}
	}
}

// Print the given list
void	print_cmd_list(t_cmd_list *list)
{
	t_cmd_node	*node;
	int			i;
	int			argc;

	i = 0;
	argc = 0;
	node = list->start;
	while (node)
	{
		//	printf("\nCommand '%s' with args:\n", node->cmd->cmd);
		while (node->cmd->argv[argc++])
			;
		//	printf("- '%s'\n", node->cmd->argv[argc++]);
		//	printf("INPUT:\n- filename: %s\n- fd: %d\n- in: '%s'\n",
		//	node->cmd->input.filename, node->cmd->input.fd, 
		//	node->cmd->input.in);
		//	printf("OUTPUT:\n- filename: %s\n- fd: %d\n", 
		//	node->cmd->output.filename, node->cmd->output.fd);
		node = node->next;
		i++;
	}
}

void	free_argv(t_cmd *cmd)
{
	int	i;

	i = 0;
	while(cmd->argv[i] != NULL)
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
}

void	free_t_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->argv)
			free_argv(cmd);
		if (cmd->content)
			free(cmd->content);
		if (cmd->input.fd != -1)
			close(cmd->input.fd);
		if (cmd->output.fd != -1)
			close(cmd->output.fd);
		if (cmd->input.filename)
			free(cmd->input.filename);
		if (cmd->input.in)
			free(cmd->input.in);
		if (cmd->output.filename)
			free(cmd->output.filename);
		if (cmd->hd_name)
			free(cmd->hd_name);
		free(cmd);
	}
}

t_cmd	*new_t_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	cmd->content = NULL;
	cmd->cmd = NULL;
	cmd->argv = (char **)malloc(4096 * sizeof(char *));
	if (!cmd->argv)
	{
		// TODO use exit_with_message instead, 
		// to make sure we deallocate everything else
		printf("Cannot allocate memory");
		exit(1);
	}
	cmd->argv[0] = NULL;
	cmd->input.filename = NULL;
	cmd->input.fd = -1;
	cmd->input.in = NULL;
	cmd->output.filename = NULL;
	cmd->output.fd = -1;
	cmd->output.append = 0;
	cmd->hd_name = NULL;
	return (cmd);
}

t_cmd_node	*new_t_cmd_node(void)
{
	t_cmd_node	*node;

	node = malloc(sizeof(t_cmd_node));
	node->cmd = new_t_cmd();
	node->next = NULL;
	return (node);
}

void	free_t_cmd_node(t_cmd_node *cmd_node)
{
	if (cmd_node)
	{
		if (cmd_node->next)
			free_t_cmd_node(cmd_node->next);
		if (cmd_node->cmd)
			free_t_cmd(cmd_node->cmd);
		free(cmd_node);
	}
}

void	free_t_cmd_list(t_cmd_list *list)
{
	if (list)
	{
		if (list->start)
			free_t_cmd_node(list->start);
		free(list);
		list = NULL;
	}
}
