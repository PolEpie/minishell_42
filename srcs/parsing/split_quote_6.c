/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/12/03 17:33:44 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_pars(t_pars *pars)
{
	int	i;
	t_list	*tmp;
	t_list	*tmp2;

	i = 0;
	tmp = *pars->ret;
	/* while (tmp)
	{
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	} */
	/* free(pars->ret); */
	/* if (pars->tmp_exec)
	{
		free(pars->tmp_exec->argv);
		free(pars->tmp_exec);
	} */
	free(pars);
}



int	sq_replace_and_free(t_list *elements, t_list **ret, t_ht *env)
{
	int		status;
	t_pars	*pars;

	pars = create_pars(ret, elements);
	if (!pars)
		return (1);
	while (pars->elements)
	{
		ft_printf("--\n");
		if (((t_splitted *)pars->elements->content)->is_delimiter)
		{
			status = handle_delimiter(pars, env);
			if (status == 1)
				return (free_pars(pars), 1);
			else if (status == 2)
				continue ;
			else if (status == 3)
				break ;
		}
		if (to_argv(pars))
			continue ;
	}
	return (end_replace(pars), 0);
}
