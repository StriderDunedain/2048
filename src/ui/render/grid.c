/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:04:30 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:04:30 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>

static wchar_t	*get_row_chars(t_row type)
{
	if (type == ROW_TOP)
		return (L"┏┳┓");
	else if (type == ROW_INSIDE)
		return (L"┣╋┫");
	return (L"┗┻┛");
}

static wchar_t	get_row_char(size_t i, size_t len, size_t cell_w, t_row type)
{
	if (i % (cell_w + 1) != 0)
		return (L'━');
	else if (i == 0)
		return (get_row_chars(type)[0]);
	else if (i == len - 1)
		return (get_row_chars(type)[2]);
	return (get_row_chars(type)[1]);
}

wchar_t	*get_grid_row(size_t size, size_t cell_w, t_row type)
{
	wchar_t	*row;
	size_t	len;
	size_t	i;

	if (size == 0 || cell_w == 0)
		return (NULL);
	len = size * (cell_w + 1) + 1;
	row = malloc((len + 2) * sizeof(wchar_t));
	if (!row)
		return (NULL);
	i = 0;
	while (i < len)
	{
		row[i] = get_row_char(i, len, cell_w, type);
		i++;
	}
	row[len] = L'\n';
	row[len + 1] = L'\0';
	return (row);
}

wchar_t	*get_vertical_separator(size_t height)
{
	wchar_t	*sep;
	size_t	i;

	if (height == 0)
		return (NULL);
	sep = malloc((height * 2 + 1) * sizeof(wchar_t));
	if (!sep)
		return (NULL);
	i = 0;
	while (i < height)
	{
		sep[i * 2] = L'┃';
		sep[i * 2 + 1] = L'\n';
		i++;
	}
	sep[height * 2] = L'\0';
	return (sep);
}
