#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int WIDTH, HEIGHT;
char **board;

// Allocates memory for board
bool initboard()
{
    do
    {
        printf("Input board width: ");
        scanf("%d", &WIDTH);
    } while (WIDTH < 3);
    do
    {
        printf("Input board height: ");
        scanf("%d", &HEIGHT);
    } while (HEIGHT < 3);

    board = malloc(HEIGHT * sizeof *board);
    if (!board) return true;
    for (int i = 0; i < HEIGHT; i++)
    {
        board[i] = malloc(WIDTH * sizeof *board[0]);
        if (!board[i]) return true;
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = ' ';
    }
    return false;
}

// Frees the memory from the board
void delboard()
{
    for (int i = 0; i < HEIGHT; i++)
        free(board[i]);
    free(board);
}

// Prints the board
void printboard()
{
    int hlabelwidth = 0;
    for (int i = HEIGHT; i > 0; i /= 10)
        hlabelwidth++;
    printf("Board:\n\t");
    for (int i = 0; i < hlabelwidth; i++) printf(" ");
    printf("|");

    // Print the secondary header if necessary
    if (WIDTH >= 10)
    {
        printf("                 ");
        for (int i = 10, lim = WIDTH - 10; i <= lim; i += 10)
        {
            for (int j = 19 - printf("|%d+", i); j >= 0; j--)
                printf(" ");
        }
        printf("|\n\t");
        for (int i = 0; i < hlabelwidth; i++) printf(" ");
        printf("|");
    }

    // Print the primary header
    for (int i = 1; i <= WIDTH; i++)
    {
        int mod = i % 10;
        printf("%d%c", mod, mod == 9 || i == WIDTH ? '|' : ' ');
    }
    printf("\n");

    // Print the rest
    char hlabel[4 + hlabelwidth];
    sprintf(hlabel, "\t%%%dd", hlabelwidth);
    for (int i = 0; i < HEIGHT; i++)
    {
        printf(hlabel, i + 1);
        for (int j = 0; j < WIDTH; j++)
        {
            printf("|%c", board[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

// Checks if the entire board is filled
bool isfilled()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            char c = board[i][j];
            if (c == ' ')
            {
                return false;
            }
        }
    }
    return true;
}

// Checks if a single space is occupied
bool isoccupied(int x, int y)
{
    return board[y][x] != ' ';
}