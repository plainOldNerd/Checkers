/***************************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COSC1076 Advanced Programming Techniques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **************************************************************************/
#include <stdio.h>
#include "draughts.h"
#include "game.h"
#include "utility.h"
#include "scoreboard.h"

int main (int argc, char* argv[])
{
    char option[2];
    char player1[MAX_NAME_LEN+1];
    char player2[MAX_NAME_LEN+1];
    result outcome[SCOREBOARDSIZE];

    init_scoreboard( outcome );

    while( TRUE )
    {
        printf( "English Draughts - Main Menu \n" );
        printf( "1)  Play Game \n" );
        printf( "2)  Display Winners \n" );
        printf( "3)  Reset Scoreboard \n" );
        printf( "4)  Quit \n" );
        printf( "Please enter a digit 1 - 4:  " );
        
        /*  Note that at this point we will DEFINITELY ONLY need a "1",
         *  "2", "3", or "4", so 2 spaces is enough, then we can
         *  read_rest_of_line();  */

        fgets( option, 2, stdin );
        read_rest_of_line();

        switch( *option )
        {
            case 49:
            {
                do
                {
                    printf( "Player 1, enter your name:  " );
                    fgets( player1, MAX_NAME_LEN+1, stdin);
                    if( tooManyChars( player1 ) )
                    {
                        printf( "Too many characters!!  Please enter a"
                                " name with less than %d characters.  \n",
                                MAX_NAME_LEN );
                        read_rest_of_line();
                    }
                    if( player1[0] == '\n' )
                    {
                        printf( "You did not enter anything.  Your name "
                                "is now \"1\"\n" );
                        player1[0] = '1';
                        player1[1] = '\n';
                        player1[2] = '\0';
                    }
                } while( tooManyChars( player1 ) );
                player1[ strlen( player1 ) - 1 ] = '\0';
                /*  Here, we are forcing the players to enter a name of
                 *  an acceptable name length, and then taking away the new
                 *  line character for printing reasons.
                 */
                do
                {
                    printf( "Player 2, enter your name:  " );
                    fgets( player2, MAX_NAME_LEN+1, stdin);
                    if( tooManyChars( player2 ) )
                    {
                        printf( "Too many characters!!  Please enter a"
                                " name with less than %d characters. \n", 
                                MAX_NAME_LEN );
                        read_rest_of_line();
                    }
                    if( player2[0] == '\n' )
                    {
                        printf( "You did not enter anything.  Your name "
                                "is now \"2\"\n" );
                        player2[0] = '2';
                        player2[1] = '\n';
                        player2[2] = '\0';
                    }
                } while( tooManyChars( player2 ) );
                player2[ strlen( player2 ) - 1 ] = '\0';

                play_game( player1, player2, outcome );

                printf( "\n \n" );
                break;
            }
            case 50:
            {
                display_scoreboard( outcome );
                break;
            }
            case 51:
            {
                init_scoreboard( outcome );
                break;
            }
            case 52:
            {
                return EXIT_SUCCESS;
            }
            default:
            {
                printf( "Choose a valid option ya tosser!! \n \n" );
            }
        }
    }
    return EXIT_SUCCESS;
}
