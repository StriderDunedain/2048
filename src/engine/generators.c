#include <stdlib.h>
#include "game.h"

static int	valid_game(const t_game *game)
{
	return (game != 0 && (game->size == 4 || game->size == 5));
}

int	game_spawn(t_game *game)
{
	int	empty[25];
	int	count;
	int	row;
	int	column;
	int	position;

	if (!valid_game(game))
		return (0);
	count = 0;
	row = 0;
	while (row < game->size)
	{
		column = 0;
		while (column < game->size)
		{
			if (game->tiles[row][column] == 0)
				empty[count++] = row * game->size + column;
			column++;
		}
		row++;
	}
	if (count == 0)
		return (0);
	position = empty[rand() % count];
	game->tiles[position / game->size][position % game->size] = 2;
	if (rand() % 10 == 0)
		game->tiles[position / game->size][position % game->size] = 4;
	return (1);
}

void	game_init(t_game *game, int size)
{
	int	row;
	int	column;

	if (game == 0)
		return ;
	if (size != 4 && size != 5)
		size = 4;
	row = 0;
	while (row < 5)
	{
		column = 0;
		while (column < 5)
			game->tiles[row][column++] = 0;
		row++;
	}
	game->size = size;
	game->score = 0;
	game_spawn(game);
	game_spawn(game);
}

int	game_can_move(const t_game *game)
{
	int	row;
	int	column;

	if (!valid_game(game))
		return (0);
	row = 0;
	while (row < game->size)
	{
		column = 0;
		while (column < game->size)
		{
			if (game->tiles[row][column] == 0)
				return (1);
			if (column + 1 < game->size && game->tiles[row][column]
				== game->tiles[row][column + 1])
				return (1);
			if (row + 1 < game->size && game->tiles[row][column]
				== game->tiles[row + 1][column])
				return (1);
			column++;
		}
		row++;
	}
	return (0);
}

int	game_has_won(const t_game *game)
{
	int	row;
	int	column;

	if (!valid_game(game) || WIN_VALUE <= 0
		|| (WIN_VALUE & (WIN_VALUE - 1)) != 0)
		return (0);
	row = 0;
	while (row < game->size)
	{
		column = 0;
		while (column < game->size)
		{
			if (game->tiles[row][column] == WIN_VALUE)
				return (1);
			column++;
		}
		row++;
	}
	return (0);
}
