/*============================================================================
| File:		e95_scanner.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements the main method for the scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "e95_tokens.h"



int yylex();
extern char *yytext;
extern int yyleng;
extern FILE *yyin;
extern int yylineno;
void *yylval;




int main(int argc, char **argv)
{

    init_token_definition_map();


    /* Retrieve first token */
    int token= yylex();


    /* While token is not EOF, continue to process tokens */
    while( token != 0 )
    {
        switch (token)
	{   case END_OF_LINE:
               printf("line: %-3d", yylineno-1);
               printf(" token: %-15s", "\\n");
               printf(" name: %-37s", token_def_map[token].name);
               printf(" type: %-15s\n", token_def_map[token].type);
               token= yylex();
	       break;


            case INTEGER_CONSTANT:
               printf("line: %-3d", yylineno);
               printf(" token: %-15s", yytext);
               printf(" name: %-18s", token_def_map[token].name);
               printf("value: %-12d", atoi(yytext));
               printf(" type: %-15s\n", token_def_map[token].type);
               token= yylex();
	       break;


            case CHARACTER_CONSTANT:
               printf("line: %-3d", yylineno);
               printf(" token: %-15s", yytext);
               printf(" name: %-18s", token_def_map[token].name);
               printf(" value: %-10d", yytext);
               printf(" type: %-15s\n", token_def_map[token].type);
               token= yylex();
	       break;


	    default:
               printf("line: %-3d", yylineno);
               printf(" token: %-15s", yytext);
               printf(" name: %-37s", token_def_map[token].name);
               printf(" type: %-15s\n", token_def_map[token].type);
               token= yylex();
        }
    }



    return 0;
}

