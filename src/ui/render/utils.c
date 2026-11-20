/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:55:16 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:55:16 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>

size_t	ft_wstrlen(wchar_t *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

static size_t	count_newlines(wchar_t *str)
{
	size_t	count;

	if (!str)
		return (0);
	count = 0;
	while (*str)
	{
		if (*str == L'\n')
			count++;
		str++;
	}
	return (count);
}

// Only works for strings with equal line-count.
wchar_t	*multiline_join(wchar_t *s1, wchar_t *s2)
{
	wchar_t	*joined;
	size_t	i;

	i = 0;
	if (!s1 || !s2 || count_newlines(s1) != count_newlines(s2))
		return (NULL);
	joined = malloc((ft_wstrlen(s1) + ft_wstrlen(s2) - count_newlines(s1) + 1)
			* sizeof(wchar_t));
	if (joined == NULL)
		return (NULL);
	while (*s1 || *s2)
	{
		if (*s1 != '\n' && *s1)
			joined[i] = *s1++;
		else
		{
			joined[i] = *s2;
			if (*s2 == '\n')
				s1++;
			s2++;
		}
		i++;
	}
	joined[i] = '\0';
	return (joined);
}
