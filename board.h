#include <stdbool.h>

extern int WIDTH, HEIGHT;
extern char **board;

bool isfilled(void);
bool isoccupied(int, int);