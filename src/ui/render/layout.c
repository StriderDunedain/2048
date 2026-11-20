/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:04:30 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:04:30 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	fit_layout(t_layout *layout, size_t size, size_t height,
		size_t width)
{
	size_t	grid_w;
	size_t	total_h;

	grid_w = size * (layout->cell_w + 1) + 1;
	total_h = size * (layout->cell_h + 1) + 1 + TITLE_LINES + HELP_LINES;
	if (width < grid_w || height < total_h)
		return (0);
	layout->pad_left = (width - grid_w) / 2;
	layout->pad_top = (height - total_h) / 2;
	layout->total_size = (layout->pad_top + total_h) * (width + 1) + 1;
	return (1);
}

int	choose_layout(t_layout *layout, size_t size, size_t height, size_t width)
{
	layout->cell_w = BIG_CELL_W;
	layout->cell_h = BIG_CELL_H;
	if (fit_layout(layout, size, height, width))
		return (1);
	layout->cell_w = SMALL_CELL_W;
	layout->cell_h = SMALL_CELL_H;
	return (fit_layout(layout, size, height, width));
}
