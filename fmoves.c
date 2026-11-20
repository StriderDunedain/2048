
#include "engine.h"

void    move_left(int board[BOARD_SIZE][BOARD_SIZE], int score)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            for (int k = j + 1; k < BOARD_SIZE; k++)
            {
                if (board[i][k] == 0)
                    continue;
                else if (board[i][j] == board[i][k] || board [i][j] == 0)
                {
                    board[i][j] += board[i][k];
                    if ((board[i][k] != 0 && board[i][j] != 0))
                        score += board[i][j];
                    board[i][k] = 0;
                    break;
                }
                else 
                    break;
            }
        }
    }
}

void    move_right(int board[BOARD_SIZE][BOARD_SIZE], int score)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = BOARD_SIZE - 1; 0 <= j; j--)
        {
            for (int k = j - 1; 0 <= k; k--)
            {
                if (board[i][k] == 0)
                    continue;
                else if (board[i][j] == board[i][k] || board [i][j] == 0)
                {
                    board[i][j] += board[i][k];
                    if ((board[i][k] != 0 && board[i][j] != 0))
                        score += board[i][j];
                    board[i][k] = 0;
                    break;
                }
                else 
                    break;
            }
        }
    }
}

void    move_up(int board[BOARD_SIZE][BOARD_SIZE], int score)
{
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int k = i + 1; k < BOARD_SIZE; k++)
            {
                if (board[i][k] == 0)
                    continue;
                else if (board[i][j] == board[k][j] || board [i][j] == 0)
                {
                    board[i][j] += board[k][j];
                    if ((board[i][k] != 0 && board[i][j] != 0))
                        score += board[i][j];
                    board[k][j] = 0;
                    break;
                }
                else 
                    break;
            }
        }
    }
}

void    move_down(int board[BOARD_SIZE][BOARD_SIZE], int score)
{
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        for (int i = BOARD_SIZE - 1; 0 <= i; i--)
        {
            for (int k = i - 1; 0 <= k; k--)
            {
                if (board[i][k] == 0)
                    continue;
                else if (board[i][j] == board[i][k] || board [i][j] == 0)
                {
                    board[i][j] += board[i][k];
                    if ((board[i][k] != 0 && board[i][j] != 0))
                        score += board[i][j];
                    board[i][k] = 0;
                    break;
                }
                else 
                    break;
            }
        }
    }
}