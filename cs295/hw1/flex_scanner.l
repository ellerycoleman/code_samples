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


\/\*([^\*]|\*+[^\/\*])*\*\/+  {   /* NOTE: This is a regex to match C comments.
                                           The core of this regex was taken from page 40
                                           of the Flex & Bison book by John Levine.
                                           ISBN: 978-0-596-15597-1
                                   
                                           It was modified slightly in order to get flex
                                           to accept it.
                                  */



                                  /* Replace C comments with a single space. */
                                  printf(" ");
                              }



"do"        { return RESERVED_WORD_DO;       }
"for"       { return RESERVED_WORD_FOR;      }
"return"    { return RESERVED_WORD_RETURN;   }
"break"     { return RESERVED_WORD_BREAK;    }
"short"     { return RESERVED_WORD_SHORT;    }
"else"      { return RESERVED_WORD_ELSE;     }
"goto"      { return RESERVED_WORD_GOTO;     }
"signed"    { return RESERVED_WORD_SIGNED;   }
"unsigned"  { return RESERVED_WORD_UNSIGNED; }
"char"      { return RESERVED_WORD_CHAR;     }
"if"        { return RESERVED_WORD_IF;       }
"void"      { return RESERVED_WORD_VOID;     }
"int"       { return RESERVED_WORD_INT;      }
"continue"  { return RESERVED_WORD_CONTINUE; }
"long"      { return RESERVED_WORD_LONG;     }
"while"     { return RESERVED_WORD_WHILE;    }



"+"     { return OP_ADDITION;  }
"/"     { return OP_DIVISION;  }
"%"     { return OP_REMAINDER; }


"|"     { return OP_BITWISE_OR;      }
"^"     { return OP_BITWISE_XOR;     }
"~"     { return OP_ONES_COMPLIMENT; }
"<<"    { return OP_LEFT_BITSHIFT;   }
">>"    { return OP_RIGHT_BITSHIFT;  }


">="    { return OP_RELATIONAL_GTE;  }
"<"     { return OP_RELATIONAL_LT;   }
"<="    { return OP_RELATIONAL_LTE;  }


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


"&&"    { return OP_LOGICAL_AND;  }
"||"    { return OP_LOGICAL_OR;   }
"!"     { return OP_LOGICAL_NOT;  }


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




[0-9]+                             { return INTEGER_CONSTANT;    }



'\\n'   { cvalue= 10; return CHARACTER_CONSTANT; }
'\\t'   { cvalue= 9;  return CHARACTER_CONSTANT; }
'\\b'   { cvalue= 8;  return CHARACTER_CONSTANT; }
'\\r'   { cvalue= 13; return CHARACTER_CONSTANT; }
'\\f'   { cvalue= 12; return CHARACTER_CONSTANT; }
'\\v'   { cvalue= 11; return CHARACTER_CONSTANT; }
'\\\\'  { cvalue= 92; return CHARACTER_CONSTANT; }
'\\''   { cvalue= 39; return CHARACTER_CONSTANT; }
'\\\"'  { cvalue= 34; return CHARACTER_CONSTANT; }
'\\a'   { cvalue= 7;  return CHARACTER_CONSTANT; }
'\\\?'  { cvalue= 63; return CHARACTER_CONSTANT; }


'\\[0-7]{1,3}' { return CHARACTER_CONSTANT_OCTAL; }




\"[^"\n]*\"                 { return STRING_CONSTANT;     }



"'"                                { return APOSTROPHE;          }
"\""                               { return DOUBLEQUOTE;         }
"\\"                               { return BACKSLASH;           }
"`"                                { return BACKTICK;            }
"."                                { return PERIOD;              }
"&"                                { return AMPERSAND;           }
"$"                                { return DOLLAR_SIGN;         }
"@"                                { return AT_SIGN;             }
"#"                                { return NUMBER_SIGN;         }
"-"                                { return MINUS_SIGN;          }
">"                                { return GREATER_THAN_SYMBOL; }


\n   { return END_OF_LINE;   }


[ \t\v\f]   { /* Ignore the following whitespace chars:
                  - space
		  - horizontal tab
		  - vertical tab
		  - form feed
             */
	    }


[A-Za-z_]{1}[A-Za-z0-9_]*   { return IDENTIFIER; }


.   { return UNDEFINED; }









%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/


