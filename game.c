/***************************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COSC1076 Advanced Programming Techniques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **************************************************************************/
#include "game.h"
#include "gameboard.h"
#include "scoreboard.h"

/* Requirement 7 - Controls the flow of play in the game*/
void play_game(char * player_one, char * player_two, 
        struct result* outcome)
{
    int i, j, redPlayer;
    char rematch[2];
    player player1, player2, currentPlayer;
    result thisGame;

    enum cell_contents gameboard[BOARDWIDTH]
        [BOARDHEIGHT];

    /*  copy the names from the memory addresses into the name slot of the
     *  correct player
     */
    for( i=0; i<strlen( player_one ); ++i )
    {
        player1.name[i] = player_one[i];
    }
    player1.name[i] = '\0';
    for( i=0; i<strlen( player_two ); ++i )
    {
        player2.name[i] = player_two[i];
    }
    player2.name[i] = '\0';
    /**********************************************************************/

    while( TRUE )
    {
        srand( time() );
        redPlayer = rand() % 2;
        if( redPlayer == 0 )
        {
            player1.col = P_RED;
            currentPlayer = player1;
            player2.col = P_WHITE;
        }
        else
        {
            player1.col = P_WHITE;
            player2.col = P_RED;
            currentPlayer = player2;
        }

        init_gameboard( gameboard );

        while( test_for_winner( &currentPlayer, gameboard ) )
        {
            display_gameboard( gameboard );

            player_turn( &currentPlayer, gameboard );
            if( currentPlayer.col == QUIT_GAME )
            {
                return;
            }

            if( currentPlayer.col == player1.col )
            {
                currentPlayer = player2;
            }
            else
            {
                currentPlayer = player1;
            }
        }

        /*  Now the current player has no moves, so they have lost   */
        for( i=0; i<strlen( currentPlayer.name ); ++i )
        {
            strcpy( thisGame.lost, currentPlayer.name );
        }
        if( currentPlayer.col == player1.col )
        {
            currentPlayer = player2;
        }
        else
        {
            currentPlayer = player1;
        }
        for( i=0; i<strlen( currentPlayer.name ); ++i )
        {
            strcpy( thisGame.won, currentPlayer.name );
        }
        add_to_scoreboard( outcome, &thisGame );

        rematch[0] = 'r';
        /*  random character just to get the loop started   */
        while( rematch[0] != 'y' && rematch[0] != 'n' )
        {
            printf( "Do either of you warlords care for a rematch? \n"
                    "Press \"y\" for yes \"n\" for no, \n"
                    "but please don't kill me, I beg of you.  " );
            fgets( rematch, 2, stdin );
            read_rest_of_line();
            switch( *rematch )
            {
                case 110:
                    {
                        return;
                    }
                default:
                    {
                        printf( "OK, now I know you're going to kill me, \n"
                                "but enter a valid option, you moron \n" );
                    }
            }
        }
    }
}

/* Requirement 5 - Handles the logic for the player's turn*/
void player_turn(struct player * current, 
        enum cell_contents board[][BOARDWIDTH])
{
    BOOLEAN invalidMove, attackNecessary;
    char moveEntered[MOVE_LEN];
    int moveFromRow, moveFromCol, moveToRow, moveToCol;
    location starting, finishing;
    move thisMove, proposedAttack;
    movetype thisMoveIs;

    do
    {
        attackNecessary = must_attack( current, board );
        print_name( current );  printf( ", " );        
        if( attackNecessary )
        {
            printf( "you must attack.  Please " );
        }
        else
        {
            printf( "please " );
        }
        printf( "enter a move (d,d-d,d where d are digits), \nor "
                "press Enter or Control-D to return to menu:  ");
        do
        {
            invalidMove = FALSE;

            if( fgets( moveEntered, MOVE_LEN+1, stdin ) == NULL ||
                    moveEntered[0] == '\n' )
            {
                current->col = QUIT_GAME;
                return;
            }
            if( tooManyChars( moveEntered ) )
            {
                printf( "Too many characters.  Try again:  " );
                read_rest_of_line();
                invalidMove = TRUE;
            }
            else
            {
                if( sscanf( moveEntered, "%d,%d-%d,%d", &moveFromCol,
                            &moveFromRow, &moveToCol, &moveToRow) != 4 )
                {
                    printf( "Invalid format.  Try again:  " );
                    invalidMove = TRUE;
                }
                if( moveFromCol<0 || moveFromCol>=BOARDWIDTH ||
                        moveFromRow<0 || moveFromRow>=BOARDHEIGHT ||
                        moveToCol<0 || moveToCol>=BOARDWIDTH || moveToRow<0
                        || moveToRow>=BOARDHEIGHT )
                {
                    printf( "Invalid row or column number.  Try again:  " );
                    invalidMove = TRUE;
                }
            }
        } while( invalidMove );
        /*  By this point, we have rows and columns to be moved from and to
         *  in the correct format.  Whether it is a valid move or not
         *  remains to be checked.
         */

        /*  note that x represents a row and y represents a column   */
        thisMove.start.y = moveFromCol;
        thisMove.start.x = moveFromRow;
        thisMove.end.y = moveToCol;
        thisMove.end.x = moveToRow;

        thisMoveIs = is_valid_move( thisMove, current, board );

        if( thisMoveIs == INVALID )
        {
            printf( "You entered an invalid move.  Try again. \n " );
            invalidMove = TRUE;
        }
        if( attackNecessary && thisMoveIs != ATTACK )
        {
            printf( "Might I remind you that you must attack?!  Try "
                    "again. \n ");
            invalidMove = TRUE;
        }

    } while( invalidMove );

    /*  Now they have made a valid move   */
    board[thisMove.end.x][thisMove.end.y] = board[thisMove.start.x]
        [thisMove.start.y];
    board[thisMove.start.x][thisMove.start.y] = EMPTY;
    if( thisMoveIs == ATTACK )
    {
        board[ (thisMove.start.x + thisMove.end.x)/2 ]
            [ (thisMove.start.y + thisMove.end.y)/2 ] = EMPTY;
    }
    if( board[thisMove.end.x][thisMove.end.y] == RED && 
            thisMove.end.x == BOARDHEIGHT - 1 )
        /*  a red man, not king, has reached the end   */
    {
        board[thisMove.end.x][thisMove.end.y] = K_RED;
        thisMoveIs = NORMAL;
    }
    if( board[thisMove.end.x][thisMove.end.y] == WHITE && thisMove.end.x == 
            0 )
        /*  a white man, not king, has reached the end   */
    {
        board[thisMove.end.x][thisMove.end.y] = K_WHITE;
        thisMoveIs = NORMAL;
    }
    if( thisMoveIs == ATTACK )
    {
        sscanf( "y\n\0", "%c\n\0", moveEntered );
        while( moveEntered[0] == 'y' )
        {
            display_gameboard( board );

            print_name( current );
            printf( ", you ruthless warlord!  \nWould you like to kill"
                    " more (y for yes, n for no, \ncontrol-D or enter to "
                    " quit the game)?  ", current->name );
            if( fgets( moveEntered, MOVE_LEN+1, stdin ) == NULL ||
                    moveEntered[0] == '\n' )
            {
                current->col = QUIT_GAME;
                return;
            }
            if( moveEntered[0] != 'y' && moveEntered[0] != 'n' )
            {
                printf( "You stupid warlord.  Please type y or n, or press"
                        "Enter or Control-D. \n" );
                sscanf( "y\n\0", "%c\n\0", moveEntered );
                /*  if an incorrect input was received, we want to make
                 *  sure the loop runs again
                 */
            }
            if( moveEntered[0] == 'y' )
            {
                invalidMove = FALSE;
                /*  note that in this block invalidMove refers to any type
                 *  of invalid ATTACK move
                 */

                printf( "Please use the same piece to attack. \n"
                        "Enter an attack move (d,d-d,d), \n" 
                        "or press Enter or Control-D to return to menu:  "
                        );
                if( fgets( moveEntered, MOVE_LEN+1, stdin ) == NULL ||
                        moveEntered[0] == '\n' )
                {
                    current->col = QUIT_GAME;
                    return;
                }
                if( tooManyChars( moveEntered ) )
                {
                    printf( "Too many characters entered.  Try again."
                            "\n" );
                    sscanf( "y\n\0", "%c\n\0", moveEntered );
                    invalidMove = TRUE;
                }
                else
                {
                    if( sscanf( moveEntered, "%d,%d-%d,%d", &moveFromCol,
                                &moveFromRow, &moveToCol, &moveToRow) != 4 )
                    {
                        printf( "Invalid format.  Try again:  " );
                        sscanf( "y\n\0", "%c\n\0", moveEntered );
                        invalidMove = TRUE;
                    }
                    if( moveFromCol<0 || moveFromCol>=BOARDWIDTH ||
                            moveFromRow<0 || moveFromRow>=BOARDHEIGHT ||
                            moveToCol<0 || moveToCol>=BOARDWIDTH ||
                            moveToRow<0 || moveToRow>=BOARDHEIGHT )
                    {
                        printf( "Invalid row or column number.  Try again:"
                              );
                        sscanf( "y\n\0", "%c\n\0", moveEntered );
                        invalidMove = TRUE;
                    }
                }

                if( !invalidMove )
                {
                    if( moveFromRow != thisMove.end.x || 
                            moveFromCol != thisMove.end.y )
                    {
                        printf( "thisMove.end.x = %d \n"
                                "thisMove.end.y = %d \n \n",
                                thisMove.end.x, thisMove.end.y );

                        printf( "You must attack using the same piece!!"
                                "  Try again. \n" );
                        sscanf( "y\n\0", "%c\n\0", moveEntered );
                        invalidMove = TRUE;
                    }
                    else
                    {
                        proposedAttack.start.x = moveFromRow;
                        proposedAttack.start.y = moveFromCol;
                        proposedAttack.end.x = moveToRow;
                        proposedAttack.end.y = moveToCol;
                        thisMoveIs = is_valid_move( proposedAttack,
                                current, board );
                        if( thisMoveIs != ATTACK )
                        {
                            printf( "You entered an invalid or "
                                    "non-attack move.  Try again. \n" );
                            sscanf( "y\n\0", "%c\n\0", moveEntered );
                            invalidMove = TRUE;
                        }
                    }
                }

                if( !invalidMove )
                    /*  so fiiiiiiiiiiiiiiinally we know that the user has
                     *  entered an attack move using the same piece!
                     */
                {
                    thisMove.start.x = moveFromRow;
                    thisMove.start.y = moveFromCol;
                    thisMove.end.x = moveToRow;
                    thisMove.end.y = moveToCol;

                    board[thisMove.end.x][thisMove.end.y] = board
                        [thisMove.start.x][thisMove.start.y];
                    board[ (thisMove.start.x + thisMove.end.x)/2 ]
                        [ (thisMove.start.y + thisMove.end.y)/2 ] = EMPTY;
                    board[thisMove.start.x][thisMove.start.y] = EMPTY;

                    sscanf( "y\n\0", "%c\n\0", moveEntered );
                    /*  by the end of the loop, the player will have entered
                     *  incorrect input, or attacked.  Either way, we want
                     *  the loop to run again.  Or, a man, not king, has
                     *  reached the end, and their turn is finished
                     */

                    if( board[thisMove.end.x][thisMove.end.y] == RED && 
                            thisMove.end.x == BOARDHEIGHT - 1 )
                        /*  if a red man, not king, has made it to the end
                        */
                    {
                        board[thisMove.end.x][thisMove.end.y] = K_RED;
                        sscanf( "n\n\0", "%c\n\0", moveEntered );
                    }
                    if( board[thisMove.end.x][thisMove.end.y] == WHITE &&
                            thisMove.end.x == 0 )
                        /*  if a white man, not king, has reached the end
                        */
                    {
                        board[thisMove.end.x][thisMove.end.y] = K_WHITE;
                        sscanf( "n\n\0", "%c\n\0", moveEntered );
                    }
                }
            } /*  end of if (y pressed)   */
        }
    }
}

BOOLEAN must_attack( player * current, cell_contents board[][BOARDWIDTH] )
{
    int i, j, k, m;
    move possAttack;

    for( i=0; i < BOARDHEIGHT; ++i )
    {
        for( j=0; j < BOARDWIDTH; ++j )
        {
            if( current->col == P_RED &&
                    ( board[i][j] == RED || board[i][j] == K_RED ) )
                /*  current player is red and on the current square   */
            {
                for( k=-2; k<=2; k=k+4 )
                {
                    for( m=-2; m<=2; m=m+4 )
                    {
                        possAttack.start.x = i;
                        possAttack.start.y = j;
                        possAttack.end.x = i+k;
                        possAttack.end.y = j+m;

                        if( is_valid_move( possAttack, current, board )
                                == ATTACK )
                        {
                            return TRUE;
                        }
                    }
                }
            }
            if( current->col == P_WHITE &&
                    ( board[i][j] == WHITE || board[i][j] == K_WHITE ) )
                /*  current player is white and on the current square   */
            {
                for( k=-2; k<=2; k=k+4 )
                {
                    for( m=-2; m<=2; m=m+4 )
                    {
                        possAttack.start.x = i;
                        possAttack.start.y = j;
                        possAttack.end.x = i+k;
                        possAttack.end.y = j+m;

                        if( is_valid_move( possAttack, current, board )
                                == ATTACK )
                        {
                            return TRUE;
                        }
                        /*  From each square on the board that has a
                         *  current player's piece on it, try each of the
                         *  4 attack moves to see if they are valid or not
                         */
                    }
                }
            }
        }
    }
    return FALSE;
}

/* Requirement 4 - Tests to see whether a move is valid or not*/
enum move_type is_valid_move(struct move next_move, 
        struct player * current, enum cell_contents board[][BOARDWIDTH])
{
    if( !( next_move.end.x >= 0 && next_move.end.x < BOARDHEIGHT &&
                next_move.end.y >= 0 && next_move.end.y < BOARDWIDTH ) )
    {
        return INVALID;
    }

    if( current->col == P_RED )
        /*  current player is red   */
    {
        if( board[next_move.start.x][next_move.start.y] == RED ||
                board[next_move.start.x][next_move.start.y] == K_RED )
            /*  the starting square contains a red man or king   */
        {
            if( next_move.end.x - next_move.start.x == 1 )
                /*  the piece is moving forward one row   */
            {
                if( next_move.end.y - next_move.start.y == 1 && 
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  moving right one column   */
                {
                    return NORMAL;
                }
                if( next_move.end.y - next_move.start.y == -1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  moving left one column   */
                {
                    return NORMAL;
                }
            }
            if( next_move.end.x - next_move.start.x == 2 )
                /*  the piece is moving forward two rows, attacking   */
            {
                if( next_move.end.y - next_move.start.y == 2 &&
                        ( board[next_move.start.x + 1][next_move.start.y +
                          1] == WHITE || board[next_move.start.x + 1][
                          next_move.start.y + 1] == K_WHITE ) && board
                        [next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the piece is moving two columns right and there IS
                     *  an opposing piece in between
                     */
                {
                    return ATTACK;
                }
                if( next_move.end.y - next_move.start.y == -2 &&
                        ( board[next_move.start.x + 1][next_move.start.y -
                          1] == WHITE || board[next_move.start.x + 1]
                          [next_move.start.y - 1] == K_WHITE ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the piece is moving two columns left and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
            }
        }
        if( board[next_move.start.x][next_move.start.y] == K_RED )
            /*  it's a red king, which can move backwards   */
        {
            if( next_move.end.x - next_move.start.x == -1 )
                /*  the king is moving one row backward   */
            {
                if( next_move.end.y - next_move.start.y == 1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving one column right   */
                {
                    return NORMAL;
                }
                if( next_move.end.y - next_move.start.y == -1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving one column left   */
                {
                    return NORMAL;
                }
            }
            if( next_move.end.x - next_move.start.x == -2 )
                /*  the king is moving back two rows, attacking   */
            {
                if( next_move.end.y - next_move.start.y == 2 &&
                        ( board[next_move.start.x-1][next_move.start.y+1] ==
                          WHITE || board[next_move.start.x-1]
                          [next_move.start.x+1] == K_WHITE ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving right two columns and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
                if( next_move.end.y - next_move.start.y == -2 &&
                        ( board[next_move.start.x-1][next_move.start.y-1] ==
                          WHITE || board[next_move.start.x-1]
                          [next_move.start.y-1] == K_WHITE ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving two columns left and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
            }
        }

    }
    else
        /*  current player is white   */
    {
        if( board[next_move.start.x][next_move.start.y] == WHITE ||
                board[next_move.start.x][next_move.start.y] == K_WHITE )
            /*  the starting square contains a white man or king   */
        {
            if( next_move.end.x - next_move.start.x == -1 )
                /*  the piece is moving forward one row   */
            {
                if( next_move.end.y - next_move.start.y == 1 && 
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  moving right one column   */
                {
                    return NORMAL;
                }
                if( next_move.end.y - next_move.start.y == -1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  moving left one column   */
                {
                    return NORMAL;
                }
            }
            if( next_move.end.x - next_move.start.x == -2 )
                /*  the piece is moving forward two rows, attacking   */
            {
                if( next_move.end.y - next_move.start.y == 2 &&
                        ( board[next_move.start.x - 1][next_move.start.y +
                          1] == RED || board[next_move.start.x - 1][
                          next_move.start.y + 1] == K_RED ) && board
                        [next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the piece is moving two columns right and there IS
                     *  an opposing piece in between
                     */
                {
                    return ATTACK;
                }
                if( next_move.end.y - next_move.start.y == -2 &&
                        ( board[next_move.start.x - 1][next_move.start.y -
                          1] == RED || board[next_move.start.x - 1]
                          [next_move.start.y - 1] == K_RED ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the piece is moving two columns left and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
            }
        }
        if( board[next_move.start.x][next_move.start.y] == K_WHITE )
            /*  it's a white king, which can move backwards   */
        {
            if( next_move.end.x - next_move.start.x == 1 )
                /*  the king is moving one row backward   */
            {
                if( next_move.end.y - next_move.start.y == 1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving one column right   */
                {
                    return NORMAL;
                }
                if( next_move.end.y - next_move.start.y == -1 &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving one column left   */
                {
                    return NORMAL;
                }
            }
            if( next_move.end.x - next_move.start.x == 2 )
                /*  the king is moving back two rows, attacking   */
            {
                if( next_move.end.y - next_move.start.y == 2 &&
                        ( board[next_move.start.x+1][next_move.start.y+1] ==
                          RED || board[next_move.start.x+1]
                          [next_move.start.x+1] == K_RED ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving right two columns and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
                if( next_move.end.y - next_move.start.y == -2 &&
                        ( board[next_move.start.x+1][next_move.start.y-1] ==
                          RED || board[next_move.start.x+1]
                          [next_move.start.y-1] == K_RED ) &&
                        board[next_move.end.x][next_move.end.y] == EMPTY )
                    /*  the king is moving two columns left and there IS an
                     *  opposing piece in between
                     */
                {
                    return ATTACK;
                }
            }
        }
    }
    return INVALID;
}

/* Requirement  - Tests whether the next player has any valid moves */
BOOLEAN test_for_winner(struct player * next_player, 
        enum cell_contents board[][BOARDWIDTH])
{
    int i, j, k, m;
    move possMove;

    for( i=0; i<BOARDHEIGHT; ++i )
    {
        for( j=0; j<BOARDWIDTH; ++j )
        {
            if( next_player->col == P_RED &&
                    ( board[i][j] == RED || board[i][j] == K_RED ) )
                /*  current player is red and on current square   */
            {
                for( k=-2; k<=2; ++k )
                {
                    for( m=-2; m<=2; ++m )
                    {
                        if( k != 0 && ( k == m || k == -m ) )
                        {
                            possMove.start.x = i;
                            possMove.start.y = j;
                            possMove.end.x = i+k;
                            possMove.end.y = j+m;

                            if( is_valid_move( possMove, next_player,
                                        board ) != INVALID )
                            {
                                return TRUE;
                            }
                        }
                    }
                }
            }
            if( next_player->col == P_WHITE &&
                    ( board[i][j] == WHITE || board[i][j] == K_WHITE ) )
                /*  current player is white and on current square   */
            {
                for( k=-2; k<=2; ++k )
                {
                    for( m=-2; m<=2; ++m )
                    {
                        if( k != 0 && ( k == m || k == -m ) )
                        {
                            possMove.start.x = i;
                            possMove.start.y = j;
                            possMove.end.x = i+k;
                            possMove.end.y = j+m;

                            if( is_valid_move( possMove, next_player,
                                        board ) != INVALID )
                            {
                                return TRUE;
                            }
                        }
                    }
                }
                /*  for each square on the board that has a current player's
                 *  piece on it, test all 8 moves (attack and normal) to
                 *  see if they are valid.
                 */
            }
        }
    }
    return FALSE;
}

void print_name( player * current )
{
    if( current->col == P_RED )
    {
        printf( "\x1b[31m" );
    }
    printf( "%s%s", current->name, WHITE_RESET );
}
