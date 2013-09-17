/*============================================================================
| File:		e95_tokens.h
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Token definitions for hw1 scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



#ifndef _E95_TOKENS_H
#define _E95_TOKENS_H  1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_TOKEN_DEFS 1000



/* Simple Operators   */
enum e95_tokens
{   ADDITION = 258,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    REMAINDER,	

    ASSIGNMENT,	
    ASSIGNMENT_ADD,
    ASSIGNMENT_SUBTRACT,
    ASSIGNMENT_MULTIPLY,
    ASSIGNMENT_DIVIDE,
    ASSIGNMENT_REMAINDER,

    LOGICAL_AND,
    LOGICAL_OR,
    LOGICAL_NOT,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    BITWISE_NOT,


    IDENTIFIER,
    RESERVED_WORD,
};



/* creating token structure */
typedef struct e95_token {
    char *name;
    char *type;
} e95_token;



/* defining a token map to hold all token definitions */
e95_token token_def_map[MAX_TOKEN_DEFS];



void init_token_definition_map(void);


#endif
