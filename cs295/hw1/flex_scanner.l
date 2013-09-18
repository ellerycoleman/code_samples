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


(do|for|return|break|short|else|goto|signed|unsigned|char|if|void|int|continue|long|while) { return RESERVED_WORD; }


"+"     { return OP_ADDITION; }
"-"     { return OP_SUBTRACTION;  }
"*"     { return OP_MULTIPLICATION;  }
"/"     { return OP_DIVISION; }
"%"     { return OP_REMAINDER; }


"+="    { return OP_ASSIGNMENT_ADD; }
"-="    { return OP_ASSIGNMENT_SUBTRACT; }
"*="    { return OP_ASSIGNMENT_MULTIPLY; }
"/="    { return OP_ASSIGNMENT_DIVIDE; }


"="     { return OP_ASSIGNMENT; }



";"     { return SEP_SEMICOLON; }




[0-9]+  {   printf("line: %-5d", yylineno); 
            printf("NUMBER: %s\n",yytext); 
        }

\n      {   printf("line: %-5d", yylineno); 
            printf("NEWLINE\n"); 
        }

[ \t]   { /* do nothing */ }


[A-Za-z_]{1}[A-Za-z0-9_]*   { return IDENTIFIER; }


.       {   printf("line: %-5d", yylineno); 
            printf("Unknown: '%s'\n", yytext); 
	}









%%
/*-----------------------------------------------------------
 |                    user functions                        
 +---------------------------------------------------------*/


