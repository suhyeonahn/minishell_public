/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 20:00:59 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 15:04:39 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <limits.h>

typedef struct s_input
{
	char		*filename;
	int			fd;
	char		*in;
}				t_input;

typedef struct s_output
{
	char		*filename;
	int			fd;
	int			append;
}				t_output;

typedef struct s_cmd
{
	char		*content;
	char		*cmd;
	char		*hd_name;
	char		**argv;
	t_input		input;
	t_output	output;
}				t_cmd;

typedef struct s_cmd_node
{
	t_cmd				*cmd;
	struct s_cmd_node	*next;

}				t_cmd_node;

typedef struct s_cmd_list
{
	t_cmd_node			*start;
}				t_cmd_list;

typedef struct s_sig
{
	int				sigint;
	int				sigquit;
	int				exit_status;
	int				syntax_error;
	pid_t			pid;
}				t_sig;

// Used only by `parse_commands` to decrease number of lines
typedef struct s_parse_cmd
{
	int				i;
	int				last;
	char			quote;
	t_cmd_list		*cmds;
}				t_parse_cmd;

typedef struct s_tokenize_state
{
	int			i;
	int			last;
	int			in_token;
	t_cmd		*cmd;
}				t_tokenize_state;

typedef struct s_mini
{
	int				ret;
	int				cmd_i;
	char			*line;
	t_cmd_list		*cmds;
	// Should contain all fd we are using, like input/outputs currently setup
}				t_mini;

typedef struct s_node
{
	char			*s;
	int				is_quote;
	struct s_node	*next;
}				t_node;

typedef struct s_list
{
	t_node	*start;
}				t_list;

typedef struct s_props
{
	int		size;
	int		is_end;
}				t_props;

typedef struct s_quote
{
	int		i;
	int 	len;
	int		n_quote;
	char	**quotes_pos;
	char	*new;
	char	quote;
}				t_quote;

// main.c
void			test_list(t_node *node);
int				main(int argc, char **argv, char **env);

// parsing.c
t_cmd			*parse_command(char *str, char **env, int cmd_i);

// exec.c
void			exec_commands(t_mini *mini, char ***env);

// cmd_list.c
t_cmd_list		*new_t_cmd_list(void);
void			add_cmd_node_to_list(t_cmd_list *list, t_cmd_node *cmd_node);
void			print_cmd_list(t_cmd_list *list);
void			free_t_cmd(t_cmd *cmd);
t_cmd			*new_t_cmd(void);
t_cmd_node		*new_t_cmd_node(void);
void			free_t_cmd_node(t_cmd_node *cmd_node);
void			free_t_cmd_list(t_cmd_list *list);

// parse_cmd.c
t_parse_cmd		*new_t_parse_cmd(void);

// builtin.c
int				ft_echo(char **argv);
int				ft_cd(char **argv, char ***env);
int				ft_pwd(void);
int				ft_export(char **argv, char ***env);
int				ft_env(char ***env);
int				ft_unset(char **argv, char ***env);
int				ft_exit(char **argv);

// command_line.c
char			*write_line(char *str, char c);
int				check_builtins(t_cmd *cmd, char ***env);

// ctrl.c
void			ft_signal_handler(int sig_num);
void			check_signal(void);

// init.c
void			new_node(t_node *list, char *str, int quote);
void			init_l(t_list *list, char *str, int quote);
t_list			*create_list(void);
char			**init_new_env_table(char **env);

// ft_split.c
char			**ft_split(char const *s, char c);

// utils.c
size_t			ft_strlen(const char *str);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
char			*ft_substr(char *s, unsigned int start, size_t len);
//char			*ft_substr(char *src, int start, char end_c, t_props *p);
int				ft_strcmp(char *str1, char *str2);
int				ft_strncmp(char *str1, char *str2, int n);
char			*ft_strcpy(char *str);
int				init_quote(char *str, int start);
char			end_c(int quote);
void			free_node(t_node *node);
void			free_list(t_list *list);
void			put_strs_in_list(t_list *list, char *str);
char			*ft_strjoin(char *s1, char *s2);
int				int_len(long n);
char			*ft_itoa(int n);
int				ft_atoi(const char *str);
long long		ft_atoi_for_exit(const char *str);
int				is_digit(const char *str);

// var_utils.c
int				has_equal(char *str);
int				has_var(char *str);
char			*get_var_value(char **env, char *name);
char			**add_var(char **env, char *var);
char			**delete_var(char **env, char *var);
void			free_var(char **var);

// check_var.c
int				check_var_name(char *str);
int				check_dollar_var_name(char *token);

// env.c
void			modify_shlvl(char ***env);
void			modify_oldpwd(char ***env, char *str);
void			modify_pwd(char ***env, char *str);

//	dollar.c
char			*handle_dollar(char *str, char **env);
char			*var_without_white_space(char *src);

// quote.c
char			*remove_quote(char *str);
char			*remove_quotes(char *s);

extern t_sig	g_sig;

#endif
