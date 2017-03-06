/***************************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat
 * Student Number   : 2109279U
 * Course Code      : COS1076 Advanced Programming Techniques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **************************************************************************/
#include "utility.h"

/* Tidy up residual data in stdin when encountering an error. You will 
 * need to use this when there is a possibility that there will be 
 * leftover characters in the input buffer. */
void read_rest_of_line(void)
{
    int ch;
    /* remove all characters from the buffer */
    while(ch = getc(stdin), ch!='\n' && ch!=EOF) 
        ;
    /* clear the error status of the input pointer */
    clearerr(stdin);
}

BOOLEAN tooManyChars( char someString[] )
{
    if( someString[strlen(someString)-1] == '\n' )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
