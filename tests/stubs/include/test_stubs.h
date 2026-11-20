#ifndef TEST_STUBS_H
# define TEST_STUBS_H

# include "game.h"

typedef struct s_stub_stats
{
	int	init_calls;
	int	last_init_size;
	int	move_calls;
	int	spawn_calls;
	int	game_draws;
	int	menu_draws;
	int	message_draws;
	int	won_messages;
	int	game_over_messages;
	int	last_menu_selection;
}t_stub_stats;

extern t_stub_stats	g_stub_stats;
extern int			g_stub_move_result;
extern int			g_stub_can_move;
extern int			g_stub_has_won;
extern unsigned long long	g_stub_score_gain;

void	stub_reset(void);
void	stub_push_key(int key);

#endif
