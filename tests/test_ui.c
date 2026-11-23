#include <stdlib.h>
#include <unistd.h>
#include "render.h"

#define OUTPUT_CAPACITY 4096

static int	test_layouts(void)
{
	t_layout	layout;

	if (!choose_layout(&layout, 4, 30, 80))
		return (0);
	if (layout.cell_w != BIG_CELL_W || layout.cell_h != BIG_CELL_H)
		return (0);
	if (!choose_layout(&layout, 5, 13, 42))
		return (0);
	if (layout.cell_w != SMALL_CELL_W || layout.cell_h != SMALL_CELL_H)
		return (0);
	if (choose_layout(&layout, 5, 5, 10))
		return (0);
	return (1);
}

static int	capture_board(char *output, size_t capacity, int **rows,
		size_t size, size_t height, size_t width)
{
	int		pipes[2];
	int		saved_stdout;
	int		status;
	ssize_t	length;

	if (pipe(pipes) < 0)
		return (0);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout < 0)
		return (close(pipes[0]), close(pipes[1]), 0);
	if (dup2(pipes[1], STDOUT_FILENO) < 0)
		return (close(saved_stdout), close(pipes[0]), close(pipes[1]), 0);
	close(pipes[1]);
	status = draw_game_board(rows, size, height, width);
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		return (close(saved_stdout), close(pipes[0]), 0);
	close(saved_stdout);
	length = read(pipes[0], output, capacity);
	close(pipes[0]);
	return (status == EXIT_SUCCESS && length > 0);
}

static int	output_is_centered(const char *output)
{
	size_t	position;
	int		line;
	int		space;

	position = CLEAR_LEN;
	line = 0;
	while (line < 8)
	{
		if (output[position++] != '\r' || output[position++] != '\n')
			return (0);
		line++;
	}
	space = 0;
	while (space < 19)
	{
		if (output[position++] != ' ')
			return (0);
		space++;
	}
	return ((unsigned char)output[position] == 0xE2
		&& (unsigned char)output[position + 1] == 0x94
		&& (unsigned char)output[position + 2] == 0x8F);
}

static int	test_board_render(void)
{
	char	output[OUTPUT_CAPACITY];
	int	map[5][5];
	int	*rows[5];
	int	row;
	int	column;

	row = 0;
	while (row < 5)
	{
		rows[row] = map[row];
		column = 0;
		while (column < 5)
		{
			map[row][column] = 0;
			column++;
		}
		row++;
	}
	map[0][0] = 2;
	map[4][4] = 2048;
	if (!capture_board(output, OUTPUT_CAPACITY, rows, 5, 30, 80))
		return (0);
	if (!output_is_centered(output))
		return (0);
	return (capture_board(output, OUTPUT_CAPACITY, rows, 4, 30, 100));
}

int	main(void)
{
	if (!test_layouts() || !test_board_render())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
