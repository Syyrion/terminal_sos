#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "boardinterface.h"
#include "behavior.h"

int main()
{
    char restart;
    do
    {
        printf("\n==== SOS ====\n\n");
        if (initboard())
        {
            printf("Insufficient memory");
            return 1;
        }
    
        int (*opponent)();
        char *opponentname;

        {
            printf("\nSelect game mode:\n\ts - Singleplayer\n\tm - Mutiplayer\n\n");
            char gamemode;
            do
            {
                printf("Selection: ");
                gamemode = getchar() == '\n' ? getchar() : gamemode;
                gamemode = toupper(gamemode);
            } while (!(gamemode == 'S' || gamemode == 'M'));
            printf("\n");
            bool gm = gamemode == 'S';
            opponent = gm ? computerplayer : player;
            opponentname = gm ? "CPU" : "Player 2";
        }

        int playerscore = 0, opponentscore = 0;

        bool gamestate = false;

        printf("==== BEGIN ====\n\n");

        while (true)
        {
            printf("Status:\n\tPlayer 1 score - %d\n\t%s score - %d\n\tCurrent Turn - %s\n\n", playerscore, opponentname, opponentscore, gamestate ? opponentname: "Player 1");
            printboard();
            if ((gamestate ? *opponent : player)(gamestate ? &opponentscore : &playerscore))
            {
                if (isfilled())
                    break;
            }
            else
            {
                if (isfilled())
                    break;
                printf("== NEXT TURN ==\n\n");
                gamestate = !gamestate;
            }
        }


        printf("== GAME OVER ==\n\nResults:\n\tPlayer 1 score - %d\n\t%s score - %d\n\tOutcome - ", playerscore, opponentname, opponentscore);
        printf(playerscore == opponentscore ? "Tie game" : "%s Wins", playerscore > opponentscore ? "Player 1" : opponentname);
        printf("\n\n");
        printboard();
        delboard();
        printf("Restart? [y/n]: ");
        restart = getchar() == '\n' ? getchar() : restart;
        restart = toupper(restart);
    } while (restart == 'Y');
    printf("\n===== END ======\n\n");
    return 0;
}