#include <string.h>
#include "ncurses.h"
#include "test_stubs.h"
#include "ui.h"

#define STUB_KEY_CAPACITY 128

t_stub_stats	g_stub_stats;
int			g_stub_move_result;
int			g_stub_can_move;
int			g_stub_has_won;
unsigned long long	g_stub_score_gain;

static int	g_keys[STUB_KEY_CAPACITY];
static int	g_key_count;
static int	g_key_index;

void	stub_reset(void)
{
	memset(&g_stub_stats, 0, sizeof(g_stub_stats));
	g_stub_move_result = 0;
	g_stub_can_move = 1;
	g_stub_has_won = 0;
	g_stub_score_gain = 4;
	g_key_count = 0;
	g_key_index = 0;
}

void	stub_push_key(int key)
{
	if (g_key_count < STUB_KEY_CAPACITY)
		g_keys[g_key_count++] = key;
}

int	getch(void)
{
	if (g_key_index < g_key_count)
		return (g_keys[g_key_index++]);
	return (27);
}

void	game_init(t_game *game, int size)
{
	memset(game, 0, sizeof(*game));
	game->size = size;
	game->tiles[0][0] = 2;
	game->tiles[0][1] = 2;
	g_stub_stats.init_calls++;
	g_stub_stats.last_init_size = size;
}

int	game_move(t_game *game, t_direction direction)
{
	(void)direction;
	g_stub_stats.move_calls++;
	if (!g_stub_move_result)
		return (0);
	game->score += g_stub_score_gain;
	game->tiles[1][1]++;
	return (1);
}

int	game_spawn(t_game *game)
{
	g_stub_stats.spawn_calls++;
	game->tiles[game->size - 1][game->size - 1] = 2;
	return (1);
}

int	game_can_move(const t_game *game)
{
	(void)game;
	return (g_stub_can_move);
}

int	game_has_won(const t_game *game)
{
	(void)game;
	return (g_stub_has_won);
}

void	ui_init(void)
{
}

void	ui_shutdown(void)
{
}

void	ui_draw_game(const t_game *game, unsigned long long best_score)
{
	(void)game;
	(void)best_score;
	g_stub_stats.game_draws++;
}

void	ui_draw_menu(int selection)
{
	g_stub_stats.menu_draws++;
	g_stub_stats.last_menu_selection = selection;
}

void	ui_draw_message(const char *message)
{
	g_stub_stats.message_draws++;
	if (strstr(message, "You won!") != NULL)
		g_stub_stats.won_messages++;
	if (strstr(message, "Game over") != NULL)
		g_stub_stats.game_over_messages++;
}
