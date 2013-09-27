/*============================================================================
| File:		flex_scanner.l
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A flex program to generate a lexical analyzer for a simple
|               calculator.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%option yylineno

%{

#include "calc_tokens.h"

%}









/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%




"+"     { return OP_ADDITION;         }
"-"     { return OP_SUBTRACTION;      }
"*"     { return OP_MULTIPLICATION;   }
"/"     { return OP_DIVISION;         }
"|"     { return OP_ABSVALUE;         }
"("     { return SEPARATOR_LPAREN;    }
")"     { return SEPARATOR_RPAREN;    }
";"     { return SEPARATOR_SEMICOLON; }


[0-9]+ { yylval= atoi(yytext); return INTEGER_CONSTANT;   }


\n     { return END_OF_LINE;        }


[ \t]  { /* Ignore the following whitespace chars:
                  - space
		  - horizontal tab
		  - vertical tab
		  - form feed
              */
       }



.      { return UNDEFINED; }









%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/



