
#ifndef ENGINE_H
# define ENGINE_H

# include "libft.h"
# include <time.h>

# define BOARD_SIZE 4

typedef struct s_block 
{
    int value;
    int line_no;
}              t_block;

enum e_const
{
    WIN_VALUE = 2048
};


void    move_down(int board[BOARD_SIZE][BOARD_SIZE], int score);
void    move_up(int board[BOARD_SIZE][BOARD_SIZE], int score);
void    move_left(int board[BOARD_SIZE][BOARD_SIZE], int score);
void    move_right(int board[BOARD_SIZE][BOARD_SIZE], int score);
void    pos_set(int board[BOARD_SIZE][BOARD_SIZE]);
int     win_check(int board[BOARD_SIZE][BOARD_SIZE]);
int     sboard_check(int board[BOARD_SIZE][BOARD_SIZE]);
int     game_engine(void);

#endif
