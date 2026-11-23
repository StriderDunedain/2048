/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascii-art.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:04:30 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:04:30 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "render.h"
#include <stdlib.h>

// Takes a digit character ('0'-'9'), e.g. one char of ft_itoa's output.
static wchar_t	*get_ascii_art_digit(char nb)
{
	if (nb == '0')
		return (L"┌─┐\n│ │\n└─┘\n");
	else if (nb == '1')
		return (L"  ╷\n  │\n  ╵\n");
	else if (nb == '2')
		return (L"╶─┐\n┌─┘\n└─╴\n");
	else if (nb == '3')
		return (L"╶─┐\n╶─┤\n╶─┘\n");
	else if (nb == '4')
		return (L"╷ ╷\n└─┤\n  ╵\n");
	else if (nb == '5')
		return (L"┌─╴\n└─┐\n╶─┘\n");
	else if (nb == '6')
		return (L"┌─╴\n├─┐\n└─┘\n");
	else if (nb == '7')
		return (L"╶─┐\n  │\n  ╵\n");
	else if (nb == '8')
		return (L"┌─┐\n├─┤\n└─┘\n");
	else if (nb == '9')
		return (L"┌─┐\n└─┤\n╶─┘\n");
	return (NULL);
}

static wchar_t	*get_vertical_padding(size_t height, t_layout *layout)
{
	size_t	i;
	size_t	j;
	size_t	pos;
	wchar_t	*padding;

	i = 0;
	pos = 0;
	padding = malloc((height * (layout->cell_w + 1) + 1) * sizeof(wchar_t));
	if (padding == NULL)
		return (NULL);
	while (i < height)
	{
		j = 0;
		while (j < layout->cell_w)
		{
			padding[pos] = L' ';
			j++;
			pos++;
		}
		padding[pos] = L'\n';
		i++;
		pos++;
	}
	padding[pos] = L'\0';
	return (padding);
}

static void	put_centered_art(wchar_t *box, size_t *pos, wchar_t *ascii,
		size_t cell_w)
{
	size_t	art_w;
	size_t	left;
	size_t	k;

	art_w = 0;
	while (ascii[art_w] && ascii[art_w] != L'\n')
		art_w++;
	left = 0;
	if (cell_w > art_w)
		left = (cell_w - art_w) / 2;
	while (*ascii)
	{
		k = 0;
		while (k < cell_w)
		{
			if (k >= left && *ascii && *ascii != L'\n')
				box[(*pos)++] = *ascii++;
			else
				box[(*pos)++] = L' ';
			k++;
		}
		while (*ascii && *ascii != L'\n')
			ascii++;
		if (*ascii == L'\n')
			ascii++;
		box[(*pos)++] = L'\n';
	}
}

wchar_t	*get_ascii_art_box(int nb, t_layout *layout)
{
	wchar_t	*box;
	wchar_t	*padding;
	size_t	pos;
	char	*digits;
	wchar_t	*ascii;
	wchar_t	*temp;
	size_t	i;

	if (nb == 0)
		return (get_vertical_padding(layout->cell_h, layout));
	i = 0;
	pos = 0;
	ascii = NULL;
	box = malloc((layout->cell_h * (layout->cell_w + 1) + 1) * sizeof(wchar_t));
	if (box == NULL)
		return (NULL);
	padding = get_vertical_padding((layout->cell_h - 3) / 2, layout);
	ft_memcpy(&box[pos], padding, ft_wstrlen(padding) * sizeof(wchar_t));
	pos += ft_wstrlen(padding);
	digits = ft_itoa(nb);
	if (digits == NULL)
		return (free(box), free(padding), NULL);
	while (digits[i])
	{
		temp = get_ascii_art_digit(digits[i]);
		if (temp == NULL)
		{
			if (i > 1)
				free(ascii);
			return (free(box), free(padding), free(digits), NULL);
		}
		if (!ascii)
			ascii = temp;
		else
		{
			temp = multiline_join(ascii, temp);
			if (i > 1)
				free(ascii);
			ascii = temp;
			if (ascii == NULL)
				return (free(box), free(padding), free(digits), NULL);
		}
		i++;
	}
	put_centered_art(box, &pos, ascii, layout->cell_w);
	if (i > 1)
		free(ascii);
	ft_memcpy(&box[pos], padding, ft_wstrlen(padding) * sizeof(wchar_t));
	pos += ft_wstrlen(padding);
	free(padding);
	free(digits);
	box[pos] = L'\0';
	return (box);
}
