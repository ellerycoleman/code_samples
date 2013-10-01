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
%token SEPARATOR_LPAREN;
%token SEPARATOR_RPAREN;
%token SEPARATOR_SEMICOLON;

%token <i> INTEGER_CONSTANT

%token END_OF_LINE

%token UNDEFINED

%type <n> expr term factor





/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%

statement:  /* null statement */ {fprintf(stderr,"Entering statement symbol with token '%s'\n"
                                                 "sizeof(node): %d\n\n", yytext,sizeof(node));
				 }
|  statement expr END_OF_LINE    { printf("reduced to statement...\n");
                                   printf("= %d\n", eval($2));

                                   fprintf(stderr,"\n\nNum_of_tokens: %d\n\n", num_of_tokens_processed);
                                   num_of_tokens_processed = 0;

				   printf("> ");
				 }
;


expr:  {fprintf(stderr,"Entering expr symbol with token '%s'\n", yytext);} 
       term { $$= $2}
|      expr OP_ADDITION    term  { printf("Performing expr addition...\n"); 
                                   node *tmp1= $1;
                                   node *tmp3= $3;
				   printf("tmp1: %d\n", tmp1->val);
				   printf("tmp3: %d\n", tmp3->val);
                                   $$= malloc_op_node('+',$1,$3);
				   printf("Completed assignment... leaving expr addition\n");
				 }
|      expr OP_SUBTRACTION term  { printf("Performing expr addition...\n"); 
                                   /* $$= malloc_op_node('-', (node *)$1, (node *)$3); */
				 }
;


term:  {fprintf(stderr,"Entering term symbol with token '%s'\n", yytext);} factor { $$= $2}
|      term OP_MULTIPLICATION factor  { printf("Performing term multiplication...\n"); 
                                        /* $$= malloc_op_node('*', (node *)$1, (node *)$3); */
				      }
|      term OP_DIVISION       factor  { printf("Performing term multiplication...\n"); 
                                        /* $$= malloc_op_node('/', (node *)$1, (node *)$3);  */
				      }
;


factor:  {fprintf(stderr,"Entering factor symbol with token '%s'\n", yytext);} 
         INTEGER_CONSTANT { $$=  malloc_number_node($2);
                            struct node *tmp= $$;
			    printf("just created node for int constant '%d'\n", tmp->val);

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



