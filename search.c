#include <stdbool.h>
#include "board.h"

#define _O_SCORE_MASK_LENGTH 4
#define _S_SCORE_MASK_LENGTH 8

#define _O_CHAIN_MASK_LENGTH 8
#define _S_CHAIN_MASK_LENGTH 16

static bool partialmatch(int, int, int[2][3]);
static bool isvalid(int, int, char);

enum symbol {_S, _O, _SPACE};

// Lookup table to find SOSs after an O is placed
int O_SCORE_MASK_LENGTH = _O_SCORE_MASK_LENGTH;
int O_SCORE_MASK[_O_SCORE_MASK_LENGTH][2][3] = {
    // Horizontal
    {
        {-1, 0, _S},
        {1, 0, _S},
    },
    // Vertical
    {
        {0, -1, _S},
        {0, 1, _S},
    },
    // NW to SE Diagonal
    {
        {-1, -1, _S},
        {1, 1, _S},
    },
    // NE to SW Diagonal
    {
        {1, -1, _S},
        {-1, 1, _S},
    }
};

// Lookup table to find SOSs after an S is placed
int S_SCORE_MASK_LENGTH = _S_SCORE_MASK_LENGTH;
int S_SCORE_MASK[_S_SCORE_MASK_LENGTH][2][3] = {
    // N
    {
        {0, -2, _S},
        {0, -1, _O}
    },
    // NE
    {
        {2, -2, _S},
        {1, -1, _O}
    },
    // E
    {
        {2, 0, _S},
        {1, 0, _O}
    },
    // SE
    {
        {2, 2, _S},
        {1, 1, _O}
    },
    // S
    {
        {0, 2, _S},
        {0, 1, _O}
    },
    // SW
    {
        {-2, 2, _S},
        {-1, 1, _O}
    },
    // W
    {
        {-2, 0, _S},
        {-1, 0, _O}
    },
    // NW
    {
        {-2, -2, _S},
        {-1, -1, _O}
    }
};

// Lookup table to find possible SOSs surrounding an O after it's placed
int O_CHAIN_MASK_LENGTH = _O_CHAIN_MASK_LENGTH;
int O_CHAIN_MASK[_O_CHAIN_MASK_LENGTH][2][3] = {
    // N
    {
        {0, -1, _S},
        {0, 1, _SPACE}
    },
    // NE
    {
        {1, -1, _S},
        {-1, 1, _SPACE}
    },
    // E
    {
        {1, 0, _S},
        {-1, 0, _SPACE}
    },
    // SE
    {
        {1, 1, _S},
        {-1, -1, _SPACE}
    },
    // S
    {
        {0, 1, _S},
        {0, -1, _SPACE}
    },
    // SW
    {
        {-1, 1, _S},
        {1, -1, _SPACE}
    },
    // W
    {
        {-1, 0, _S},
        {1, 0, _SPACE}
    },
    // NW
    {
        {-1, -1, _S},
        {1, 1, _SPACE}
    }
};

// Lookup table to find possible SOSs surrounding an S after it's placed
int S_CHAIN_MASK_LENGTH = _S_CHAIN_MASK_LENGTH;
int S_CHAIN_MASK[_S_CHAIN_MASK_LENGTH][2][3] = {
    //N
    {
        {0, -1, _O},
        {0, -2, _SPACE}
    },
    {
        {0, -2, _S},
        {0, -1, _SPACE}
    },
    //NE
    {
        {1, -1, _O},
        {2, -2, _SPACE}
    },
    {
        {2, -2, _S},
        {1, -1, _SPACE}
    },
    //E
    {
        {1, 0, _O},
        {2, 0, _SPACE}
    },
    {
        {2, 0, _S},
        {1, 0, _SPACE}
    },
    //SE
    {
        {1, 1, _O},
        {2, 2, _SPACE}
    },
    {
        {2, 2, _S},
        {1, 1, _SPACE}
    },
    //S
    {
        {0, 1, _O},
        {0, 2, _SPACE}
    },
    {
        {0, 2, _S},
        {0, 1, _SPACE}
    },
    //SW
    {
        {-1, 1, _O},
        {-2, 2, _SPACE}
    },
    {
        {-2, 2, _S},
        {-1, 1, _SPACE}
    },
    //W
    {
        {-1, 0, _O},
        {-2, 0, _SPACE}
    },
    {
        {-2, 0, _S},
        {-1, 0, _SPACE}
    },
    //NW
    {
        {-1, -1, _O},
        {-2, -2, _SPACE}
    },
    {
        {-2, -2, _S},
        {-1, -1, _SPACE}
    },
};

// Uses a lookup table and returns the amount of matches found
int search(int (*lookup)[2][3], int length, int x, int y)
{
    int matches = 0;
    for (int i = 0; i < length; i++)
        matches += partialmatch(x, y, lookup[i]);
    return matches;
}

static bool partialmatch(int x, int y, int ofs[2][3])
{
    // Check two relevant spaces
    for (int i = 0; i < 2; i++)
    {
        // Check if surrounding space is valid
        char c;
        switch (ofs[i][2])
        {
        case _S:
            c = 'S';
            break;
        case _O:
            c = 'O';
            break;
        case _SPACE:
            c = ' ';
            break;
        }
        if (!isvalid(x + ofs[i][0], y + ofs[i][1], c))
            return false;
    }
    return true;
}

// Returns true if x and y is in bounds and matches
static bool isvalid(int x, int y, char c)
{
    return x >= 0 && x < WIDTH &&
           y >= 0 && y < HEIGHT &&
           board[y][x] == c;
}