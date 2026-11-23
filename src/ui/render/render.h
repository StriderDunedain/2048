/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 13:59:21 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 13:59:21 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H
# include <stddef.h>

# define DIGIT_WIDTH 3
# define DIGIT_HEIGHT 3

# define TITLE_LINES 1
# define HELP_LINES 1
# define BIG_CELL_W 17
# define BIG_CELL_H 3
# define SMALL_CELL_W 7
# define SMALL_CELL_H 1

# define CLEAR_SEQ L"\033[H\033[J"
# define CLEAR_LEN 6

typedef enum e_row
{
	ROW_TOP,
	ROW_INSIDE,
	ROW_BOTTOM
}			t_row;

typedef struct s_layout
{
	size_t	cell_w;
	size_t	cell_h;
	size_t	pad_left;
	size_t	pad_top;
	size_t	total_size;
}			t_layout;

wchar_t		*get_ascii_art_box(int nb, t_layout *layout);
int			draw_game_board(int **map, size_t size, size_t height,
				size_t width);
int			choose_layout(t_layout *layout, size_t size, size_t height,
				size_t width);
wchar_t		*get_grid_row(size_t size, size_t cell_w, t_row type);
wchar_t		*get_vertical_separator(size_t height);
wchar_t		*multiline_join(wchar_t *s1, wchar_t *s2);
size_t		ft_wstrlen(wchar_t *str);
int			ft_putwstr_fd(wchar_t *str, int fd);

#endif
