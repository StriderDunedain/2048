NAME := 2048
CC := cc
CFLAGS := -Wall -Wextra -Werror -std=c11
CPPFLAGS := -Iinclude -Isrc/ui -Isrc/ui/render
LDLIBS := lib/libft.a -lncurses

APP_SRCS := $(wildcard src/app/*.c)
ENGINE_SRCS := $(wildcard src/engine/*.c)
UI_SRCS := $(wildcard src/ui/*.c) $(wildcard src/ui/render/*.c)
SRCS := $(APP_SRCS) $(ENGINE_SRCS) $(UI_SRCS)
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) lib/libft.a
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
