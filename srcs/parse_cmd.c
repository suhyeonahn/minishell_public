#include "../includes/minishell.h"

t_parse_cmd	*new_t_parse_cmd(void)
{
	t_parse_cmd	*parse;

	parse = malloc(sizeof(t_parse_cmd));
	parse->i = 0;
	parse->last = 0;
	parse->quote = '\0';
	parse->cmds = new_t_cmd_list();
	return (parse);
}
