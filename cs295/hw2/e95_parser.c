/*============================================================================
| File:		e95_parser.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements the main method for the scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "e95_tokens.h"
#include "parser_support.h"
#define INT_MAX       2147483647
#define LONG_MAX      2147483647
#define ULONG_MAX     4294967295


void connect_io(int,char **);
void usage(void);
int yylex();



extern char *yytext;
extern int yyleng;
extern FILE *yyin;
extern int yylineno;
extern int cvalue;              /* for converting char constants */
extern long long ivalue_tmp;    /* for converting int constants  */
extern struct ast *parse_tree;  /* root of completed parse tree  */
FILE *input, *output;


int num_of_tokens_processed=0;



int main(int argc, char **argv)
{
    

    /* Configure input/output sources based on
     * user invocation of command.
     *------------------------------------------*/
     connect_io(argc,argv);
     yyin= input;
     

    /* Initialize token definition map */
    /* init_token_definition_map();    */


    printf("in e95_parser.c::main() about to run yyparse()\n");

    /* Parse Input */
    yyparse();
    parse_tree= (struct ast *) reverse_tld_list((struct tld_list *)parse_tree);
    print_tree(parse_tree);

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




