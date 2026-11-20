#ifndef APP_H
# define APP_H

# include <game.h>
# include "scores.h"

/* These indices are the menu contract shared with the UI implementation. */
typedef enum e_app_menu_choice
{
	APP_MENU_NEW_4 = 0,
	APP_MENU_NEW_5 = 1,
	APP_MENU_QUIT = 2,
	APP_MENU_CHOICE_COUNT = 3
}t_app_menu_choice;

typedef enum e_app_state
{
	APP_MENU,
	APP_PLAYING,
	APP_WON_PROMPT,
	APP_GAME_OVER,
	APP_EXIT
}t_app_state;

typedef enum e_input_type
{
	INPUT_IRRELEVANT,
	INPUT_MOVEMENT,
	INPUT_ENTER,
	INPUT_ESCAPE,
	INPUT_RESIZE
}t_input_type;

typedef struct s_input
{
	t_input_type	type;
	t_direction	direction;
}t_input;

typedef struct s_app
{
	t_game		game;
	t_scores	scores;
	t_app_state	state;
	int			menu_selection;
	int			win_acknowledged;
	int			score_io_error;
}t_app;

int			app_init(t_app *app);
int			app_run(t_app *app);
int			input_to_direction(int key, t_direction *direction);
t_input		input_translate(int key);

#endif
