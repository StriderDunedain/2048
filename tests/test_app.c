#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "app.h"
#include "ncurses.h"
#include "test_stubs.h"

static int	g_failures;

#define CHECK(condition) check((condition), #condition, __FILE__, __LINE__)

static void	check(int condition, const char *text, const char *file, int line)
{
	if (!condition)
	{
		fprintf(stderr, "%s:%d: check failed: %s\n", file, line, text);
		g_failures++;
	}
}

static void	init_app(t_app *app, t_app_state state)
{
	memset(app, 0, sizeof(*app));
	app->game.size = 4;
	app->state = state;
	app->menu_selection = APP_MENU_NEW_4;
	app->scores.best_4 = ULLONG_MAX;
	app->scores.best_5 = ULLONG_MAX;
}

static void	test_input(void)
{
	t_direction	direction;

	CHECK(input_to_direction(KEY_UP, &direction) && direction == DIR_UP);
	CHECK(input_to_direction(KEY_DOWN, &direction) && direction == DIR_DOWN);
	CHECK(input_to_direction(KEY_LEFT, &direction) && direction == DIR_LEFT);
	CHECK(input_to_direction(KEY_RIGHT, &direction) && direction == DIR_RIGHT);
	CHECK(input_translate('x').type == INPUT_IRRELEVANT);
	CHECK(input_translate(27).type == INPUT_ESCAPE);
	CHECK(input_translate(KEY_RESIZE).type == INPUT_RESIZE);
	CHECK(input_translate('\n').type == INPUT_ENTER);
	CHECK(input_translate(KEY_ENTER).type == INPUT_ENTER);
}

static void	test_irrelevant_key(void)
{
	t_app	app;
	t_game	before;

	stub_reset();
	init_app(&app, APP_PLAYING);
	app.game.tiles[2][2] = 64;
	app.game.score = 321;
	before = app.game;
	stub_push_key('x');
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.move_calls == 0);
	CHECK(g_stub_stats.spawn_calls == 0);
	CHECK(memcmp(&app.game, &before, sizeof(before)) == 0);
}

static void	test_escape(void)
{
	t_app		app;
	t_app_state	states[4];
	int			index;

	states[0] = APP_MENU;
	states[1] = APP_PLAYING;
	states[2] = APP_WON_PROMPT;
	states[3] = APP_GAME_OVER;
	index = 0;
	while (index < 4)
	{
		stub_reset();
		init_app(&app, states[index]);
		stub_push_key(27);
		CHECK(app_run(&app) == 0);
		CHECK(app.state == APP_EXIT);
		index++;
	}
}

static void	test_spawn_rules(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_PLAYING);
	stub_push_key(KEY_LEFT);
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.move_calls == 1);
	CHECK(g_stub_stats.spawn_calls == 0);
	stub_reset();
	init_app(&app, APP_PLAYING);
	g_stub_move_result = 1;
	stub_push_key(KEY_RIGHT);
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.move_calls == 1);
	CHECK(g_stub_stats.spawn_calls == 1);
}

static void	test_victory_continue(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_PLAYING);
	g_stub_move_result = 1;
	g_stub_has_won = 1;
	stub_push_key(KEY_LEFT);
	stub_push_key('\n');
	stub_push_key(KEY_RIGHT);
	stub_push_key(27);
	app_run(&app);
	CHECK(app.win_acknowledged == 1);
	CHECK(g_stub_stats.won_messages == 1);
	CHECK(g_stub_stats.move_calls == 2);
	CHECK(g_stub_stats.spawn_calls == 2);
}

static void	test_initial_victory(void)
{
	t_app	app;
	int		size;

	size = 4;
	while (size <= 5)
	{
		stub_reset();
		init_app(&app, APP_MENU);
		g_stub_has_won = 1;
		if (size == 5)
			stub_push_key(KEY_DOWN);
		stub_push_key('\n');
		stub_push_key(27);
		app_run(&app);
		CHECK(g_stub_stats.last_init_size == size);
		CHECK(app.win_acknowledged == 1);
		CHECK(g_stub_stats.won_messages == 1);
		CHECK(g_stub_stats.move_calls == 0);
		size++;
	}
}

static void	test_game_over(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_PLAYING);
	g_stub_can_move = 0;
	stub_push_key(KEY_LEFT);
	stub_push_key('\n');
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.game_over_messages == 1);
	CHECK(g_stub_stats.menu_draws >= 1);
}

static void	test_menu_sizes(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_MENU);
	stub_push_key('\n');
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.last_init_size == 4);
	stub_reset();
	init_app(&app, APP_MENU);
	stub_push_key(KEY_DOWN);
	stub_push_key('\n');
	stub_push_key(27);
	app_run(&app);
	CHECK(g_stub_stats.last_init_size == 5);
}

static void	test_resize(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_PLAYING);
	app.game.tiles[2][3] = 128;
	app.game.score = 99;
	stub_push_key(KEY_RESIZE);
	stub_push_key(27);
	app_run(&app);
	CHECK(app.game.tiles[2][3] == 128);
	CHECK(app.game.score == 99);
	CHECK(g_stub_stats.init_calls == 0);
	CHECK(g_stub_stats.game_draws >= 2);
}

static void	test_win_value(void)
{
	t_app	app;

	stub_reset();
	init_app(&app, APP_PLAYING);
	app.game.tiles[0][0] = 2048;
	g_stub_move_result = 1;
	stub_push_key(KEY_LEFT);
	stub_push_key(27);
	app_run(&app);
	CHECK(WIN_VALUE != 2048);
	CHECK(g_stub_stats.won_messages == 0);
}

static void	test_session_scores(void)
{
	t_scores	scores;
	t_game		game;

	memset(&scores, 0, sizeof(scores));
	memset(&game, 0, sizeof(game));
	game.size = 4;
	game.score = 999;
	CHECK(scores_update(&scores, &game) == 1);
	CHECK(scores.best_4 == 999 && scores.best_5 == 0);
	CHECK(scores_update(&scores, &game) == 0);
	game.size = 5;
	game.score = 1000;
	CHECK(scores_update(&scores, &game) == 1);
	CHECK(scores.best_4 == 999 && scores.best_5 == 1000);
}

static void	write_score_file(const char *contents)
{
	FILE	*file;

	file = fopen("test_scores.tmp", "w");
	CHECK(file != NULL);
	if (file == NULL)
		return ;
	CHECK(fputs(contents, file) >= 0);
	CHECK(fclose(file) == 0);
}

static void	test_score_files(void)
{
	t_scores	scores;

	remove("test_scores.tmp");
	scores.best_4 = 44;
	scores.best_5 = 55;
	CHECK(scores_load(&scores) == 0);
	CHECK(scores.best_4 == 0 && scores.best_5 == 0);
	scores.best_4 = 123456789ULL;
	scores.best_5 = ULLONG_MAX;
	CHECK(scores_save(&scores) == 0);
	scores.best_4 = 0;
	scores.best_5 = 0;
	CHECK(scores_load(&scores) == 0);
	CHECK(scores.best_4 == 123456789ULL);
	CHECK(scores.best_5 == ULLONG_MAX);
	write_score_file("4 42\n5");
	CHECK(scores_load(&scores) == -1);
	CHECK(scores.best_4 == 0 && scores.best_5 == 0);
	write_score_file("not a score file\n");
	CHECK(scores_load(&scores) == -1);
	CHECK(scores.best_4 == 0 && scores.best_5 == 0);
	remove("test_scores.tmp");
	CHECK(mkdir("test_scores.tmp", 0700) == 0);
	CHECK(scores_save(&scores) == -1);
	CHECK(rmdir("test_scores.tmp") == 0);
}

int	main(void)
{
	test_input();
	test_irrelevant_key();
	test_escape();
	test_spawn_rules();
	test_victory_continue();
	test_initial_victory();
	test_game_over();
	test_menu_sizes();
	test_resize();
	test_win_value();
	test_session_scores();
	test_score_files();
	if (g_failures != 0)
	{
		fprintf(stderr, "%d test check(s) failed\n", g_failures);
		return (1);
	}
	printf("all app/input/persistent-score tests passed\n");
	return (0);
}
