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
{   OP_ADDITION = 258,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_REMAINDER,	


    OP_BITWISE_AND,
    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_ONES_COMPLIMENT,


    OP_ASSIGNMENT,	
    OP_ASSIGNMENT_ADD,
    OP_ASSIGNMENT_SUBTRACT,
    OP_ASSIGNMENT_MULTIPLY,
    OP_ASSIGNMENT_DIVIDE,
    OP_ASSIGNMENT_REMAINDER,

    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_LOGICAL_NOT,
    OP_LOGICAL_NOT_EQUAL,


    IDENTIFIER,
    RESERVED_WORD,
    INT_CONSTANT,


    SEP_SEMICOLON,
    END_OF_LINE,
    UNDEFINED
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
