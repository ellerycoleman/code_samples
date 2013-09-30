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
int num_of_tokens_processed=0;


%}









/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%




"+"     { ++num_of_tokens_processed; return OP_ADDITION;         }
"-"     { ++num_of_tokens_processed; return OP_SUBTRACTION;      }
"*"     { ++num_of_tokens_processed; return OP_MULTIPLICATION;   }
"/"     { ++num_of_tokens_processed; return OP_DIVISION;         }
"|"     { ++num_of_tokens_processed; return OP_ABSVALUE;         }
"("     { ++num_of_tokens_processed; return SEPARATOR_LPAREN;    }
")"     { ++num_of_tokens_processed; return SEPARATOR_RPAREN;    }
";"     { ++num_of_tokens_processed; return SEPARATOR_SEMICOLON; }


-?[0-9]+ { ++num_of_tokens_processed;return INTEGER_CONSTANT;    }


\n     { ++num_of_tokens_processed; return END_OF_LINE;          }


[ \t]  { /* Ignore the following whitespace chars:
                  - space
		  - horizontal tab
		  - vertical tab
		  - form feed
              */
       }



.      { ++num_of_tokens_processed; return UNDEFINED; }









%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/



