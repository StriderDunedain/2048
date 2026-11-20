#include <limits.h>
#include <ncurses.h>
#include <stddef.h>
#include "app.h"
#include "libft.h"
#include "render.h"
#include "ui.h"

static void	centered_text(int row, const char *text, int highlighted)
{
	int	column;
	int	length;

	if (text == NULL || row < 0 || row >= LINES)
		return ;
	length = (int)ft_strlen(text);
	column = (COLS - length) / 2;
	if (column < 0)
		column = 0;
	if (highlighted)
		attron(A_REVERSE);
	mvaddnstr(row, column, text, COLS - column);
	if (highlighted)
		attroff(A_REVERSE);
}

void	ui_init(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}

void	ui_shutdown(void)
{
	SCREEN	*screen;

	endwin();
	screen = set_term(NULL);
	if (screen != NULL)
		delscreen(screen);
}

static int	copy_tiles(const t_game *game, int tiles[5][5], int *rows[5])
{
	int	row;
	int	column;

	if (game == NULL || (game->size != 4 && game->size != 5))
		return (0);
	row = 0;
	while (row < game->size)
	{
		rows[row] = tiles[row];
		column = 0;
		while (column < game->size)
		{
			if (game->tiles[row][column] > INT_MAX)
				tiles[row][column] = INT_MAX;
			else
				tiles[row][column] = (int)game->tiles[row][column];
			column++;
		}
		row++;
	}
	return (1);
}

void	ui_draw_game(const t_game *game, unsigned long long best_score)
{
	int	tiles[5][5];
	int	*rows[5];
	int	height;
	int	width;

	erase();
	refresh();
	if (!copy_tiles(game, tiles, rows))
	{
		centered_text(LINES / 2, "Invalid game board.", 0);
		refresh();
		return ;
	}
	getmaxyx(stdscr, height, width);
	if (draw_game_board(rows, (size_t)game->size, (size_t)height,
			(size_t)width) != 0)
	{
		erase();
		centered_text(LINES / 2,
			"Window too small. Please resize the window.", 0);
		refresh();
		return ;
	}
	mvprintw(height - 2, 0, "Score: %llu  Best: %llu", game->score,
		best_score);
	mvaddnstr(height - 1, 0, "Arrow keys: move  ESC: quit", width);
	refresh();
}

void	ui_draw_menu(int selection)
{
	static const char	*choices[APP_MENU_CHOICE_COUNT] = {
		"New Game 4x4", "New Game 5x5", "Quit"
	};
	int					row;
	int					index;

	erase();
	row = LINES / 2 - 3;
	centered_text(row, "2048", 0);
	index = 0;
	while (index < APP_MENU_CHOICE_COUNT)
	{
		centered_text(row + 2 + index, choices[index], index == selection);
		index++;
	}
	centered_text(row + 6, "Use Up/Down and Enter. ESC quits.", 0);
	refresh();
}

void	ui_draw_message(const char *message)
{
	int	row;

	row = LINES - 1;
	if (row < 0)
		return ;
	move(row, 0);
	clrtoeol();
	centered_text(row, message, 1);
	refresh();
}
