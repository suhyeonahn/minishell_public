#include "../includes/minishell.h"

void	handle_quotes(char *str, t_tokenize_state *state)
{
	char	open_quote;

	if (str[state->i] == '\'' || str[state->i] == '\"')
	{
		// Si jamais on est pas `in_token`, 
		// ca veut dire qu'on arrive sur une nouvelle token
		// qui commence par des quotes. 
		// Dans ce cas il faut activer le `in_token` avec
		// update du last, pour que cette token soit bien extraite. 
		// Si on ne fait pas ca
		// elle est skipped. 
		// Par exemple: `echo "toto" tata` => `echo tata`
		if (!state->in_token)
		{
			state->last = state->i;
			state->in_token = 1;
		}
		open_quote = str[state->i];
		state->i++;
		while (str[state->i])
		{
			if (str[state->i] == open_quote)
			{
				// On fait du recursif pour s'assurer de gerer le cas ou
				// `i+1` serait aussi une quote
				state->i++;
				return (handle_quotes(str, state));
			}
			state->i++;
		}
	}
}

int	char_is_in(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (c);
		i++;
	}
	return (0);
}

void	close_token(char *str, t_tokenize_state *state, char **env)
{
	char	*token;
	char	*tmp;
	int		last_argv;

	last_argv = 0;
	if (state->in_token)
	{
		token = ft_substr(str, state->last, (state->i - state->last));
		if (ft_strlen(token) > 0)
		{
			tmp = handle_dollar(token, env);
			free(token);
			token = remove_quote(tmp);
			//printf("TOKEN: '%s'\n", token);
			free(tmp);
			if (ft_strlen(token) || str[state->last] == '\"')
			{
				//printf("TOKEN2: '%s'\n", token);
				if (!token && !state->cmd->cmd)
				{
					state->cmd->cmd = "";
				}
				else if (!state->cmd->cmd)
				{
					//printf("cmd->cmd : '%s'\n", token);
					state->cmd->cmd = token;
				}
				while (state->cmd->argv[last_argv])
					last_argv++;
				if (!token)
					state->cmd->argv[last_argv] = "";
				else
					state->cmd->argv[last_argv] = token;
				//printf("TOKEN3: '%s'\n", state->cmd->argv[last_argv]);
				state->cmd->argv[last_argv + 1] = NULL;
			}
			state->last = state->i;
			state->in_token = 0;
		}
	}
}

void	handle_input_file(t_tokenize_state *state, char *identifier)
{
	int	fd;

	// TODO: Handle quotes and $
	// ATTENTION, pas d'interpretation du $ dans le delimiter, 
	// meme avec les quotes.
	// open file
	fd = open(identifier, O_RDONLY);
	if (fd == -1)
	{
		// TODO: populate mini->ret
		g_sig.exit_status = 127;
		perror(identifier);
	}
	if (state->cmd->input.filename)
		free(state->cmd->input.filename);
	if (access(identifier, F_OK) == 0)
		state->cmd->input.filename = identifier;
	if (state->cmd->input.fd <= 0)
		close(state->cmd->input.fd);
	if (access(identifier, F_OK) == 0)
		state->cmd->input.fd = fd;
	if (state->cmd->input.in)
	{
		free(state->cmd->input.in);
		state->cmd->input.in = NULL;
	}
}

char	*hd_name(int cmd_i)
{
	char	*hd_name;
	char	*nb;

	nb = ft_itoa(cmd_i);
	hd_name = ft_strjoin("here_doc", nb);
	free(nb);
	return (hd_name);
}

void	handle_input_delimiter(t_tokenize_state *state, char *delimiter)
{
	char	*in;
	char	*buf;
	char	*name;

	if (g_sig.syntax_error == 1)
		return ;
	name = state->cmd->hd_name;
	if (state->cmd->input.filename)
	{
		free(state->cmd->input.filename);
		state->cmd->input.filename = NULL;
		close(state->cmd->input.fd);
		state->cmd->input.fd = -1;
		if (state->cmd->input.in)
			free(state->cmd->input.in);
	}
	state->cmd->input.filename = name;
	state->cmd->input.fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	in = "";
	while (1)
	{
		buf = readline("> ");
		if (!ft_strcmp(buf, delimiter))
		{
			state->cmd->input.in = in;
			write(state->cmd->input.fd, in, ft_strlen(in));
			close(state->cmd->input.fd);
			state->cmd->input.fd = open(name, O_RDONLY);
			free(delimiter);
			return ;
		}
		in = ft_strjoin(in, buf);
		in = ft_strjoin(in, "\n");
	}
}

void	handle_output_file(t_tokenize_state *state,
		char *identifier, int append)
{
	int	fd;

	if (append)
		fd = open(identifier, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(identifier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		g_sig.exit_status = 127;
		perror(identifier);
		return ;
	}
	if (state->cmd->output.filename)
		free(state->cmd->output.filename);
	state->cmd->output.filename = identifier;
	if (state->cmd->output.fd)
		close(state->cmd->output.fd);
	state->cmd->output.fd = fd;
	state->cmd->output.append = append;
}

t_quote	*new_t_quote(void)
{
	t_quote *q;

	q = malloc(sizeof(t_quote));
	q->i = 0;
	q->len = 0;
	q->n_quote = 0;
	q->quotes_pos = malloc(sizeof(char *));
	q->quotes_pos[0] = NULL;
	q->new = NULL;
	q->quote = '\0';
	return (q);
}

void	update_quote(t_quote *q)
{
	char	**old;
	char	**new;
	int 	i;

	i = 0;
	old = q->quotes_pos;
	q->n_quote++;
	new = malloc(sizeof(char *) * q->n_quote + 1);
	while (old[i])
	{
		new[i] = ft_strcpy(old[i]);
		free(old[i]);
		i++;
	}
	new[i] = ft_itoa(q->i);
	new[i + 1] = NULL;
	free(old);
	q->quotes_pos = new;
}

int	in_tab(int i, t_quote *q)
{
	int	atoi;
	int j;

	j = 0;
	atoi = -1;
	while(q->quotes_pos[j])
	{
		atoi = ft_atoi(q->quotes_pos[j]);
		if (atoi == i)
			return (1);
		j++;
	}
	return (0);
}

char	*rm_quotes(t_quote *q, char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) - q->n_quote) + 1);
	while(str[i])
	{
		if(in_tab(i, q) == 1)
			i++;
		else
		{
			new[j] = str[i];
			j++;
			i++;
		}
	}
	new[j] = '\0';
	return (new);
}

char	*remove_quotes(char *s)
{
	t_quote *q;

	q = new_t_quote();
	while (s[q->i])
	{
		if (q->quote == '\0' && (s[q->i] == '\'' || s[q->i] == '\"'))
		{
			q->quote = s[q->i];
			update_quote(q);
		}
		else if (q->quote != '\0' && s[q->i] == q->quote)
		{
			q->quote = '\0';
			update_quote(q);
		}
		else
			q->len++;
		q->i++;
	}
	q->new = rm_quotes(q, s);
	if (s)
		free(s);
	return (q->new);
}

char	*parse_identifier(char *str, t_tokenize_state *state)
{
	int start;
	int done_leading_spaces;
	char *substr;

	start = state->i;
	done_leading_spaces = 0;
	while (str[state->i])
	{
		if (char_is_in(str[state->i], "|<>"))
		{
			if (!char_is_in(str[state->i - 1], "|<> "))
				break ;
			write(2, "minishell: syntax error near unexpected token `", 47);
			write(2, &str[state->i], 1);
			write(2, "`\n", 2);
			return (NULL);
			//break;
		}
		else if (str[state->i] == ' ' && done_leading_spaces)
			break ;
		else if (str[state->i] != ' ')
		{
			if (!done_leading_spaces)
			{
				done_leading_spaces = 1;
				start = state->i;
			}
			handle_quotes(str, state);
			if (str[state->i] == ' ')
				break ;
		}
		state->i++;
	}
	if (str[state->i] == 0 && !done_leading_spaces)
	{
		write(2, "minishell: syntax error near unexpected token `newline or |`\n", 61);
		return (NULL);
	}
	state->last = state->i;
	substr = ft_substr(str, start, (state->i - start));
	start = ft_strlen(substr);
	//printf("delimiter : '%s'\n", substr);
	substr = remove_quotes(substr);
	if ((int)ft_strlen(substr) < start)
		state->last++;
	//printf("delimiter after remove_quotes : '%s'\n", substr);
	return (substr);
}

void	handle_input(char *str, t_tokenize_state *state)
{
	char	*identifier;

	identifier = NULL;
	if (!ft_strcmp(state->cmd->input.filename, state->cmd->hd_name))
	{
		close(state->cmd->input.fd);
		unlink(state->cmd->hd_name);
	}
	if (str[state->i + 1] == '<')
	{
		state->i += 2;
		identifier = parse_identifier(str, state);
		if (!identifier)
		{
			g_sig.exit_status = 2;
			return ;
		}
		handle_input_delimiter(state, identifier);
	}
	else
	{
		state->i += 1;
		identifier = parse_identifier(str, state);
		if (!identifier)
		{
			g_sig.exit_status = 2;
			return ;
		}
		handle_input_file(state, identifier);
	}
}

void	handle_output(char *str, t_tokenize_state *state)
{
	char	*identifier;

	identifier = NULL;
	if (str[state->i + 1] == '>')
	{
		state->i += 2;
		identifier = parse_identifier(str, state);
		if (!identifier)
		{
			g_sig.exit_status = 2;
			return ;
		}
		handle_output_file(state, identifier, 1);
	}
	else
	{
		state->i += 1;
		identifier = parse_identifier(str, state);
		if (!identifier)
		{
			g_sig.exit_status = 2;
			return ;
		}
		handle_output_file(state, identifier, 0);
	}
}

void	handle_redirect(char *str, t_tokenize_state *state)
{
	if (str[state->i] == '<')
		handle_input(str, state);
	else if (str[state->i] == '>')
		handle_output(str, state);
}

void	init_t_tokenize_state(t_tokenize_state *state, int cmd_i)
{
	state->i = 0;
	state->last = 0;
	state->in_token = 0;
	state->cmd = new_t_cmd();
	state->cmd->hd_name = hd_name(cmd_i);
}

t_cmd	*parse_command(char *str, char **env, int cmd_i)
{
	t_tokenize_state	state;
	t_cmd				*cmd;

	init_t_tokenize_state(&state, cmd_i);
	while (str[state.i])
	{
		// On veut avancer si jamais on croise des quotes.
		// Donc handle_quotes fera avancer
		// `i` jusqu'a la prochaine quote, ou la fin de str
		handle_quotes(str, &state);
		if (char_is_in(str[state.i], "<>"))
		{
			close_token(str, &state, env);
			handle_redirect(str, &state);
			if (g_sig.exit_status != 0)
			{
				g_sig.syntax_error = 1;
				return (NULL);
			}
		}		
		else if (str[state.i] == ' ')
			close_token(str, &state, env);
		else if (!state.in_token)
		{
			state.last = state.i;
			state.in_token = 1;
		}
		state.i++;
	}
	close_token(str, &state, env);
	cmd = state.cmd;
	return (cmd);
}
