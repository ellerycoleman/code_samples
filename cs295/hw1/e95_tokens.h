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


#define MAX_TOKENS 1000



/* Operators */
#define ADDITION         258
#define SUBTRACTION      259
#define MULTIPLICATION   260
#define DIVISION         261
#define ASSIGNMENT	 262


char **token_type;
char *token_name;
void init_token_typemap(void);



#endif
