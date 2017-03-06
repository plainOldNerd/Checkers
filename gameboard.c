/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COSC1076 Advanced Programming Techniques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **********************************************************************/
#include "gameboard.h"

/* Copies the master board to a local copy for each game */
void init_gameboard(enum cell_contents board[][BOARDWIDTH])
{
    int i, j;

    for( i=0; i < BOARDHEIGHT; ++i )
    {
        for( j=0; j < BOARDWIDTH; ++j )
        {
            board[i][j] = master_board[i][j];
        }
    }
}

/* Prints the game board to the screen */
void display_gameboard(enum cell_contents board[][BOARDWIDTH])
{
    int i, j;

    printf( " |0|1|2|3|4|5|6|7| \n" );
    printf( "------------------ \n" );
    for( i=0; i < BOARDHEIGHT; ++i )
    {
        printf( "%d|", i );
        for( j=0; j < BOARDWIDTH; ++j )
        {
            switch( board[i][j] )
            {
                case EMPTY:
                    {
                        printf( " |" );
                        break;
                    }
                case RED:
                    {
                        printf( "%s%s|", RED_DISPLAY, WHITE_RESET );
                        break;
                    }
                case K_RED:
                    {
                        printf( "%s%s|", RED_KING_DISPLAY, WHITE_RESET
                              );
                        break;
                    }
                case WHITE:
                    {
                        printf( "%s|", WHITE_DISPLAY );
                        break;
                    }
                case K_WHITE:
                    {
                        printf( "%s|", WHITE_KING_DISPLAY );
                        break;
                    }
                default:
                    {
                        printf( "What the ... ?! \n" );
                    }
            }
        }
        printf( "\n" );
        printf( "------------------ \n" );
    }
}
