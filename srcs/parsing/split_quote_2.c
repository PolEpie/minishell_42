/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:33:44 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_string_quote(t_split_sh *sp)
{
	sp->i = 0;
	sp->str_start = 0;
	sp->quote_start = 0;
	sp->is_simp_quote = false;
	sp->is_dbl_quote = false;
	return (0);
}

int	count_until_del(t_list *ret)
{
	t_list		*e_tmp;
	t_splitted	*tmp;
	int			i;

	e_tmp = ret;
	i = 0;
	while (e_tmp)
	{
		tmp = e_tmp->content;
		if (tmp->is_delimiter)
			break ;
		e_tmp = e_tmp->next;
		i++;
	}
	return (i);
}

int	sq_replace_and_free(t_list *elements, t_list **ret)
{
	t_list		*e_tmp;
	t_splitted	*tmp;
	int			i;
	t_exec		*tmp_exec;

	i = 0;
	tmp_exec = malloc(sizeof(t_exec));
	if (!tmp_exec)
		return (1);
	tmp_exec->argc = count_until_del(elements);
	tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
	if (!tmp_exec->argv)
		return (1);
	while (elements)
	{
		tmp = elements->content;
		if (tmp->is_delimiter)
		{
			tmp_exec->token_next = tmp->delimiter;
			ft_lstadd_back(ret, ft_lstnew(tmp_exec));
			tmp_exec = malloc(sizeof(t_exec));
			if (!tmp_exec)
				return (1);
			elements = elements->next;
			tmp = elements->content;
			tmp_exec->argc = count_until_del(elements);
			tmp_exec->argv = malloc(sizeof(char *) * (tmp_exec->argc + 1));
			if (!tmp_exec->argv)
				return (1);
			i = 0;
			tmp = elements->content;
		}
		tmp_exec->argv[i] = tmp->content;
		e_tmp = elements->next;
		free(elements);
		i++;
		elements = e_tmp;
	}
	tmp_exec->token_next = -1;
	ft_lstadd_back(ret, ft_lstnew(tmp_exec));
	free(elements);
	return (0);
}
