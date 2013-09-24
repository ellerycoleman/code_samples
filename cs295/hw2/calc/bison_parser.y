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

%token INTEGER_CONSTANT

%token END_OF_LINE

%token UNDEFINED







/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%

calclist: /* nothing */
| calclist exp END_OF_LINE    { printf("= %d\n\n", $2); }
;


exp: factor
| exp OP_ADDITION    factor  { $$= $1 + $3; }
| exp OP_SUBTRACTION factor  { $$= $1 - $3; }
;


factor: term
| factor OP_MULTIPLICATION term  { $$= $1 * $3; }
| factor OP_DIVISION       term  { $$= $1 / $3; }
;


term: INTEGER_CONSTANT
| OP_ABSVALUE term  { $$= ($2 > 0)? $2 : - $2; }
;







%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/
yyerror(char *s)
{   fprintf(stderr, "error: %s\n", s);
}

