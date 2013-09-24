/*============================================================================
| File:		calc_scanner.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements the main method for the scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "calc_tokens.h"


void connect_io(int,char **);
void usage(void);
int yylex();



extern char *yytext;
extern int yyleng;
extern FILE *yyin;
extern int yylineno;
FILE *input, *output;



int main(int argc, char **argv)
{
     int int_constant;

    /* Configure input/output sources based on
     * user invocation of command.
     *------------------------------------------*/
     connect_io(argc,argv);
     yyin= input;
     

    /* Initialize token definition map */
    init_token_definition_map();


    /* Retrieve first token */
    int token= yylex();


    /* While token is not EOF, continue to process tokens */
    while( token != 0 )
    {
        switch (token)
	{   case END_OF_LINE:
               fprintf(output,"line: %-3d", yylineno-1);
               fprintf(output," token: %-15s", "\\n");
               fprintf(output," name: %-37s", token_def_map[token].name);
               fprintf(output," type: %-15s\n", token_def_map[token].type);
               token= yylex();
	       break;


            case INTEGER_CONSTANT:
               fprintf(output,"line: %-3d", yylineno);
               fprintf(output," token: %-15s", yytext);
	       int_constant= atoi(yytext);
               fprintf(output," name: %-18s", token_def_map[token].name);
               fprintf(output,"value: %-12d\n", int_constant);
               token= yylex();
	       break;



	    default:
               fprintf(output,"line: %-3d", yylineno);
               fprintf(output," token: %-15s", yytext);
               fprintf(output," name: %-37s", token_def_map[token].name);
               fprintf(output," type: %-15s\n", token_def_map[token].type);
               token= yylex();
        }
    }



    return 0;
}






/*===============================================================
 * Function: connect_io()
 * This method contains code from scanner_main.c provided
 * on the section website.
 *===============================================================
 */
void connect_io(int argc,char **argv)
{   
    if(argc == 3)
    {   
#ifdef DEBUG
        printf("argc == 3\n\n");
	printf("argv[1]: %s\n", argv[1]);
	printf("argv[2]: %s\n", argv[2]);
#endif
        /* connect input  */
        if(strcmp("-",argv[1]))
	{   

#ifdef DEBUG
	    printf("connecting input: %s\n", argv[1]);
#endif
	    input= fopen(argv[1], "r");
	}
	else
	{   input= stdin;
	}

	/* connect output */
	if(strcmp("-",argv[2]))
	{   
#ifdef DEBUG
   	    printf("connecting output: %s\n", argv[2]);
#endif
	    output= fopen(argv[2], "w");
	}
	else
	{   output= stdout;
	}
    }
    else if(argc <= 2)
    {   input= stdin;
        output= stdout;
    }
    else
    {   usage();
    }
}




/*===============================================================
 * Function: usage()
 * Writes proper scanner usage notice to STDOUT.
 *===============================================================
 */
void usage(void)
{   printf("\n\nUsage: scanner [input_file output_file]\n\n");
    exit(-1);
}




