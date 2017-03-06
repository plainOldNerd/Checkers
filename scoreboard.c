/***************************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COS1076 Advanced Programming Techniques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **************************************************************************/

#include "scoreboard.h"

/* Req 2 -Initialises the scoreboard and sets all the strings to be 
 * empty*/
void init_scoreboard(struct result * scoreboard)
{
    int i;
    for( i=0; i<SCOREBOARDSIZE; ++i )
    {
        char emptyString[MAX_NAME_LEN+1] = "\n\0";
        scoreboard[i].won[0] = '\0';
        scoreboard[i].lost[0] = '\0';
    }
}

/* Req 9 -Adds a result to the beginning of the scoreboard*/
void add_to_scoreboard(struct result * scoreboard, 
    struct result * latest_game)
{

    int i;

    for( i=SCOREBOARDSIZE-1; i>0; --i )
    {
        strcpy( scoreboard[i].won, scoreboard[i-1].won );
        strcpy( scoreboard[i].lost, scoreboard[i-1].lost );
    }
        strcpy( scoreboard[0].won, latest_game->won );
        strcpy( scoreboard[0].lost, latest_game->lost );
}

/* Req 11 - Initialises the scoreboard and sets all the strings to be 
 * empty*/
void reset_scoreboard(struct result * scoreboard)
{
    /* Delete this comment and write your own comments and code here
     * ummmmmmmmmmmmmmmmmmmm...
     * init_scoreboard( scoreboard );
     * ... ??!
     */
    
}

/* Req 10 - Displays a formatted printout of the last 10 winners and 
 * losers*/
void display_scoreboard(struct result * scoreboard)
{
    int i, j;

    printf( "   APT English Draughts Tournament - Last 10 Games \n" );
    printf( "   =============================================== \n" );
    printf( "  | Winner              | Loser               | \n" );
    printf( "  |---------------------|---------------------| \n" );
    for( i=0; i<SCOREBOARDSIZE; ++i )
    {
        if( i != SCOREBOARDSIZE-1 )
        {
            printf( " " );
        }
        printf( "%d| %s", i+1, scoreboard[i].won );
        for( j=0; j < MAX_NAME_LEN - strlen( scoreboard[i].won ); ++j )
        {
            printf( " " );
        }
        printf( "| %s", scoreboard[i].lost );
        for( j=0; j < MAX_NAME_LEN - strlen( scoreboard[i].lost ); ++j )
        {
            printf( " " );
        }
        printf( "| \n" );
    }
    printf( "  ================================================ \n" );

}

