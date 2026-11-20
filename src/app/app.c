#include "app.h"
#include "ui.h"
#include <ncurses.h>

static void	clear_app(t_app *app)
{
	int	row;
	int	column;

	row = 0;
	while (row < 5)
	{
		column = 0;
		while (column < 5)
			app->game.tiles[row][column++] = 0;
		row++;
	}
	app->game.size = 4;
	app->game.score = 0;
	app->scores.best_4 = 0;
	app->scores.best_5 = 0;
	app->state = APP_MENU;
	app->menu_selection = APP_MENU_NEW_4;
	app->win_acknowledged = 0;
	app->score_io_error = 0;
}

static unsigned long long	current_best(const t_app *app)
{
	if (app->game.size == 5)
		return (app->scores.best_5);
	return (app->scores.best_4);
}

static const char	*state_message(const t_app *app)
{
	if (app->state == APP_WON_PROMPT)
		return ("You won! Press Enter to continue.");
	if (app->state == APP_GAME_OVER)
		return ("Game over. Press Enter to return to the menu.");
	return (0);
}

static void	draw_state(const t_app *app)
{
	const char	*message;

	if (app->state == APP_MENU)
		ui_draw_menu(app->menu_selection);
	else if (app->state == APP_PLAYING || app->state == APP_WON_PROMPT
		|| app->state == APP_GAME_OVER)
		ui_draw_game(&app->game, current_best(app));
	message = state_message(app);
	if (message != 0 && app->state != APP_MENU && app->state != APP_EXIT)
		ui_draw_message(message);
	else if (app->score_io_error)
		ui_draw_message("High scores could not be loaded or saved.");
}

static void	finish_turn(t_app *app);

static void	start_game(t_app *app, int size)
{
	game_init(&app->game, size);
	app->win_acknowledged = 0;
	app->state = APP_PLAYING;
	finish_turn(app);
}

static void	handle_menu(t_app *app, t_input input)
{
	if (input.type == INPUT_ESCAPE)
		app->state = APP_EXIT;
	else if (input.type == INPUT_MOVEMENT && input.direction == DIR_UP)
		app->menu_selection = (app->menu_selection + APP_MENU_CHOICE_COUNT - 1)
			% APP_MENU_CHOICE_COUNT;
	else if (input.type == INPUT_MOVEMENT && input.direction == DIR_DOWN)
		app->menu_selection = (app->menu_selection + 1)
			% APP_MENU_CHOICE_COUNT;
	else if (input.type == INPUT_ENTER)
	{
		if (app->menu_selection == APP_MENU_NEW_4)
			start_game(app, 4);
		else if (app->menu_selection == APP_MENU_NEW_5)
			start_game(app, 5);
		else if (app->menu_selection == APP_MENU_QUIT)
			app->state = APP_EXIT;
	}
}

static void	record_score(t_app *app)
{
	if (scores_update(&app->scores, &app->game)
		&& scores_save(&app->scores) < 0)
		app->score_io_error = 1;
}

static void	finish_turn(t_app *app)
{
	if (!app->win_acknowledged && game_has_won(&app->game))
	{
		app->win_acknowledged = 1;
		app->state = APP_WON_PROMPT;
	}
	else if (!game_can_move(&app->game))
		app->state = APP_GAME_OVER;
}

static void	handle_playing(t_app *app, t_input input)
{
	if (input.type == INPUT_ESCAPE)
	{
		app->state = APP_EXIT;
		return ;
	}
	if (input.type != INPUT_MOVEMENT)
		return ;
	if (game_move(&app->game, input.direction))
	{
		game_spawn(&app->game);
		record_score(app);
	}
	finish_turn(app);
}

static void	handle_won_prompt(t_app *app, t_input input)
{
	if (input.type == INPUT_ESCAPE)
		app->state = APP_EXIT;
	else if (input.type == INPUT_ENTER)
	{
		if (game_can_move(&app->game))
			app->state = APP_PLAYING;
		else
			app->state = APP_GAME_OVER;
	}
}

static void	handle_game_over(t_app *app, t_input input)
{
	if (input.type == INPUT_ESCAPE)
		app->state = APP_EXIT;
	else if (input.type == INPUT_ENTER)
	{
		app->menu_selection = APP_MENU_NEW_4;
		app->state = APP_MENU;
	}
}

static void	dispatch_input(t_app *app, t_input input)
{
	if (input.type == INPUT_RESIZE)
		return ;
	if (app->state == APP_MENU)
		handle_menu(app, input);
	else if (app->state == APP_PLAYING)
		handle_playing(app, input);
	else if (app->state == APP_WON_PROMPT)
		handle_won_prompt(app, input);
	else if (app->state == APP_GAME_OVER)
		handle_game_over(app, input);
}

int	app_init(t_app *app)
{
	if (app == 0)
		return (-1);
	clear_app(app);
	if (scores_load(&app->scores) < 0)
		app->score_io_error = 1;
	return (0);
}

int	app_run(t_app *app)
{
	t_input	input;
	int		key;

	if (app == 0)
		return (-1);
	draw_state(app);
	while (app->state != APP_EXIT)
	{
		key = getch();
		input = input_translate(key);
		if (input.type != INPUT_RESIZE)
			app->score_io_error = 0;
		dispatch_input(app, input);
		draw_state(app);
	}
	return (0);
}
