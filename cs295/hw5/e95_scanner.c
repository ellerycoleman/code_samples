/*============================================================================
| File:		e95_scanner.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements the main method for the scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "e95_tokens.h"
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
extern int cvalue;            /* for converting char constants */
extern long long ivalue_tmp;  /* for converting int constants  */
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
	       itype="";
               fprintf(output,"line: %-3d", yylineno);
               fprintf(output," token: %-15s", yytext);
	       ivalue_tmp= strtoll(yytext,NULL,10);
               fprintf(output," name: %-18s", token_def_map[token].name);

	       /*------------------------------------------------------------------  
	        |  Per H&S, if the value can fit in an int, then make it an int.
	        |  Otherwise try a long.
		|  Otherwise try an unsigned long.
		|
		|  For our purposes, int and long are both 4 bytes, so while the
		|  variable names refer to "long", we're actually using ints.
	        +------------------------------------------------------------------  
               */
	       if(ivalue_tmp <= INT_MAX)
	       {   e95_int_constant.ival= (int) ivalue_tmp;
	           itype="int";
                   fprintf(output,"value: %-12d", e95_int_constant.ival);
	       }
	       else if(ivalue_tmp <= LONG_MAX)
	       {   e95_int_constant.lval= (int) ivalue_tmp;
	           itype="long";
                   fprintf(output,"value: %-12d", e95_int_constant.lval);
	       }
	       else if(ivalue_tmp <= ULONG_MAX)
	       {   e95_int_constant.ulval= (unsigned int) ivalue_tmp;
	           itype="unsigned long";
                   fprintf(output,"value: %-12u", e95_int_constant.ulval);
	       }
	       else
	       {   e95_int_constant.ulval= (unsigned int) ivalue_tmp;
	           itype="truncated to unsigned long";
                   fprintf(output,"value: %-12u", e95_int_constant.ulval);
	       }
               fprintf(output," type: %-15s\n", itype);
               token= yylex();
	       break;


            case CHARACTER_CONSTANT:
               fprintf(output,"line: %-3d", yylineno);
               fprintf(output," token: %-15s", yytext);
               fprintf(output," name: %-18s", token_def_map[token].name);
               fprintf(output,"  dec_val: %-9d", cvalue);
               fprintf(output,"type: %-15s\n", token_def_map[token].type);
               token= yylex();
	       break;



            case CHARACTER_CONSTANT_OCTAL:
               fprintf(output,"line: %-3d", yylineno);
               fprintf(output," token: %-15s", yytext);
               fprintf(output," name: %-18s", token_def_map[token].name);
               fprintf(output,"  dec_val: %-9d", cvalue);
               fprintf(output,"type: %-15s\n", token_def_map[token].type);
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




