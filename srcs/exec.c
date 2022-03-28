/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:38:47 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 10:59:54 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_memcmp(void *s1, void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

void	exit_error(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

char	*ft_strnstr(char *haystack, char *needle, size_t len)
{
	size_t	len_nd;

	len_nd = ft_strlen(needle);
	if (!*needle)
		return ((char *)haystack);
	while (*haystack && len-- >= len_nd)
	{
		if (*haystack == *needle && !(ft_memcmp(haystack, needle, len_nd)))
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}

void	free_strarr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(env[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_strarr(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_strarr(paths);
	return (NULL);
}
/////////////////
// INFORMATION:: execute() is called within child process.
// TODO:: To communicate 'g_sig.exit_status' value among processes.  
// SOLUTION:: use pipe() communication;
// 			  can write in child process and read in parent process.
/////////////////
//int execute_cmd(char *s_cmd, char **argv, char ***env)
//{
//	int		status = 0;
//	char	*path;
//
//	path = find_path(s_cmd, *env);
//	status = execve(path, argv, *env);
//	free(path);
//	if (status == -1)
//	{
//		printf("minishell: %s :command not found\n", s_cmd); //do the same as bash error
//		exit(127);
//	}
//	return (status);
//}

int	execute(char *s_cmd, char**argv, char ***env, t_cmd *cmd)
{
	int		status;
	char	*path;
	(void)argv;
	(void)cmd;

	//printf("EXEC: %s\n", path);
	//write(2, "'", 1);
	//write(2, s_cmd,  ft_strlen(s_cmd));
	//write(2, "'\n", 2);
	status = check_builtins(cmd, env);
	if (!status)
		exit(0);
	// In 'check_builtins' when error occurs update exit_status = 1 and return 1
	// If it returns 2 it means no builtin has found in command
	else if (status == 2)
	{

		//path = find_path(s_cmd, *env);
		path = s_cmd;
		if (access(path, F_OK) != 0)
			path = find_path(s_cmd, *env);
		//write(1, argv[1], ft_strlen(argv[1]));
		//write(1, "\n", 1);
		if (!ft_strcmp(s_cmd, "ls") && !ft_strcmp(cmd->input.filename, "pipe"))
		{
			close(cmd->input.fd);
			unlink("pipe");
		}
		status = execve(path, argv, *env);
		free(path);
		if (status == -1)
		{
			write(2, "minishell: ", 12);
			write(2, s_cmd, ft_strlen(s_cmd));
			write(2, ":command not found\n", 19);
			//printf("minishell: %s:command not found\n", s_cmd); //do the same as bash error
			exit(127);
		}
		return (status);
	}
	exit(1);
}

int		has_pipe(t_cmd_node *cmds)
{
	if (cmds->next != NULL)
		return (1);
	else
		return (0);
}
/////////////
// TODO: use pipe() to communicate with child process
//		 to able to read g_sig.exit_status in main process
/////////////
void	exec_no_rdrct(t_cmd *cmd, char ***env)
{
	pid_t	pid;
	int 	wstatus;

	wstatus = check_builtins(cmd, env);
	if (!wstatus)
		g_sig.exit_status = 0;
	else if (wstatus == 2)
	{
		pid = fork();
		if (pid == -1)
			exit_error("pid");
		if (pid == 0)
			execute(cmd->cmd, cmd->argv, env, cmd); // execute() is called within child process
		else
		{
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
			{
				g_sig.exit_status = WEXITSTATUS(wstatus);
				//printf("Exit status of the child was %d\n", g_sig.exit_status);
			}
		}
	}
}

void	child_rdrct(int *fd, t_cmd *cmd)
{

	close(fd[0]);
	if (cmd->output.fd != -1)
		dup2(cmd->output.fd, STDOUT_FILENO);
	else
		dup2(fd[1], STDOUT_FILENO);
	//	if (cmd->input.in)
	//	{
	//		cmd->input.fd = open("here_doc", O_RDWR | O_CREAT | O_TRUNC, 0644);
	//		cmd->input.filename = "here_doc";
	//		write(cmd->input.fd, cmd->input.in, ft_strlen(cmd->input.in));
	//	}
	if (cmd->input.fd != -1)
		dup2(cmd->input.fd, STDIN_FILENO);
	close(fd[1]);
}

void	parent_rdrct(int *fd, t_cmd *cmd, t_cmd_node *next)
{
	//char buffer[4096];
	(void)cmd;

	close(fd[1]);
	//printf("output : %d\n", cmd->output.fd);
	if (cmd->output.fd != -1)
		dup2(fd[0], STDIN_FILENO);
	else
	{
		if (next->cmd->input.fd == -1)
		{
			next->cmd->input.fd = open("pipe", O_RDWR | O_CREAT | O_TRUNC, 0644);
			next->cmd->input.filename = "pipe";
			dup2(fd[0], next->cmd->input.fd);
		}
	}
	//	read(STDIN_FILENO, buffer, 4096);
	//	printf("buf : %s\n", buffer);
	//if (next->cmd->input.in)
	//	free(next->cmd->input.in);
	//next->cmd->input.in = "";
	//next->cmd->input.in = ft_strjoin(next->cmd->input.in, buffer);
	//printf("next cmd input : %s\n", next->cmd->input.in);
	close(fd[0]);
}

/////////////
// TODO: use pipe() to communicate with child process
//		 to able to read g_sig.exit_status in main process
/////////////
void	exec_rdrct(t_cmd *cmd, t_cmd_node *next, char ***env)
{
	pid_t	pid;
	int		fd[2];
	int 	wstatus;

	if (pipe(fd) == -1)
		exit_error("pipe");
	//printf("fd[0]: %d\nfd[1]: %d\n", fd[0], fd[1]);
	pid = fork();
	if (pid == -1)
		exit_error("pid");
	if (pid == 0)
	{
		child_rdrct(fd, cmd);
		execute(cmd->cmd, cmd->argv, env, cmd);
	}
	else
	{
		parent_rdrct(fd, cmd, next);
		waitpid(pid, &wstatus, 0);
		if ( WIFEXITED(wstatus) )
		{
			g_sig.exit_status = WEXITSTATUS(wstatus);
			//printf("Exit status of the child was %d\n", g_sig.exit_status);
		}
		if (!ft_strcmp(cmd->input.filename, "pipe"))
		{
			close(cmd->input.fd);
			unlink("pipe");
		}
		if (!ft_strcmp(cmd->input.filename, cmd->hd_name))
		{
			close(cmd->input.fd);
			unlink(cmd->hd_name);
		}
	}
}

/////////////
// TODO: use pipe() to communicate with child process
//		 to able to read g_sig.exit_status in main process
/////////////
void	exec_last(t_cmd *cmd, char ***env)
{
	pid_t	pid;
	int		wstatus;
	//char	buff[1000];

	//(void)env;
	pid = fork();
	if (pid == -1)
		exit_error("pid");
	if (pid == 0)
	{
		if (cmd->output.fd != -1)
			dup2(cmd->output.fd, STDOUT_FILENO);
		if (cmd->input.fd != -1)
			dup2(cmd->input.fd, STDIN_FILENO);
		execute(cmd->cmd, cmd->argv, env, cmd); // execute() is called within child process
		exit(0);
	}
	else
	{
		waitpid(pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
		{
			g_sig.exit_status = WEXITSTATUS(wstatus);
			//printf("Exit status of the child was %d\n", g_sig.exit_status);
		}
		if (!ft_strcmp(cmd->input.filename, "pipe"))
		{
			close(cmd->input.fd);
			unlink("pipe");
		}
		if (!ft_strcmp(cmd->input.filename, cmd->hd_name))
		{
			close(cmd->input.fd);
			unlink(cmd->hd_name);
		}
	}
}

void	exec_nopipe(t_cmd *cmd, char ***env)
{
	pid_t	pid;
	int 	wstatus;
	int 	stdout_cpy;

	(void)stdout_cpy;
	//write(1, "l\n", 2);
	stdout_cpy = dup(STDOUT_FILENO);
	if (cmd->output.fd != -1)
		dup2(cmd->output.fd, STDOUT_FILENO);
	wstatus = check_builtins(cmd, env);
	dup2(stdout_cpy, STDOUT_FILENO);
	//write(STDOUT_FILENO, "test\n", 5);
	if (!wstatus)
		g_sig.exit_status = 0;
	else if (wstatus == 2)
	{
		pid = fork();
		if (pid == -1)
			exit_error("pid");
		if (pid == 0)
		{
			if (cmd->output.fd != -1)
				dup2(cmd->output.fd, STDOUT_FILENO);
			if (cmd->input.fd != -1)
				dup2(cmd->input.fd, STDIN_FILENO);
			execute(cmd->cmd, cmd->argv, env, cmd); // execute() is called within child process
		}
		else
		{
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
			{
				g_sig.exit_status = WEXITSTATUS(wstatus);
				//printf("Exit status of the child was %d\n", g_sig.exit_status);
			}
			if (!ft_strcmp(cmd->input.filename, cmd->hd_name))
			{
				close(cmd->input.fd);
				unlink(cmd->hd_name);
			}
		}
	}
}

void	exec(t_mini *mini, t_cmd *cmd, t_cmd_node *next, char ***env)
{
	if (!mini->cmds->start->next && cmd->input.fd == -1 && cmd->output.fd == -1 && cmd->input.in == NULL)
		exec_no_rdrct(cmd, env);
	else if (!next && !ft_strcmp(cmd->input.filename, "pipe"))
		exec_last(cmd, env);
	else if (!next)
		exec_nopipe(cmd, env);
	else
		exec_rdrct(cmd, next, env);
}
// to check :: when 'n'th argument is not correct in command line
//             it should print error with 'n'th argument 
void	exec_commands(t_mini *mini, char ***env)
{
	(void)mini;
	t_cmd_node *cmd_node;
	int			stdin_cpy;

	stdin_cpy = dup(STDIN_FILENO);
	cmd_node = mini->cmds->start;
	//return ;
	while (cmd_node)
	{
		//	printf("%s: command not found\n", cmd_node->cmd->cmd);
		exec(mini, cmd_node->cmd, cmd_node->next, env);
		close(cmd_node->cmd->input.fd);
		close(cmd_node->cmd->output.fd);
		cmd_node = cmd_node->next;
	}
	dup2(stdin_cpy, STDIN_FILENO);
	close(stdin_cpy);
}
