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
        switch(token)
	{   ASSIGNMENT:


	    default:
               printf("line: %-5d", yylineno);
               printf("token: %-20s", yytext);
               printf("name: %-20s", token_def_map[token].name);
               printf("type: %-20s\n", token_def_map[token].type);
               token= yylex();
        }
    }



    return 0;
}

