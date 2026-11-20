#ifndef UI_H
# define UI_H

# include <game.h>

void	ui_init(void);
void	ui_shutdown(void);
void	ui_draw_game(const t_game *game, unsigned long long best_score);
void	ui_draw_menu(int selection);
void	ui_draw_message(const char *message);

#endif
