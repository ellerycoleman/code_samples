/*============================================================================
| File:		flex_scanner.l
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


\/\*([^\*]|\*+[^\/\*])*\*\/+  { /* 
                                   NOTE: The core of this regex was taken from page 40
				         of the Flex & Bison book by John Levine.
					 It was modified slightly in order to get flex
					 to accept it.
			                 ISBN: 978-0-596-15597-1
                                */ 



                                  /* Replace C comments with a single space. */
                                  printf(" ");
                              }



(do|for|return|break|short|else|goto|signed|unsigned|char|if|void|int|continue|long|while) { return RESERVED_WORD; }



"+"     { return OP_ADDITION;       }
"-"     { return OP_SUBTRACTION;    }
"*"     { return OP_MULTIPLICATION; }
"/"     { return OP_DIVISION;       }
"%"     { return OP_REMAINDER;      }


"&"     { return OP_BITWISE_AND;     }
"|"     { return OP_BITWISE_OR;      }
"^"     { return OP_BITWISE_XOR;     }
"~"     { return OP_ONES_COMPLIMENT; }


"+="    { return OP_ASSIGNMENT_ADD;      }
"-="    { return OP_ASSIGNMENT_SUBTRACT; }
"*="    { return OP_ASSIGNMENT_MULTIPLY; }
"/="    { return OP_ASSIGNMENT_DIVIDE;   }

"&&"    { return OP_LOGICAL_AND;       }
"||"    { return OP_LOGICAL_OR;        }
"!"     { return OP_LOGICAL_NOT;       }
"!="    { return OP_LOGICAL_NOT_EQUAL; }


"="     { return OP_ASSIGNMENT; }



";"     { return SEP_SEMICOLON; }



[0-9]+  { return INT_CONSTANT;  }



\n      { return END_OF_LINE;   }


[ \t\v\f]   { /* Do nothing for the following whitespace chars:
                  - space
		  - horizontal tab
		  - vertical tab
		  - form feed
             */
	    }


[A-Za-z_]{1}[A-Za-z0-9_]*   { return IDENTIFIER; }


.       { return UNDEFINED; }









%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/


