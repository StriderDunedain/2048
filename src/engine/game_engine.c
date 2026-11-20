#include "engine_internal.h"

static int	valid_game(const t_game *game)
{
	return (game != 0 && (game->size == 4 || game->size == 5));
}

int	game_move(t_game *game, t_direction direction)
{
	if (!valid_game(game))
		return (0);
	if (direction == DIR_UP)
		return (move_up(game));
	if (direction == DIR_DOWN)
		return (move_down(game));
	if (direction == DIR_LEFT)
		return (move_left(game));
	if (direction == DIR_RIGHT)
		return (move_right(game));
	return (0);
}
