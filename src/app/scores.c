#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include "scores.h"

#ifndef SCORE_FILE_PATH
# define SCORE_FILE_PATH ".2048_scores"
#endif

#define SCORE_BUFFER_SIZE 64

static int	append_number(char *buffer, int position, unsigned long long value)
{
	char	digits[20];
	int	count;

	count = 0;
	if (value == 0)
		digits[count++] = '0';
	while (value != 0)
	{
		digits[count++] = (char)('0' + value % 10);
		value /= 10;
	}
	while (count > 0)
		buffer[position++] = digits[--count];
	return (position);
}

static int	format_scores(char *buffer, const t_scores *scores)
{
	int	position;

	position = 0;
	buffer[position++] = '4';
	buffer[position++] = ' ';
	position = append_number(buffer, position, scores->best_4);
	buffer[position++] = '\n';
	buffer[position++] = '5';
	buffer[position++] = ' ';
	position = append_number(buffer, position, scores->best_5);
	buffer[position++] = '\n';
	return (position);
}

static int	write_all(int fd, const char *buffer, int length)
{
	ssize_t	written;
	int		position;

	position = 0;
	while (position < length)
	{
		written = write(fd, buffer + position, (size_t)(length - position));
		if (written <= 0)
			return (-1);
		position += (int)written;
	}
	return (0);
}

static int	write_scores(int fd, const t_scores *scores)
{
	char	buffer[SCORE_BUFFER_SIZE];
	int		length;
	int		status;

	length = format_scores(buffer, scores);
	status = write_all(fd, buffer, length);
	if (close(fd) < 0)
		status = -1;
	return (status);
}

int	scores_save(const t_scores *scores)
{
	int	fd;

	if (scores == 0)
		return (-1);
	fd = open(SCORE_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	return (write_scores(fd, scores));
}

static int	create_score_file(const t_scores *scores)
{
	int	fd;

	fd = open(SCORE_FILE_PATH, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd < 0)
		return (-1);
	return (write_scores(fd, scores));
}

static int	read_file(char *buffer, int capacity)
{
	ssize_t	amount;
	int		fd;
	int		length;

	fd = open(SCORE_FILE_PATH, O_RDONLY);
	if (fd < 0)
		return (-1);
	length = 0;
	while (length < capacity)
	{
		amount = read(fd, buffer + length, (size_t)(capacity - length));
		if (amount < 0)
		{
			close(fd);
			return (-1);
		}
		if (amount == 0)
			break ;
		length += (int)amount;
	}
	if (close(fd) < 0 || length == capacity)
		return (-1);
	return (length);
}

static int	parse_number(const char *buffer, int length, int *position,
	unsigned long long *value)
{
	unsigned long long	result;
	unsigned int		digit;

	result = 0;
	if (*position >= length || buffer[*position] < '0'
		|| buffer[*position] > '9')
		return (-1);
	while (*position < length && buffer[*position] >= '0'
		&& buffer[*position] <= '9')
	{
		digit = (unsigned int)(buffer[*position] - '0');
		if (result > (ULLONG_MAX - digit) / 10)
			return (-1);
		result = result * 10 + digit;
		(*position)++;
	}
	*value = result;
	return (0);
}

static int	parse_record(const char *buffer, int length, int *position,
	char size, unsigned long long *value)
{
	if (*position + 2 > length || buffer[*position] != size
		|| buffer[*position + 1] != ' ')
		return (-1);
	*position += 2;
	if (parse_number(buffer, length, position, value) < 0)
		return (-1);
	if (*position >= length || buffer[*position] != '\n')
		return (-1);
	(*position)++;
	return (0);
}

int	scores_load(t_scores *scores)
{
	char		buffer[SCORE_BUFFER_SIZE];
	t_scores	loaded;
	int			length;
	int			position;

	if (scores == 0)
		return (-1);
	scores->best_4 = 0;
	scores->best_5 = 0;
	length = read_file(buffer, SCORE_BUFFER_SIZE);
	if (length < 0)
		return (create_score_file(scores));
	position = 0;
	if (parse_record(buffer, length, &position, '4', &loaded.best_4) < 0
		|| parse_record(buffer, length, &position, '5', &loaded.best_5) < 0
		|| position != length)
		return (-1);
	*scores = loaded;
	return (0);
}

int	scores_update(t_scores *scores, const t_game *game)
{
	unsigned long long	*record;

	if (scores == 0 || game == 0)
		return (0);
	if (game->size == 4)
		record = &scores->best_4;
	else if (game->size == 5)
		record = &scores->best_5;
	else
		return (0);
	if (game->score <= *record)
		return (0);
	*record = game->score;
	return (1);
}
