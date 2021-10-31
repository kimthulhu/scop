/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstrn.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: kde-wint <kde-wint@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/08/13 19:01:18 by kde-wint      #+#    #+#                 */
/*   Updated: 2021/08/04 17:25:30 by kde-wint      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft_internals.h"

void	ft_putstrn(char const *s, size_t n)
{
	write(1, s, n);
}