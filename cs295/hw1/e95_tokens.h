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



/* Operators */
#define ADDITION         258
#define SUBTRACTION      259
#define MULTIPLICATION   260
#define DIVISION         261
#define ASSIGNMENT	 262




/* creating token structure */
typedef struct e95_token {
    char *name;
    char *type;
} e95_token;



/* defining a token map to hold all token definitions */
e95_token token_def_map[MAX_TOKEN_DEFS];



void init_token_definition_map(void);


#endif
