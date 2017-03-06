/***************************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2014 Assignment #1 
 * Full Name        : Andrew Mouat 
 * Student Number   : 2109279U
 * Course Code      : COSC1076 Advanced Programming Techinques
 * Program Code     : BP096 B. Software Engineering
 * Start up code provided by Paul Miller and Virginia King
 **************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME_LEN 20

typedef enum
{
    FALSE, TRUE
} BOOLEAN;

/*Clears residual data from stdin*/
void read_rest_of_line(void);

BOOLEAN tooManyChars( char someString[] );

#endif
