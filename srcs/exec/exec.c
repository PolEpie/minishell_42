/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/12/08 23:21:01 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_program_exec(char *path, char **argv, char **envp)
{
	char	*cmd_path;

	if (!path)
		return (exit(1), 1); // to print
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exit(127), 127); // to print
	else if (access(cmd_path, X_OK))
		return (exit(126), 126); // to print
	else if (execve(cmd_path, argv, envp) < 0)
		return (printf("here"), exit(1), 1);
	return (exit(0), 0);
}

int	run_program(char *path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if (!child)
		run_program_exec(path, argv, envp);
	waitpid(child, &status, 0);
	return (WEXITSTATUS(status));
}

int	select_exec(int argc, char **argv, t_ht *env, char **envp)
{
	if (!ft_strncmp(argv[0], "cd", 2))
		return (ft_cd(argc, argv, env));
	else if (!ft_strncmp(argv[0], "pwd", 3))
		return (ft_pwd(argc, argv));
	else if (!ft_strncmp(argv[0], "echo", 4))
		return (ft_echo(argc, argv));
	//else if (!ft_strncmp(argv[0], "cat", 3))
	//	return (ft_cat(argc, argv));
	else
		return (run_program(argv[0], argv, envp));
}

///////////////////////////////////////////
void	init_flags(t_flags *flags, t_list *splitted)
{
	int		i;
	t_exec	*temp;
	t_list	*start;

	start = splitted;

	flags->cmd_count = 0;
	flags->pipe_count = 0;
	flags->pid = NULL;
	flags->fd_in = NULL;
	flags->fd_out = NULL;

	while (splitted)
	{
		temp = splitted->content;
		//flags->cmd_count++;
		if (temp->token_next == PIPE)
			flags->pipe_count++;
		splitted = splitted->next;
	}
	splitted = start;

	flags->cmd_count = flags->pipe_count + 1;
	flags->cmd = (t_exec **)malloc(sizeof(t_exec *) * flags->cmd_count);
	//check malloc
	i = 0;
	while (splitted)
	{
		flags->cmd[i] = splitted->content;
		splitted = splitted->next;
		i++;
	}

	if (flags->pipe_count > 0)
	{
		flags->pid = ft_calloc(flags->cmd_count + 1, sizeof(pid_t));////
		flags->fd_in = ft_calloc(flags->cmd_count + 1, sizeof(int));//// prévoir un fd_in[0] pour le cas où il y a un fichier en entrée
		flags->fd_in = ft_memset(flags->fd_in, -1, sizeof(int) * flags->cmd_count);
		flags->fd_out = ft_calloc(flags->cmd_count + 1, sizeof(int));////
		flags->fd_out = ft_memset(flags->fd_out, -1, sizeof(int) * flags->cmd_count);
	}
}
///////////////////////////////////////////

int	parse_cmd(char *input, t_ht *env, char **envp)
{
	t_list	*splitted;
	t_flags	*flags;
	t_exec	*temp;
	int		res;

	res = 0;
	flags = malloc(sizeof(t_flags));
	if (!flags)
		return (0);// real exit
	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (0);
	free(input);
	init_flags(flags, splitted);
	printf("pipe_count: %d\n", flags->pipe_count);
	printf("cmd_count: %d\n", flags->cmd_count);
	if (flags->pipe_count > 0)
	{
		printf("pid: %d\n", flags->pid[0]);
		int i = 0;
		while (i < flags->cmd_count)
		{
			printf("cmd[%d]: %s\n", i, flags->cmd[i]->argv[0]);
			i++;
		}
		forking(flags, env);
	}
	else
	{
		while (splitted)
		{
			temp = splitted->content;
			printf("=====================\n");
			printf("1 argc %d\n", temp->argc);
			printf("1 argv1: %s\n", temp->argv[0]);
			printf("1 tokens: %d\n", temp->token_next); 
			splitted = splitted->next;
			if (ft_strcmp(temp->argv[0], "exit") == 0)
				return (exit_prog(&splitted, env));
			res = select_exec(temp->argc, temp->argv, env, envp);
		}
	}
	return (res);
}
