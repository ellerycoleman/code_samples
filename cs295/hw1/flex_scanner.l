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
			                 ISBN: 978-0-596-15597-1

					 It was modified slightly in order to get flex
					 to accept it.
                                */



                                  /* Replace C comments with a single space. */
                                  printf(" ");
                              }



(do|for|return|break|short|else|goto|signed|unsigned|char|if|void|int|continue|long|while) { return RESERVED_WORD; }



"+"     { return OP_ADDITION;       }
"-"     { return OP_SUBTRACTION;    }
"/"     { return OP_DIVISION;       }
"%"     { return OP_REMAINDER;      }


"&"     { return OP_BITWISE_AND;     }
"|"     { return OP_BITWISE_OR;      }
"^"     { return OP_BITWISE_XOR;     }
"~"     { return OP_ONES_COMPLIMENT; }
"<<"    { return OP_LEFT_BITSHIFT;   }
">>"    { return OP_RIGHT_BITSHIFT;  }


">"     { return OP_RELATIONAL_GT;  }
">="    { return OP_RELATIONAL_GTE; }
"<"     { return OP_RELATIONAL_LT;  }
"<="    { return OP_RELATIONAL_LTE; }


"+="    { return OP_ASSIGNMENT_ADD;            }
"-="    { return OP_ASSIGNMENT_SUBTRACT;       }
"*="    { return OP_ASSIGNMENT_MULTIPLY;       }
"/="    { return OP_ASSIGNMENT_DIVIDE;         }
"%="    { return OP_ASSIGNMENT_REMAINDER;      }
"<<="   { return OP_ASSIGNMENT_LEFT_BITSHIFT;  }
">>="   { return OP_ASSIGNMENT_RIGHT_BITSHIFT; }
"&="    { return OP_ASSIGNMENT_BITWISE_AND;    }
"|="    { return OP_ASSIGNMENT_BITWISE_OR;     }
"^="    { return OP_ASSIGNMENT_BITWISE_XOR;    }


"&&"    { return OP_LOGICAL_AND;       }
"||"    { return OP_LOGICAL_OR;        }
"!"     { return OP_LOGICAL_NOT;       }


"=="    { return OP_EQUALITY;     }
"!="    { return OP_NON_EQUALITY; }


"++"    { return OP_INCREMENT;  }
"--"    { return OP_DECREMENT;  }


"="     { return OP_ASSIGNMENT;    }
"*"     { return OP_ASTERISK;      }
"?"     { return OP_QUESTION_MARK; }


"("     { return SEP_LEFT_PAREN;           }
")"     { return SEP_RIGHT_PAREN;          }
"{"     { return SEP_LEFT_CURLY_BRACE;     }
"}"     { return SEP_RIGHT_CURLY_BRACE;    }
"["     { return SEP_LEFT_SQUARE_BRACKET;  }
"]"     { return SEP_RIGHT_SQUARE_BRACKET; }
";"     { return SEP_SEMICOLON;            }
":"     { return SEP_COLON;                }
","     { return SEP_COMMA;                }



[0-9]+  { return INT_CONSTANT;  }



\n      { return END_OF_LINE;   }


[ \t\v\f]   { /* Ignore the following whitespace chars:
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


