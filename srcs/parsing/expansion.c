/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:18:12 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 11:59:12 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_last_status(char *str, t_ht *env, int i)
{
	char	*tmp;
	char	*res;

	res = hashtable_search(env, "?");
	if (!res)
		res = "0";
	tmp = ft_strndup(str, i);
	tmp = ft_strjoin_free(tmp, res);
	tmp = ft_strjoin_free(tmp, &str[i + 2]);
	free(str);
	return (tmp);
}

char	*handle_env_var(char **s, t_ht *env, int i)
{
	char	*env_var;
	char	*tmp;
	char	*res;
	char	*str;
	int		j;

	str = *s;
	j = i + 1;
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	env_var = ft_strndup(&str[i + 1], j - i - 1);
	tmp = ft_strndup(str, i);
	res = hashtable_search(env, env_var);
	if (!res)
		res = "";
	tmp = ft_strjoin_free(tmp, res);
	tmp = ft_strjoin_free(tmp, &str[j]);
	free(env_var);
	free(str);
	return (tmp);
}

char	*handle_expansion(char *str, t_ht *env)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			str = handle_last_status(str, env, i);
			i++;
		}
		else if (str[i] == '$' )
		{
			str = handle_env_var(&str, env, i);
		}
		i++;
	}
	return (str);
}

t_list	*create_str(char *str, bool is_simple_quote, t_ht *env)
{
	if (!is_simple_quote)
		return (ft_lstnew(handle_expansion(str, env)));
	return (ft_lstnew(str));
}

void	register_env_var(t_ht *env, char **envp)
{
	char	**splitted;
	int		i;

	i = 0;
	while (envp[i])
	{
		splitted = ft_split(envp[i], '=');
		if (!splitted)
			return ;
		hashtable_insert(env, splitted[0], splitted[1]);
		i++;
	}
}
