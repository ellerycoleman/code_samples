/*============================================================================
| File:		scanner_main.c
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
extern char **token_type;
extern char *token_name;

void *yylval;




int main(int argc, char **argv)
{   
     
     init_token_typemap();


    /* Retrieve first token */
     int token= yylex();




    /* While token is not EOF, continue to process tokens */
    while( token != 0 )
    {   printf("token: %-20s", yytext);
        printf("name: %-20s", token_name);
        printf("type: %20s\n", token_type[token]);
        token= yylex();
    }



    return 0;
}
