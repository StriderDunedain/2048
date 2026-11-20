
#include "engine.h"

static void num_set(int board[BOARD_SIZE][BOARD_SIZE], int pos)
{
    int persent;

    srand(time(NULL));
    persent = rand() % 9;
    if (persent <= 7)
        board[pos / BOARD_SIZE][pos % BOARD_SIZE] = 2;
    else 
        board[pos / BOARD_SIZE][pos % BOARD_SIZE] = 4;
}

void pos_set(int board[BOARD_SIZE][BOARD_SIZE])
{
    int pos;
    int pos_zero[BOARD_SIZE * BOARD_SIZE];
    int size = 0;
    ft_bzero(pos_zero, BOARD_SIZE * BOARD_SIZE * 4);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                pos_zero[size] = i * BOARD_SIZE + j;
                size++;
            }
        }
    }
    if (size != 0)
    {
        srand(time(NULL));
        pos = pos_zero[rand() % size];
        num_set(board, pos);
    }
}

int win_check(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == WIN_VALUE)
                return (1);
        }
    }
    return (0);
}

int board_check(int board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            if (board[i][j] == 0 && board[i][j + 1] == 0)
                return (1);
            else if (board[i][j] == board[i][j + 1])
                return (1);
        }
    }
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        for (int i = 0; i < BOARD_SIZE - 1; i++)
        {
            if (board[i][j] == board[i + 1][j])
                return (1);
        }
    }   
    return (0);
}