/*============================================================================
| File:		bison_parser.y
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A bison program to generate a parser for a simple calculator.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%{

#include "calc_tokens.h"
#include "parser_support.h"

int answer = 0;

%}



%union { struct node *n;
         int   i;
       }



/* declare tokens; these should align with the token definitions
 | that were previously kept in the calc_tokens.h file.
 */

%token OP_ADDITION
%token OP_SUBTRACTION
%token OP_MULTIPLICATION
%token OP_DIVISION
%token OP_ABSVALUE
%token SEP_LPAREN;
%token SEP_RPAREN;
%token SEP_SEMICOLON;

%token <i> INTEGER_CONSTANT

%token END_OF_LINE

%token UNDEFINED

%type <n> expr



%left OP_ADDITION OP_SUBTRACTION
%left OP_MULTIPLICATION OP_DIVISION




/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%

statement:  /* null statement */ 


|  statement expr END_OF_LINE    { fprintf(stderr,"\nNum_of_tokens: %d\n\n", num_of_tokens_processed);
                                   print_tree($2);
                                   printf("\n= %d\n", eval($2));

                                   num_of_tokens_processed = 0;
				   printf("\n> ");
				 }


expr:  expr OP_ADDITION       expr  { $$= malloc_op_node('+',$1,$3);
				    }


|      expr OP_SUBTRACTION    expr  { $$= malloc_op_node('-',$1,$3);
				    }


|      expr OP_MULTIPLICATION expr  { $$= malloc_op_node('*',$1,$3);
				    }


|      expr OP_DIVISION       expr  { $$= malloc_op_node('/', (node *)$1, (node *)$3);
				    }


|      SEP_LPAREN expr SEP_RPAREN   { $$= $2;  }



|      INTEGER_CONSTANT { $$=  malloc_number_node($1);
                          struct node *tmp= $$;
                        }
;







%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/
int main(void)
{   printf("> ");
    yyparse();
    return 0;
}


yyerror(char *s)
{   fprintf(stderr, "Problem in Parseville!\n\t");
    fprintf(stderr, "error: %s\n", s);
}


/* All other user-defined parse functions are defined in parser_support.c */




