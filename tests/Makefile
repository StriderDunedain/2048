CC := cc
CFLAGS := -Wall -Wextra -Werror -std=c11
ROOT := ..
LIBFT := $(ROOT)/lib/libft.a

APP_TEST := run_app_tests
APP_SRCS := $(ROOT)/src/app/app.c $(ROOT)/src/app/input.c \
	$(ROOT)/src/app/scores.c test_app.c stubs/stubs.c
APP_CPPFLAGS := -Istubs/include -I$(ROOT)/include \
	-DSCORE_FILE_PATH='"test_scores.tmp"'

UI_TEST := run_ui_tests
UI_RENDER_SRCS := $(wildcard $(ROOT)/src/ui/render/*.c)
UI_SRCS := $(ROOT)/src/ui/ui.c $(UI_RENDER_SRCS) test_ui.c
UI_CPPFLAGS := -I$(ROOT)/include -I$(ROOT)/src/ui \
	-I$(ROOT)/src/ui/render

ENGINE_TEST := run_engine_tests
ENGINE_SRCS := $(wildcard $(ROOT)/src/engine/*.c) test_engine.c
ENGINE_CPPFLAGS := -I$(ROOT)/include -I$(ROOT)/src/engine

all: $(APP_TEST) $(UI_TEST) $(ENGINE_TEST)
	./$(APP_TEST)
	./$(UI_TEST) >/dev/null
	./$(ENGINE_TEST)

$(APP_TEST): $(APP_SRCS)
	$(CC) $(APP_CPPFLAGS) $(CFLAGS) $(APP_SRCS) -o $@

$(UI_TEST): $(UI_SRCS) $(LIBFT)
	$(CC) $(UI_CPPFLAGS) $(CFLAGS) $(UI_SRCS) $(LIBFT) -lncurses -o $@

$(ENGINE_TEST): $(ENGINE_SRCS)
	$(CC) $(ENGINE_CPPFLAGS) $(CFLAGS) $(ENGINE_SRCS) -o $@

clean:
	rm -f $(APP_TEST) $(UI_TEST) $(ENGINE_TEST) test_scores.tmp

re: clean all

.PHONY: all clean re
