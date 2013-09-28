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

extern char *yytext;
extern int num_of_tokens_processed;

typedef enum node_type {NODE_OPERATOR,NODE_NUMBER} node_type;

typedef struct node
{   node_type type;
    char *operator;
    struct node *left;
    struct node *right;
    int val;
} node; 

node *malloc_op_node(char *operator, node *child_left, node *child_right);
node *malloc_number_node(int val);
void print_tree(node *);

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

statement:  /* null statement */ {fprintf(stderr,"Entering statement symbol with token '%s'\n",yytext);}
|  statement expr END_OF_LINE    { fprintf(stderr,"= %d\n\nNum_of_tokens: %d\n\n", $2,num_of_tokens_processed);
                                   num_of_tokens_processed = 0;
				 }
;


expr:  {fprintf(stderr,"Entering expr symbol with token '%s'\n", yytext);} term { $$= $2}
|      expr OP_ADDITION    term  { printf("Performing expr addition...\n"); 
                                   $$= $1 + $3; 
				 }
|      expr OP_SUBTRACTION term  { printf("Performing expr subtraction...\n"); 
                                   $$= $1 - $3; 
				 }
;


term:  {fprintf(stderr,"Entering term symbol with token '%s'\n", yytext);} factor { $$= $2}
|      term OP_MULTIPLICATION factor  { printf("Performing term multiplication...\n"); 
                                        $$= $1 * $3; 
				      }
|      term OP_DIVISION       factor  { printf("Performing term division...\n"); 
                                        $$= $1 / $3; 
				      }
;


factor:  {fprintf(stderr,"Entering factor symbol with token '%s'\n", yytext);} INTEGER_CONSTANT {$$= $2}
|        OP_ABSVALUE factor  { $$= ($2 > 0)? $2 : - $2; }
|        SEPARATOR_LPAREN expr SEPARATOR_RPAREN {$$= $2}
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
{   fprintf(stderr, "Problem in Parseville!\n\t");
    fprintf(stderr, "error: %s\n", s);
}



node *malloc_op_node(char *operator, node *child_left, node *child_right)
{   node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_OPERATOR;
        nodeptr->operator= operator;
	nodeptr->left= child_left;
	nodeptr->right= child_right;
    }

    return nodeptr;
}

node *malloc_number_node(int val)
{   node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_NUMBER;
        nodeptr->val= val;
    }

    return nodeptr;
}

void print_tree(node *nodeptr)
{   printf("node_type: %d\n", nodeptr->type);
}


