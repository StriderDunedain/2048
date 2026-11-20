#include "engine_internal.h"

static int	move_line(t_game *game, int row, int column,
	int row_step, int column_step)
{
	unsigned int	values[5];
	unsigned int	result[5];
	unsigned int	value;
	int				count;
	int				index;
	int				output;
	int				changed;

	index = 0;
	count = 0;
	while (index < game->size)
	{
		value = game->tiles[row + index * row_step]
		[column + index * column_step];
		if (value != 0)
			values[count++] = value;
		result[index++] = 0;
	}
	index = 0;
	output = 0;
	while (index < count)
	{
		if (index + 1 < count && values[index] == values[index + 1])
		{
			result[output] = values[index] + values[index + 1];
			game->score += result[output];
			index += 2;
		}
		else
			result[output] = values[index++];
		output++;
	}
	changed = 0;
	index = 0;
	while (index < game->size)
	{
		value = game->tiles[row + index * row_step]
		[column + index * column_step];
		if (value != result[index])
			changed = 1;
		game->tiles[row + index * row_step]
		[column + index * column_step] = result[index];
		index++;
	}
	return (changed);
}

int	move_left(t_game *game)
{
	int	row;
	int	changed;

	row = 0;
	changed = 0;
	while (row < game->size)
	{
		changed |= move_line(game, row, 0, 0, 1);
		row++;
	}
	return (changed);
}

int	move_right(t_game *game)
{
	int	row;
	int	changed;

	row = 0;
	changed = 0;
	while (row < game->size)
	{
		changed |= move_line(game, row, game->size - 1, 0, -1);
		row++;
	}
	return (changed);
}

int	move_up(t_game *game)
{
	int	column;
	int	changed;

	column = 0;
	changed = 0;
	while (column < game->size)
	{
		changed |= move_line(game, 0, column, 1, 0);
		column++;
	}
	return (changed);
}

int	move_down(t_game *game)
{
	int	column;
	int	changed;

	column = 0;
	changed = 0;
	while (column < game->size)
	{
		changed |= move_line(game, game->size - 1, column, -1, 0);
		column++;
	}
	return (changed);
}
