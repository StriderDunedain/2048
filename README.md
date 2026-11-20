# 2048

Terminal implementation of 2048 using C and ncurses.

## Build and run

```sh
make
./2048
```

Use the arrow keys to move tiles, Enter to select menu options, and Escape to
exit from any screen. The menu supports 4x4 and 5x5 games. Best scores are
stored separately in `.2048_scores` in the directory where the game is run.

## Development tests

```sh
make -C tests
```
