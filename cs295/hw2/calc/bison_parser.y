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

%}


/* declare tokens; these should align with the definitions
 | in the calc_tokens.h file.
 */

%token OP_ADDITION
%token OP_SUBTRACTION
%token OP_MULTIPLICATION
%token OP_DIVISION
%token OP_ABSVALUE
%token SEPARATOR_LPAREN;
%token SEPARATOR_RPAREN;
%token SEPARATOR_SEMICOLON;

%token INTEGER_CONSTANT

%token END_OF_LINE

%token UNDEFINED







/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%

statement:  /* null statement */
| statement expr END_OF_LINE    { printf("= %d\n\n", $2); }
;


expr:  expr OP_ADDITION    term  { $$= $1 + $3; }
|      expr OP_SUBTRACTION term  { $$= $1 - $3; }
|      term;
;


term:  term OP_MULTIPLICATION factor  { $$= $1 * $3; }
|      term OP_DIVISION       factor  { $$= $1 / $3; }
|      factor;
;


factor:  OP_ABSVALUE term  { $$= ($2 > 0)? $2 : - $2; }
|        SEPARATOR_LPAREN expr SEPARATOR_RPAREN {$$= $2};
|        INTEGER_CONSTANT;
;







%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/
int main(void)
{   yyparse();
    return 0;
}


yyerror(char *s)
{   fprintf(stderr, "error: %s\n", s);
}

