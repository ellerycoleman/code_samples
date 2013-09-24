/*============================================================================
| File:		calc_tokens.h
| Date:		Tue Sep 24 15:07:52 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Token definitions for calculator.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



#ifndef _CALC_TOKENS_H
#define _CALC_TOKENS_H  1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_TOKEN_DEFS 500



/* Simple Operators   */
/*
enum calc_tokens
{   

    OP_ADDITION = 258,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_ABSVALUE,

    INTEGER_CONSTANT,

    END_OF_LINE,

    UNDEFINED
};
*/
#include "bison_parser.tab.h"


/* creating token structure */
typedef struct calc_token 
{   char *name;
    char *type;
} calc_token;



/* defining a token map to hold all token definitions */
calc_token token_def_map[MAX_TOKEN_DEFS];



/* function prototypes */
void init_token_definition_map(void);



#endif
