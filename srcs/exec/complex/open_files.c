/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:01:34 by Sifex             #+#    #+#             */
/*   Updated: 2025/01/16 16:01:34 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(int index, t_flags *flags)
{
	char	*file;

	file = flags->pipe[flags->pipe_index]->infile[flags->pipe[flags->pipe_index]->infile_index]->file;
	flags->fd_in[index] = open(file, O_RDONLY);
	if (flags->fd_in[index] < 0)
		return (file_error(strerror(errno), file), 0);
	return (1);
}

/*int	open_outfile(t_flags *flags)
{
	if (flags->has_append)
		flags->fd_out[flags->cmd_count - 1] = open(flags->outfile, O_WRONLY | O_CREAT | O_APPEND, 0667);//// 0667~ 
	else
		flags->fd_out[flags->cmd_count - 1] = open(flags->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0667);//// 0667~
	if (flags->fd_out[flags->cmd_count - 1] < 0)
		return (file_error(strerror(errno), flags->outfile), 0);
	return (1);
}

void	open_heredoc(t_flags *flags)
{
	int		fd[2];
	char	*line;
	int		len_heredoc;

	if (pipe(fd) < 0)
		exit(1);////real exit
	g_pid = fork();
	if (g_pid < 0)
		exit(1);////real exit
	if (!g_pid)
	{
		len_heredoc = ft_strlen(flags->heredoc);
		if (signal(SIGQUIT, handle_signals_heredoc) == SIG_ERR)
			printf("failed to register interrupts with kernel\n");
		if (signal(SIGINT, handle_signals_heredoc) == SIG_ERR)
			printf("failed to register interrupts with kernel\n");
		while (1) 
		{
			line = readline("> ");
			if (!line || (!ft_strncmp(line, flags->heredoc, len_heredoc) && !line[len_heredoc]))
			{
				free(line);
				break ;
			}
			ft_putstr_fd(line, fd[1]);
			ft_putstr_fd("\n", fd[1]);
			free(line);
		}
		close(fd[1]);
		close(fd[0]);
		exit(0);
	}
	else
	{
		waitpid(g_pid, NULL, 0);
		close(fd[1]);
		flags->fd_in[0] = fd[0];
		g_pid = 0;
	}
}*/
//// close pipe ?

