#include <stdio.h>
#include <string.h>
#include "game.h"

static int	g_failures;

#define CHECK(condition) check((condition), #condition, __FILE__, __LINE__)

static void	check(int condition, const char *text, const char *file, int line)
{
	if (!condition)
	{
		fprintf(stderr, "%s:%d: check failed: %s\n", file, line, text);
		g_failures++;
	}
}

static void	clear_game(t_game *game, int size)
{
	memset(game, 0, sizeof(*game));
	game->size = size;
}

static int	count_tiles(const t_game *game)
{
	int	row;
	int	column;
	int	count;

	row = 0;
	count = 0;
	while (row < game->size)
	{
		column = 0;
		while (column < game->size)
		{
			if (game->tiles[row][column] != 0)
				count++;
			column++;
		}
		row++;
	}
	return (count);
}

static void	test_initialization(void)
{
	t_game	game;
	int		row;
	int		column;

	game_init(&game, 4);
	CHECK(game.size == 4 && game.score == 0);
	CHECK(count_tiles(&game) == 2);
	row = 0;
	while (row < game.size)
	{
		column = 0;
		while (column < game.size)
		{
			CHECK(game.tiles[row][column] == 0
				|| game.tiles[row][column] == 2
				|| game.tiles[row][column] == 4);
			column++;
		}
		row++;
	}
	game_init(&game, 5);
	CHECK(game.size == 5 && count_tiles(&game) == 2);
	game_init(&game, 3);
	CHECK(game.size == 4 && count_tiles(&game) == 2);
}

static void	test_left_merges(void)
{
	t_game	game;

	clear_game(&game, 4);
	game.tiles[0][1] = 2;
	game.tiles[0][2] = 2;
	CHECK(game_move(&game, DIR_LEFT) == 1);
	CHECK(game.tiles[0][0] == 4 && game.tiles[0][1] == 0);
	CHECK(game.score == 4 && count_tiles(&game) == 1);
	clear_game(&game, 4);
	game.tiles[0][0] = 2;
	game.tiles[0][1] = 2;
	game.tiles[0][2] = 2;
	game.tiles[0][3] = 2;
	CHECK(game_move(&game, DIR_LEFT) == 1);
	CHECK(game.tiles[0][0] == 4 && game.tiles[0][1] == 4);
	CHECK(game.tiles[0][2] == 0 && game.score == 8);
	clear_game(&game, 4);
	game.tiles[0][0] = 4;
	game.tiles[0][1] = 4;
	game.tiles[0][2] = 8;
	CHECK(game_move(&game, DIR_LEFT) == 1);
	CHECK(game.tiles[0][0] == 8 && game.tiles[0][1] == 8);
	CHECK(game.score == 8);
}

static void	test_all_directions(void)
{
	t_game	game;

	clear_game(&game, 5);
	game.tiles[3][2] = 8;
	game.tiles[1][2] = 8;
	CHECK(game_move(&game, DIR_UP) == 1);
	CHECK(game.tiles[0][2] == 16 && game.score == 16);
	clear_game(&game, 5);
	game.tiles[0][3] = 4;
	game.tiles[2][3] = 4;
	CHECK(game_move(&game, DIR_DOWN) == 1);
	CHECK(game.tiles[4][3] == 8 && game.score == 8);
	clear_game(&game, 4);
	game.tiles[1][0] = 2;
	game.tiles[1][2] = 2;
	CHECK(game_move(&game, DIR_RIGHT) == 1);
	CHECK(game.tiles[1][3] == 4 && game.score == 4);
	clear_game(&game, 4);
	game.tiles[0][0] = 2;
	game.tiles[0][1] = 4;
	game.tiles[0][2] = 8;
	game.tiles[0][3] = 16;
	CHECK(game_move(&game, DIR_LEFT) == 0);
	CHECK(game.score == 0 && count_tiles(&game) == 4);
}

static void	test_spawn(void)
{
	t_game	game;
	int		before;

	clear_game(&game, 4);
	before = count_tiles(&game);
	CHECK(game_spawn(&game) == 1);
	CHECK(count_tiles(&game) == before + 1);
	memset(game.tiles, 1, sizeof(game.tiles));
	CHECK(game_spawn(&game) == 0);
}

static void	test_end_conditions(void)
{
	t_game	game;
	int		row;
	int		column;

	clear_game(&game, 4);
	CHECK(game_can_move(&game) == 1);
	row = 0;
	while (row < game.size)
	{
		column = 0;
		while (column < game.size)
		{
			game.tiles[row][column] = 2 << ((row + column) % 2);
			column++;
		}
		row++;
	}
	CHECK(game_can_move(&game) == 0);
	game.tiles[0][1] = game.tiles[0][0];
	CHECK(game_can_move(&game) == 1);
	CHECK(game_has_won(&game) == 0);
	game.tiles[2][2] = WIN_VALUE;
	CHECK(game_has_won(&game) == 1);
}

int	main(void)
{
	test_initialization();
	test_left_merges();
	test_all_directions();
	test_spawn();
	test_end_conditions();
	if (g_failures != 0)
	{
		fprintf(stderr, "%d engine test check(s) failed\n", g_failures);
		return (1);
	}
	printf("all engine tests passed\n");
	return (0);
}
