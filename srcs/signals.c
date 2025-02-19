/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/01/13 18:46:22 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_noredisplay(int signo)
{
	if (signo == SIGINT)
	{
		g_sig = 130;
		printf("\n");
		rl_reset_line_state();
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	handle_sigint(int signo)
{
	if (signo == SIGINT)
	{
		g_sig = 130;
		printf("\n");
		rl_reset_line_state();
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigint_null(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		rl_redisplay();
		exit(130);
	}
}

void	sigquit_handler(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("Quit: ", 1);
		ft_putnbr_fd(signo, 1);
		ft_putstr_fd(" (core dumped)\n", 1);
		g_sig = 131;
	}
}
