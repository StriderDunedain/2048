#ifndef GAME_H
# define GAME_H

/* Deliberately differs from 2048 so tests catch controller-side hardcoding. */
enum e_const
{
	WIN_VALUE = 4096
};

typedef enum e_direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
}t_direction;

typedef struct s_game
{
	unsigned int			tiles[5][5];
	int					size;
	unsigned long long	score;
}t_game;

void	game_init(t_game *game, int size);
int		game_move(t_game *game, t_direction direction);
int		game_spawn(t_game *game);
int		game_can_move(const t_game *game);
int		game_has_won(const t_game *game);

#endif
