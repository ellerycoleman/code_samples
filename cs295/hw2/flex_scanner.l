/*============================================================================
| File:		flex_scanner.l
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A flex program to generate a lexical analyzer for a subset
|               of the C89 programming language.
+-----------------------------------------------------------------------------
| Revision: $Id: flex_scanner.l 157 2013-09-26 03:04:23Z ellery $
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%option yylineno
%x e95_string

%{

#include "e95_tokens.h"
#include "bison_parser.tab.h"

extern YYSTYPE yylval;
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




[0-9]+                  { return INTEGER_CONSTANT;  }


'\\[ntbrfv\\\'\"a?]' { /* Handle non-octal escape char constants */
                      char tmp[10];
                      strncpy(tmp,yytext,10);
                      e95_strbuf[0]= '\0';
                      yytext[strlen(yytext)-1]= '\0';            /* remove trailing apostrophe */
                      strncpy(e95_strbuf,yytext+1,3);            /* remove leading apostrophe  */
                      cvalue= convert_escape_char(e95_strbuf);   /* get decimal value of escape sequence */
                      strncpy(yytext,tmp,10);                    /* restore yytext */
                      return CHARACTER_CONSTANT;
                    }



'\\[0-7]{1,3}' {  /* Handle octal character constants */
                  cvalue= get_octal_value(yytext);
                  return CHARACTER_CONSTANT_OCTAL; 
               }

'[0-9A-Za-z]'  {  cvalue= yytext[1]; return CHARACTER_CONSTANT; }


"\""                    {   /* Beginning of string constant has been detected  */
                            BEGIN(e95_string); 
			    e95_strbuf[0]='\0';
                            e95_strbuf_idx=0;
                        }

<e95_string>[^"\n\\]*   {   /* All string chars except double quotes,
                               newlines, and escape chars will be copied
			       to our string buffer: e95_strbfr[].
                            */
                            strncat(e95_strbuf, yytext,strlen(yytext));
			    e95_strbuf_idx+= strlen(yytext);
			}

<e95_string>\\([ntbrfv\\'"a?]|[0-9]{1,3})  { /* Detected an escape sequence */
				                char converted_escape_code= (char) convert_escape_char(yytext);
				                strncat(e95_strbuf,&converted_escape_code,1); 
			                   }


<e95_string>\\\n        {  /* allow escaped returns, but don't copy them
                              to the string buffer.
                           */ 
		        }


<e95_string>\n          {   /* unescaped returns are errors - try to fail gracefully */ 
                               strncat(e95_strbuf, "...UNTERMINATED_STRING",22);
			       yytext= e95_strbuf;
			       BEGIN(INITIAL);
			       return STRING_CONSTANT;
                        }


<e95_string>"\""        {   /* End of string has been detected */
			    yytext= e95_strbuf;
                            BEGIN(INITIAL);
			    return STRING_CONSTANT;
		        }


"'"                     { return APOSTROPHE;          }
"\\"                    { return BACKSLASH;           }
"`"                     { return BACKTICK;            }
"."                     { return PERIOD;              }
"&"                     { return AMPERSAND;           }
"$"                     { return DOLLAR_SIGN;         }
"@"                     { return AT_SIGN;             }
"#"                     { return NUMBER_SIGN;         }
"-"                     { return MINUS_SIGN;          }
">"                     { return GREATER_THAN_SYMBOL; }


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

/*========================================================
 * Function: convert_escape_char()
 *
 * A method to convert an octal escape sequence
 * to it's decimal value.
 *========================================================
 */
char convert_escape_char(char *escape_code)
{   
    char converted_char=0;

    if(isalpha(escape_code[1]) || ispunct(escape_code[1]))
    {   /* printf("escape code is alphabetic\n"); */
        switch(escape_code[1])
	{
            case 'n':
	      converted_char= 10;
	      break;

            case 't':
	      converted_char= 9;
	      break;

            case 'b':
	      converted_char= 8;
	      break;

            case 'r':
	      converted_char= 13;
	      break;

            case 'f':
	      converted_char= 12;
	      break;

            case 'v':
	      converted_char= 11;
	      break;

            case '\\':
	      converted_char= 92;
	      break;

            case '\'':
	      converted_char= 39;
	      break;

            case '"':
	      converted_char= 34;
	      break;

            case 'a':
	      converted_char= 7;
	      break;

            case '?':
	      converted_char= 63;
	      break;
        }
    }
    else if(isdigit(escape_code[1]))
    {   /* printf("escape code is octal\n"); */
        converted_char= (char) get_octal_value(escape_code);
    }


    return converted_char;
}





/*========================================================
 * Function: get_octal_value()
 *
 * A method to convert an octal escape sequence
 * to it's decimal value.
 *========================================================
 */
int get_octal_value(char *str)
{

#ifdef DEBUG
    fprintf("\n\n\n");
    fprintf("input string: |%s|\n\n", str);
#endif

    /* initialization */
    int len= strlen(str);
    char oct[4]= "000";
    int i,j,sum;



#ifdef DEBUG
    fprintf("String Length: %d\n", len);
#endif

    /* read octal digits into separate string */
    j=2;
    for(i=len-1; i>=0; i--)
    {   if( isdigit(str[i]) )
        {   oct[j--]= str[i];
        }
    }


#ifdef DEBUG
    fprintf("Octal Digits: %s\n", oct);
#endif

    /* calculate decimal value of octal string */
    sum= (int) strtol(oct,NULL,8);

    return sum;
}



