#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"
#include "search.h"

bool fallbackmode = false;

static char increment(bool, int *, int, int);
static int singlepass(int *);
static bool intermediatepass(void);
static void finalpass(void);

int computerplayer(int *score)
{
    // Match until no more can be made
    while (singlepass(score) > 0);
    // Find space that does not give SOS oppourtinities
    if (intermediatepass())
        finalpass();
    return false;
}

// Scans the board once and makes moves that gain points
// Increments score
// Returns number of points gained
static int singlepass(int *score)
{
    int gain = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (board[i][j] != ' ')
                continue;
    
            // Possible points gained
            int sgain = search(S_SCORE_MASK, S_SCORE_MASK_LENGTH, j, i), ogain = search(O_SCORE_MASK, O_SCORE_MASK_LENGTH, j, i);

            // Skip if no points can be gained
            if (sgain == 0 && ogain == 0)
                continue;

            if (sgain > 0 && ogain > 0)
            {
                // Both sgain and ogain are greater than 0
                // Priorotize choice that results in the most possible SOS setups
                int schain = search(S_CHAIN_MASK, S_CHAIN_MASK_LENGTH, j, i), ochain = search(O_CHAIN_MASK, O_CHAIN_MASK_LENGTH, j, i);
                board[i][j] = increment(schain > ochain, &gain, sgain, ogain);
            }
            else
            {
                // One or the other is equal to 0
                board[i][j] = increment(sgain > ogain, &gain, sgain, ogain);
            }
        }
    }
    *score += gain;
    return gain;
}

static char increment(bool expr, int *gain, int sgain, int ogain)
{
    *gain += expr ? sgain : ogain;
    return expr ? 'S' : 'O';
}

// Looks two steps ahead to not give opponent an advantage
// Returns true if no valid space is found
static bool intermediatepass()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (board[i][j] != ' ')
                continue;

            int schain = search(S_CHAIN_MASK, S_CHAIN_MASK_LENGTH, j, i), ochain = search(O_CHAIN_MASK, O_CHAIN_MASK_LENGTH, j, i);
            // No setups can be made
            if (schain == 0 && ochain == 0)
            {
                board[i][j] = (fallbackmode = !fallbackmode) ? 'S' : 'O';
                return false;
            }

            // Both letters create setups
            if (schain > 0 && ochain > 0)
                continue;
            
            // Only one letter can create setups
            board[i][j] = schain > ochain ? 'O': 'S';
            return false;
        }
    }
    return true;
}

// Scans for an empty space
static void finalpass()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = (fallbackmode = !fallbackmode) ? 'S' : 'O';
                return;
            }
        }
    }
}

// Ask for manual player input
int player(int *score)
{
    char c;
    do
    {
        printf("Select 's' or 'o': ");
        c = getchar() == '\n' ? getchar() : c;
        c = toupper(c);
    } while (!(c == 'S' || c == 'O'));

    int x, y;
    do
    {
        do
        {
            printf("Input X coordinate (1 - %d): ", WIDTH);
            scanf("%d", &x);
            x--;
        } while (x < 0 || x >= WIDTH);

        do
        {
            printf("Input Y coordinate (1 - %d): ", HEIGHT);
            scanf("%d", &y);
            y--;
        } while (y < 0 || y >= HEIGHT);
    } while (isoccupied(x, y));

    printf("\n");

    int inc;
    if ((board[y][x] = c) == 'S')
        inc = search(S_SCORE_MASK, S_SCORE_MASK_LENGTH, x, y);
    else
        inc = search(O_SCORE_MASK, O_SCORE_MASK_LENGTH, x, y);
    *score += inc;
    return inc;
}