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

int	open_infile(int index, int index_file, t_flags *flags)
{
	int		fd;
	char	*file;

	if (flags->fd_in[index] != -1)
		close(flags->fd_in[index]);
	file = flags->pipe[flags->pipe_index]->infile[index_file]->file;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (file_error(strerror(errno), file), 0);
	if (index_file + 1 != flags->pipe[flags->pipe_index]->infile_nb)
		close(fd);
	else
		flags->fd_in[index] = fd;
	return (1);
}

int	open_outfile(int index, int index_file, t_flags *flags)
{
	char	*file;

	if (flags->fd_out[index] != -1)
		close(flags->fd_out[index]);
	file = flags->pipe[flags->pipe_index]->outfile[index_file]->file;
	if (flags->pipe[flags->pipe_index]->outfile[index_file]->is_append)
		flags->fd_out[index] = open(file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		flags->fd_out[index] = open(file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (flags->fd_out[index] < 0)
		return (file_error(strerror(errno), file), 0);
	return (1);
}

static void	open_heredoc_while(int fd[2], char *heredoc)
{
	char	*line;

	signal(SIGINT, handle_sigint_noredisplay);
	while (1)
	{
		signal(SIGINT, handle_sigint_null);
		line = readline(">");
		if (!line || (!ft_strncmp(line, heredoc, ft_strlen(heredoc))
				&& !line[ft_strlen(heredoc)]))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
}

int	open_heredoc(int index, int index_file, t_flags *flags)
{
	int		fd[2];
	char	*heredoc;

	if (pipe(fd) < 0)
		return (0);
	heredoc = flags->pipe[flags->pipe_index]->infile[index_file]->file;
	open_heredoc_while(fd, heredoc);
	close(fd[1]);
	if (index_file + 1 != flags->pipe[flags->pipe_index]->infile_nb)
		close(fd[0]);
	else
		flags->fd_in[index] = fd[0];
	return (1);
}
