/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 23:57:11 by pepie             #+#    #+#             */
/*   Updated: 2024/12/09 00:44:40 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <libgen.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

# include <sys/wait.h>

# ifdef LINUX
#  include <linux/limits.h>
# endif
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif

typedef enum e_tokens
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	HEREDOC,
	APPEND,
	PIPE
}	t_tokens;

typedef struct s_exec
{
	char		*cmd;
	int			argc;
	char		**argv;
	char		**envp;
	t_tokens	token_next;
}			t_exec;

typedef struct s_flags
{
	int		cmd_count;
	int		pipe_count;
	pid_t	*pid;
	int		*fd_in;
	int		*fd_out;
	t_exec 	**cmd;
}			t_flags;
// flag for if has file in start & end

typedef struct s_str_input
{
	char	*str;
	bool	is_double_quote;
	bool	is_simple_quote;
}	t_str_input;

typedef struct s_split_sh
{
	int		i;
	int		str_start;
	int		quote_start;
	bool	is_simp_quote;
	bool	is_dbl_quote;
}	t_split_sh;

typedef struct s_splitted
{
	bool	is_delimiter;
	void	*content;
	int		delimiter;
}	t_splitted;

/* main */
int		parse_cmd(char *input, t_ht *env, char **envp);
int		exit_prog(t_list **splitted, t_ht *env);

/* command/cd */
int		ft_cd(int argc, char **argv, t_ht *env);

/* command/pwd */
int		ft_pwd(int argc, char **argv);
char	*get_pwd(void);

/* command/echo */
int		ft_echo(int argc, char **argv);

/* command/cat */
int		ft_cat(int argc, char **argv);

/* split_quote */
t_list	*ft_split_quote(char const *str, t_ht *env);

/* split_quote_2 */
int		sq_replace_and_free(t_list *elements, t_list **ret);
int		init_string_quote(t_split_sh *sp);

/* parser/expansion */
void	register_env_var(t_ht *env, char **envp);
t_list	*create_str(char *str, bool is_simple_quote, t_ht *env);

/* parser/meta */
bool	is_meta(char c);
int		handle_meta(char const *str, t_split_sh *sp, t_list **elem);

/* utils */
int		ft_strarr_len(char **input);

/* exec/exec */
int		run_program_exec(char *path, char **argv, char **envp);
int		select_exec(int argc, char **argv, t_ht *env, char **envp);

/* exec/cmd_path */
char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

/* exec/exec_pipe */
void	forking(t_flags *flags, t_ht *env);

#endif