/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:41:03 by pepie             #+#    #+#             */
/*   Updated: 2024/05/24 11:09:48 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cat.h"

int	ft_cat(int argc, char**argv)
{
	int	i;

	i = 1;
	if (argc == 1 || (argc == 2 && argv[1][0] == '-'))
	{
		read_stdin();
		return (0);
	}
	while (i < argc)
	{
		read_file(argv[i]);
		i++;
	}
	return (0);
}
