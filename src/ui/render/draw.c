/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
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
#include <unistd.h>
#include <wchar.h>

static wchar_t	*get_small_box(int num, t_layout *layout)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	pos;
	size_t	offset;
	wchar_t	*box;
	char	*nbr;

	j = 0;
	pos = 0;
	box = malloc((layout->cell_h * (layout->cell_w + 1) + 1) * sizeof(wchar_t));
	if (box == NULL)
		return (NULL);
	while (j < layout->cell_h)
	{
		k = 0;
		if (j == layout->cell_h / 2 && num != 0)
		{
			nbr = ft_itoa(num);
			if (nbr == NULL)
				return (free(box), NULL);
			offset = 0;
			if (layout->cell_w > ft_strlen(nbr))
				offset = (layout->cell_w - ft_strlen(nbr)) / 2;
			i = 0;
			while (k < layout->cell_w)
			{
				if (k >= offset && nbr[i])
					box[pos++] = (wchar_t)nbr[i++];
				else
					box[pos++] = L' ';
				k++;
			}
			free(nbr);
		}
		else
		{
			while (k < layout->cell_w)
			{
				box[pos++] = L' ';
				k++;
			}
		}
		box[pos++] = L'\n';
		j++;
	}
	box[pos] = L'\0';
	return (box);
}

static int	append_part(wchar_t **out, wchar_t *part)
{
	wchar_t	*temp;

	if (!part)
		return (free(*out), *out = NULL, 0);
	temp = *out;
	*out = multiline_join(temp, part);
	free(temp);
	free(part);
	return (*out != NULL);
}

static wchar_t	*get_small_row(int *row, size_t size, t_layout *layout)
{
	size_t	i;
	wchar_t	*out;

	out = get_vertical_separator(layout->cell_h);
	if (!out)
		return (NULL);
	i = 0;
	while (i < size)
	{
		if (!append_part(&out, get_small_box(row[i], layout)))
			return (NULL);
		if (!append_part(&out, get_vertical_separator(layout->cell_h)))
			return (NULL);
		i++;
	}
	return (out);
}

static wchar_t	*get_big_row(int *row, size_t size, t_layout *layout)
{
	size_t	i;
	wchar_t	*out;

	out = get_vertical_separator(layout->cell_h);
	if (!out)
		return (NULL);
	i = 0;
	while (i < size)
	{
		if (!append_part(&out, get_ascii_art_box(row[i], layout)))
			return (NULL);
		if (!append_part(&out, get_vertical_separator(layout->cell_h)))
			return (NULL);
		i++;
	}
	return (out);
}

static size_t	append_centered(wchar_t *out, size_t pos, wchar_t *row,
		size_t padding)
{
	size_t	i;

	while (*row)
	{
		i = 0;
		while (i < padding)
		{
			out[pos++] = L' ';
			i++;
		}
		while (*row && *row != L'\n')
			out[pos++] = *row++;
		if (*row == L'\n')
		{
			out[pos++] = *row;
			row++;
		}
	}
	return (pos);
}

static int	draw(int **map, size_t size, t_layout *layout, int small)
{
	wchar_t	*out;
	wchar_t	*row;
	size_t	pos;
	size_t	i;

	i = 0;
	out = malloc((layout->total_size + CLEAR_LEN) * sizeof(wchar_t));
	if (out == NULL)
		return (EXIT_FAILURE);
	ft_memcpy(out, CLEAR_SEQ, CLEAR_LEN * sizeof(wchar_t));
	pos = CLEAR_LEN;
	while (i < layout->pad_top)
	{
		out[pos++] = L'\n';
		i++;
	}
	i = 0;
	while (i < size * 2 + 1)
	{
		if (i % 2 == 0)
		{
			if (i == size * 2)
				row = get_grid_row(size, layout->cell_w, ROW_BOTTOM);
			else if (i == 0)
				row = get_grid_row(size, layout->cell_w, ROW_TOP);
			else
				row = get_grid_row(size, layout->cell_w, ROW_INSIDE);
		}
		else if (small)
			row = get_small_row(map[i / 2], size, layout);
		else
			row = get_big_row(map[i / 2], size, layout);
		if (row == NULL)
			return (free(out), EXIT_FAILURE);
		pos = append_centered(out, pos, row, layout->pad_left);
		free(row);
		i++;
	}
	out[pos] = L'\0';
	if (ft_putwstr_fd(out, STDOUT_FILENO) != EXIT_SUCCESS)
		return (free(out), EXIT_FAILURE);
	return (free(out), EXIT_SUCCESS);
}

int	draw_game_board(int **map, size_t size, size_t height, size_t width)
{
	t_layout	layout;

	if (!map)
		return (EXIT_FAILURE);
	if (!choose_layout(&layout, size, height, width))
	{
		write(2, "Window too small. Please resize the window.\n", 44);
		return (EXIT_FAILURE);
	}
	if (layout.cell_w == BIG_CELL_W)
		return (draw(map, size, &layout, 0));
	return (draw(map, size, &layout, 1));
}
