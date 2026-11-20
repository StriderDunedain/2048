
#include "engine.h"

static void board_move(int board[BOARD_SIZE][BOARD_SIZE], int control_arror, int score)
{
    switch (control_arror)
    {
    case 1:
        move_left(board, score);
        break;
    case 2:
        move_up(board, score);
        break;
    case 3:
        move_down(board, score);
        break;
    case 4:
        move_up(board, score);
        break;
    }
}

int game_engine()
{
    int board[BOARD_SIZE][BOARD_SIZE];
    int cpyboard[BOARD_SIZE][BOARD_SIZE];
    int success = 1;
    int control_arror = 0;
    int win = 0;
    int score = 0;

    ft_bzero(board, (BOARD_SIZE * BOARD_SIZE) * 4);
    pos_set(board);
    while (success)
    {
        if (success && ft_memcmp(cpyboard, board, BOARD_SIZE * BOARD_SIZE * 4))
        {
            pos_set(board);
            success = board_check(board);
        }
        if (success)
        {
            board_move(board, control_arror, score);
            ft_memcpy(cpyboard, board, BOARD_SIZE * BOARD_SIZE * 4);
        }
        if (success && !win)
        {
            win = win_check(board);
            if (win)
                write(0, "You win!!!!!", 12);
        }
    }
    // 
    // score save
    // 
    return (0);
}