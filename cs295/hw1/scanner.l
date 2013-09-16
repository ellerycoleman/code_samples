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

%}









/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%
(do|for|return|break|short|else|goto|signed|unsigned) { return RESERVED_WORD; }
"+"     { return ADDITION; }
"-"     { return SUBTRACTION;  }
"*"     { return MULTIPLICATION;  }
"/"     { return DIVISION; }
"="     { return ASSIGNMENT; }
[0-9]+  { printf("NUMBER: %s\n",yytext); }
\n      { printf("NEWLINE\n"); }
[ \t]   { /* do nothing */ }
[A-Za-z_]{1}[A-Za-z0-9_]*   { return IDENTIFIER; }
.       { printf("Mystery Char: '%s'\n", yytext); }









%%
/*-----------------------------------------------------------
 |                    user functions                        
 +---------------------------------------------------------*/


