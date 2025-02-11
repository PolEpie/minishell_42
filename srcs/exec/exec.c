/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2025/01/14 15:11:18 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_program_exec(char *path, char **argv, char **envp)
{
	char	*cmd_path;
	
	//if (path == NULL)////
	//	return (exec_error("failed to exec command", NULL), exit(1), 1);////
	cmd_path = get_cmd_path(path, get_path(envp));
	if (!cmd_path)
		return (exec_error("command not found", argv[0]), exit(127), 127);
	else if (access(cmd_path, X_OK))
		return (exec_error("permission denied", argv[0]), exit(126), 126);////check
	else if (execve(cmd_path, argv, envp) < 0)
		return (exec_error("failed to exec command", argv[0]), exit(1), 1);////check
	return (exit(0), 0);
}

int	run_program(char *path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if (child < 0)
		return (1);////real exit
	g_pid = child;
	if (!child)
		run_program_exec(path, argv, envp);
	waitpid(child, &status, 0);
	g_pid = 0;
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
	else if (!ft_strncmp(argv[0], "env", 3))
		return (ft_env(envp));
	else if (!ft_strncmp(argv[0], "unset", 5))
		return (ft_unset(argc, argv, env));
	else if (!ft_strncmp(argv[0], "export", 6))
		return (ft_export(argc, argv, env, envp));
	else
		return (run_program(argv[0], argv, envp));
}

void	free_splitted(void *v)
{
	t_splitted	*tmp_splitted;

	tmp_splitted = v;
	if (tmp_splitted)
	{
		free(tmp_splitted);
	}
}

void	free_splitted_c(void *v)
{
	t_splitted	*tmp_splitted;

	tmp_splitted = v;
	if (tmp_splitted)
	{
		if (tmp_splitted->content)
			free(tmp_splitted->content);
		free(tmp_splitted);
	}
}
void	free_splitted_wc(void *v)
{
	t_list	*splitted;
	t_exec	*tmp_exec;
	int		i;

	tmp_exec = v;
	i = 0;
	if (tmp_exec && tmp_exec->argv)
	{
		while (tmp_exec->argv[i])
		{
			free(tmp_exec->argv[i]);
			i++;
		}
		free(tmp_exec->argv);
		free(tmp_exec);
	}
}

int	exit_with_clear(t_list **splitted, t_ht *env, t_flags *flags, long long last_status)
{
	long	exit_status;

	exit_status = ft_exit(((t_exec *)(*splitted)->content)->argc, ((t_exec *)(*splitted)->content)->argv, last_status);
	if (((t_exec *)(*splitted)->content)->argc > 2 && exit_status == 1)
		return (1);
	free_flags(flags);
	ft_lstclear(splitted, &free_splitted_wc);
	exit_prog(splitted, env, exit_status);
	return (exit_status);
}

int	parse_cmd(char *input, t_ht *env, char **envp, int last_status)

{
	t_list	*splitted;
	t_flags	*flags;
	t_exec	*temp;
	int		res;
	char	**envp_cpy;


	res = 0;
	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (-1);
	free(input);

	////
	/*t_list	*temp_list = splitted;
	while (splitted)
	{
		ft_printf("splitted != NULL\n");////
		printf("((t_exec *)splitted->content)->argc: %d\n", ((t_exec *)splitted->content)->argc);////
		for (int i = 0; i < ((t_exec *)splitted->content)->argc; i++)
			printf("((t_exec *)splitted->content)->argv[%d]: %s\n", i, ((t_exec *)splitted->content)->argv[i]);////
		printf("((t_exec *)splitted->content)->token_next: %d\n", ((t_exec *)splitted->content)->token_next);////
		splitted = splitted->next;
	}
	splitted = temp_list;*/
	////

	//// < Makefile cat | wc -l > test
	flags = set_flags(splitted);

	/*printf("\n[FLAGS]\n\ntotal_redir: %d\n", flags->total_redir);////
	printf("pipe_index: %d\n", flags->pipe_index);////
	printf("pipe_nb: %d\n", flags->pipe_nb);////
	for (int i = 0; i < flags->pipe_nb && flags->total_redir > 0; i++)
	{
		printf("\n[INFILE]\n");////
		printf("pipe[%d]->infile_index: %d\n", i, flags->pipe[i]->infile_index);////
		printf("pipe[%d]->infile_nb: %d\n", i, flags->pipe[i]->infile_nb);////
		printf("[IN] infile\n");////
		for (int f = 0; f < flags->pipe[i]->infile_nb; f++)
		{
			printf("infile[%d]: %s\n", f, flags->pipe[i]->infile[f]->file);////
			printf("is_infile: %d\n", flags->pipe[i]->infile[f]->is_infile);////
			printf("is_outfile: %d\n", flags->pipe[i]->infile[f]->is_outfile);////
			printf("is_heredoc: %d\n", flags->pipe[i]->infile[f]->is_heredoc);////
			printf("is_append: %d\n", flags->pipe[i]->infile[f]->is_append);////
		}
		printf("[OUT] infile\n");////

		printf("\n[OUTFILE]\n");///
		printf("pipe[%d]->outfile_index: %d\n", i, flags->pipe[i]->outfile_index);////
		printf("pipe[%d]->outfile_nb: %d\n", i, flags->pipe[i]->outfile_nb);////
		printf("[IN] outfile\n");////
		for (int f = 0; f < flags->pipe[i]->outfile_nb; f++)
		{
			printf("outfile[%d]: %s\n", f, flags->pipe[i]->outfile[f]->file);////
			printf("is_infile: %d\n", flags->pipe[i]->outfile[f]->is_infile);////
			printf("is_outfile: %d\n", flags->pipe[i]->outfile[f]->is_outfile);////
			printf("is_heredoc: %d\n", flags->pipe[i]->outfile[f]->is_heredoc);////
			printf("is_append: %d\n", flags->pipe[i]->outfile[f]->is_append);////
		}
		printf("[OUT] outfile\n");////

		printf("\n[COMMAND]\n");////
		if (flags->pipe[i]->cmd)
		{
			printf("pipe[%d]->cmd->argc: %d\n", i, flags->pipe[i]->cmd->argc);////
			for (int j = 0; j < flags->pipe[i]->cmd->argc; j++)
				printf("pipe[%d]->cmd->argv[%d]: %s\n", i, j, flags->pipe[i]->cmd->argv[j]);////
		}
	}*/

	if (flags->multi_exec) {
		//printf("forking\n");////
		res = forking(flags, env, envp);
	}
	else
	{
		temp = splitted->content;
		if (ft_strcmp(temp->argv[0], "exit") == 0)
			return (exit_with_clear(&splitted, env, flags, last_status));
		envp_cpy = ht_to_envp(env);
		res = select_exec(temp->argc, temp->argv, env, envp_cpy);
		clear_env(envp_cpy);
	}
	//ft_lstclear(&splitted, &free_splitted_wc);
	//free_flags(flags);
	return (res);
}
 