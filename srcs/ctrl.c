/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:38:08 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:16:14 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_signal_handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		if (rl_on_new_line() == -1)
			exit(1);
		write(1, "\n", 1);
		rl_replace_line("", 1); // Wipe off typed line in prompt
		rl_redisplay(); // Cursor in prompt doesn't move
	}
}

void	check_signal(void)
{
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
