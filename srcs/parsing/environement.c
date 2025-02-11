/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Sifex <Sifex@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:53:35 by polepie           #+#    #+#             */
/*   Updated: 2024/12/06 17:00:47 by Sifex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_entry(char *out, char *key, char *val)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (key[i])
	{
		out[j] = key[i];
		i++;
		j++;
	}
	out[j] = '=';
	j++;
	i = 0;
	while (val[i])
	{
		out[j] = val[i];
		i++;
		j++;
	}
	out[j] = 0;
}

void	process_env(t_entry	*entry, int j, char **new_envp)
{
	char	*e;
	int		len;

	e = entry->value;
	len = ft_strlen(entry->key) + 2 + ft_strlen(e);
	new_envp[j] = malloc(sizeof(char) * (len));
	do_entry(new_envp[j], entry->key, e);
}

char	**ht_to_envp(t_ht *env)
{
	char	**new_envp;
	t_entry	*entry;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * (env->size + 1));
	if (!new_envp)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < env->size)
	{
		entry = env->elements[i];
		if (entry)
		{
			while (entry)
			{
				process_env(entry, j, new_envp);
				j++;
				entry = entry->next;
			}
		}
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	clear_envp(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}
