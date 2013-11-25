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


#include "bison_parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_TOKEN_DEFS 500




/* creating token structure */
typedef struct e95_token 
{   char *name;
    char *type;
} e95_token;



/* defining a token map to hold all token definitions */
e95_token token_def_map[MAX_TOKEN_DEFS];


/* for storing values of escape sequences */
int cvalue;


/* for reading integer constants of unknown size */
long long ivalue_tmp;
char *itype;
union e95_int_union
{   
   /* int and long are the same size for our purposes.
    | So we use the 'int' keyword for both ival and lval,
    | and we use unsigned int for ulval.
    +-----------------------------------------------------*/
    int ival;
    int lval;            
    unsigned int ulval;  
                          
} e95_int_constant;


/* in support of escape sequence parsing in char and string constants */
char e95_strbuf[16000];
char e95_strbuf2[16000];
int  e95_strbuf_idx;
int indent;


/* function prototypes */
void init_token_definition_map(void);
char convert_escape_char(char *);
int get_octal_value(char *);



#endif
