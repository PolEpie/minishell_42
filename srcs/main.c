/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: polepie <polepie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 07:51:42 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 19:18:14 by polepie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

char	*get_prefix(int last_status)
{
	char	*pwd;
	char	**splitted;
	char	*tmp;
	char	*status_str;

	pwd = get_pwd();
	status_str = ft_itoa(last_status);
	if (last_status == 0)
		tmp = ft_strjoin("\e[1;32m", status_str);
	else
		tmp = ft_strjoin("\e[1;31m", status_str);
	tmp = ft_strjoin_free(tmp, " \e[1;35m[CUSTOM] \e[1;33m");
	splitted = ft_split(pwd, '/');
	tmp = ft_strjoin_free(tmp,
			splitted[ft_strarr_len(splitted) - 1]);
	tmp = ft_strjoin_free(tmp, " > \e[0;37m");
	ft_freesplit(splitted);
	free(pwd);
	return (tmp);
}

int	run_program(char *path, char **argv)
{
	int	i;

	i = 0;
	printf("run program %s\n", path);
	printf("argv:\n");
	while (argv[i])
	{
		printf("%s\n", argv[i]);
		i++;
	}
	return (0);
}

int select_exec(char *prog, int argc, char **argv, t_ht *env)
{
	if (ft_strncmp(prog, "cd", 2) == 0)
		return (ft_cd(argc, argv, env));
	else if (ft_strncmp(prog, "pwd", 3) == 0)
		return (ft_pwd(argc, argv));
	else if (ft_strncmp(prog, "echo", 4) == 0)
		return (ft_echo(argc, argv));
	else if (ft_strncmp(prog, "exit", 5) == 0)
		return (exit(0), 0);
	else
		return (run_program(prog, argv));
}

int	parse_cmd(char *input, t_ht *env)
{
	char	**splitted;
	char	**argv;
	int		argc;
	int		res;

	splitted = ft_split_quote(input, env);
	if (!splitted)
		return (-1);
	free(input);
	argv = splitted;
	argv++;
	argc = ft_strarr_len(argv);
	res = select_exec(splitted[0], argc, argv, env);
	if (hashtable_search(env, "next_line"))
	{
		free(argv[0]);
		argv[0] = hashtable_search(env, "next_line");
		hashtable_delete(env, "next_line");
		parse_cmd(argv[0], env);
	}
	return (res);
}

int	main(int argc, char **argv, char **envp)
{
	char	*buffer;
	t_ht	*env;
	int		last_status;

	(void)argc;
	(void)argv;
	if (signal(SIGINT, handle_signals) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	if (signal(SIGQUIT, handle_signals) == SIG_ERR)
		printf("failed to register interrupts with kernel\n");
	env = hashtable_create(100);
	if (!env)
		return (printf("failed to malloc!"), 1);
	register_env_var(env, envp);
	buffer = readline(get_prefix(0));
	while (buffer != NULL)
	{
		if (buffer[0] != 0)
		{
			add_history(buffer);
			last_status = parse_cmd(buffer, env);
			hashtable_insert(env, "?", ft_uitoa(last_status));
		}
		buffer = readline(get_prefix(last_status));
	}
}
