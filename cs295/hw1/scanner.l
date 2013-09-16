/*============================================================================
| File:		scanner.l
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A flex program to generate a lexical analyzer for a subset
|               of the C89 programming language.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%option yylineno

%{

#include "e95_tokens.h"
extern char *token_name;
void set_token_name(char *name);
%}









/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%
"+"     { return ADDITION; set_token_name("Addition");  }
"-"     { return SUBTRACTION;  }
"*"     { return MULTIPLICATION;  }
"/"     { return DIVISION; }
"="     { return ASSIGNMENT; }
[0-9]+  { printf("NUMBER: %s\n",yytext); }
\n      { printf("NEWLINE\n"); }
[ \t]   { /* do nothing */ }
.       { printf("Mystery Char: '%s'\n", yytext); }









%%
/*-----------------------------------------------------------
 |                    user functions                        
 +---------------------------------------------------------*/
void set_token_name(char *name)
{   token_name= name;
}



