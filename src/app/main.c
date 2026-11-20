#include <stdlib.h>
#include <time.h>
#include "app.h"
#include "ui.h"

int	main(void)
{
	t_app	app;
	int		status;

	srand((unsigned int)time(NULL));
	if (app_init(&app) != 0)
		return (EXIT_FAILURE);
	ui_init();
	status = app_run(&app);
	ui_shutdown();
	if (status != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
