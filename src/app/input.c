#include "app.h"
#include <ncurses.h>

int	input_to_direction(int key, t_direction *direction)
{
	if (direction == 0)
		return (0);
	if (key == KEY_UP)
		*direction = DIR_UP;
	else if (key == KEY_DOWN)
		*direction = DIR_DOWN;
	else if (key == KEY_LEFT)
		*direction = DIR_LEFT;
	else if (key == KEY_RIGHT)
		*direction = DIR_RIGHT;
	else
		return (0);
	return (1);
}

t_input	input_translate(int key)
{
	t_input	input;

	input.type = INPUT_IRRELEVANT;
	input.direction = DIR_UP;
	if (input_to_direction(key, &input.direction))
		input.type = INPUT_MOVEMENT;
	else if (key == '\n' || key == '\r' || key == KEY_ENTER)
		input.type = INPUT_ENTER;
	else if (key == 27)
		input.type = INPUT_ESCAPE;
	else if (key == KEY_RESIZE)
		input.type = INPUT_RESIZE;
	return (input);
}
