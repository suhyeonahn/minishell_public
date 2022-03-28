/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:36:54 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 14:39:47 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **argv)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	while (argv[i] && !ft_strcmp("-n", argv[i]))
	{
		flag = 1;
		i++;
	}
	while (argv && argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!flag)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(char **argv, char ***env)
{
	int		ret;
	char	s1[100];
	char	s2[100];
	char	*old_pwd;

	ret = 0;
	old_pwd = getcwd(s1, 100);
	if (argv[1])
	{
		if (!ft_strcmp(argv[1], "-"))
		{
			ret = chdir(get_var_value(*env, "OLDPWD"));
			ft_pwd();
		}
		else
			ret = chdir(argv[1]);
	}
	else
		ret = chdir(get_var_value(*env, "HOME"));
	//printf("%s\n", getcwd(s2, 100));
	if (ret == -1)
	{
		write(2, "cd: ", 4);
		write(2, argv[1], ft_strlen(argv[1]));
		write(2, ": No such file or directory\n", 28);
		g_sig.exit_status = 1;
		return (1);
		//printf("cd: %s: No such file or directory\n", argv[1]);
	}
	else
	{
		modify_oldpwd(env, old_pwd);
		modify_pwd(env, getcwd(s2, 100));
	}
	return (0);
}

int	ft_pwd(void)
{
	char	s[100];

	getcwd(s, 100);
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
	return (0);
}

int	ft_export(char **argv, char ***env)
{
	int	i;
	int	j;
	int	ret;
	int	var_name;

	if (!argv[1])
	{
		j = 0;
		while ((*env) && (*env)[j])
		{
			write(1, "declare -x ", 11);
			var_name = has_equal((*env)[j]);
			if (var_name)
			{
				write(1, (*env)[j], var_name + 1);
				write(1, "\"", 1);
				write(1, &(*env)[j][var_name + 1],
					ft_strlen((*env)[j]) - (var_name + 1));
				write(1, "\"\n", 2);
			}
			else
			{
				write(1, (*env)[j], ft_strlen((*env)[j]));
				write(1, "\n", 1);
			}
			j++;
		}
		return (0);
	}
	i = 1;
	while (argv && argv[i])
	{
		ret = has_var(argv[i]);
		var_name = check_var_name(argv[i]);
		if (var_name == 1)
		{
			write(2, "export: `", 9);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "`: not a valid identifier\n", 26);
			g_sig.exit_status = 1;
			return (1);
		}
		else
		{
			j = 0;
			while ((*env) && (*env)[j])
			{
				if (!ft_strncmp(argv[i], (*env)[j], ret))
				{
					if (argv[i][ret])
					{
						free((*env)[j]);
						(*env)[j] = ft_strcpy(argv[i]);
					}
					break ;
				}
				j++;
			}
			if (!(*env)[j])
				(*env) = add_var((*env), argv[i]);
		}
		i++;
	}
	return (0);
}

int	ft_env(char ***env)
{
	char **envp;

	envp = (*env);
	while (*envp != 0)
	{
		if (has_equal(*envp))
		{
			write(1, *envp, ft_strlen(*envp));
			write(1, "\n", 1);
		}
		envp++;
	}
	return (0);
}

int	ft_unset(char **argv, char ***env)
{
	int	i;
	int	j;
	int	ret;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv && argv[i])
	{
		ret = check_var_name(argv[i]);
		if (ret == 1 || ret == -1)
		{
			write(2, "unset: `", 8);
			write(2, argv[i], ft_strlen(argv[i]));
			write(2, "`: not a valid identifier\n", 26);
			//printf("unset: `%s': not a valid identifier\n", argv[i]);
			g_sig.exit_status = 1;
			return (1);
		}
		j = 0;
		while ((*env) && (*env)[j])
		{
			if (!ft_strncmp(argv[i], (*env)[j], ft_strlen(argv[i])))
			{
				(*env) = delete_var((*env), argv[i]);
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_exit(char **argv)
{
	long long	return_code;

	if (argv[1])
	{
		if (is_digit(argv[1]))
		{
			write(2, "minishell: exit: ", 17);
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, ": numeric argument required\n", 28);
			g_sig.exit_status = 255;
			exit(g_sig.exit_status);
		}
		if (argv[2])
		{
			write(2, "exit\n", 5);
			write(2, "minishell: exit: too many arguments\n", 36);
			g_sig.exit_status = 1;
			return (1);
		}
		return_code = ft_atoi_for_exit(argv[1]);
		if (return_code >= 0 && return_code <= 255)
		{
			g_sig.exit_status = return_code;
			exit(g_sig.exit_status);
		}
		else
		{
			if (return_code < 0)
			{
				while (return_code < 100000000000000)
					return_code += 256 * 300000000000;
				while (return_code < INT_MIN)
					return_code += 256 * 8000000;
				while (return_code < 0)
					return_code += 256;
			}
			else if (return_code > 255)
			{
				while (return_code > 100000000000000)
					return_code -= 256 * 300000000000;
				while (return_code > INT_MAX)
					return_code -= 256 * 8000000;
				while (return_code > 255)
					return_code -= 256;
			}
			g_sig.exit_status = return_code;
			exit(g_sig.exit_status);
		}
	}
	exit(g_sig.exit_status);
}
