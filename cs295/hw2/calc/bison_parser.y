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

enum node_type {NODE_OPERATOR,NODE_NUMBER};

struct node
{   enum node_type type;
    char *operator;
    struct node *left;
    struct node *right;
    int val;
};

struct node *malloc_op_node(char *operator, struct node *child_left, struct node *child_right);
struct node *malloc_number_node(int val);
void print_tree(struct node *nodeptr);
char *display_node_type(int i);

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

statement:  /* null statement */ {fprintf(stderr,"Entering statement symbol with token '%s'\n"
                                                 "sizeof(struct node): %d\n\n", yytext,sizeof(struct node));
				 }
|  statement expr END_OF_LINE    { print_tree((struct node *)$2);
                                   fprintf(stderr,"\n\nNum_of_tokens: %d\n\n", num_of_tokens_processed);
                                   num_of_tokens_processed = 0;
				 }
;


expr:  {fprintf(stderr,"Entering expr symbol with token '%s'\n", yytext);} term { $$= $2}
|      expr OP_ADDITION    term  { printf("Performing expr addition...\n"); 
                                   $$= (long)malloc_op_node("+", (struct node *)$1, (struct node *)$3);
				   printf("Validating malloc_op_node return val:\n");
				   struct node *tmp= (struct node *)$$;
				   printf("   * operator: %s\n", tmp->operator);
				   printf("   * left child: %d\n", tmp->left->val);
				   printf("   * right child: %d\n", tmp->right->val);
				 }
;


term:  {fprintf(stderr,"Entering term symbol with token '%s'\n", yytext);} factor { $$= $2}
|      term OP_MULTIPLICATION factor  { printf("Performing term multiplication...\n"); 
                                        $$= (long)malloc_op_node("*", (struct node *)$1, (struct node *)$3);
				      }
;


factor:  {fprintf(stderr,"Entering factor symbol with token '%s'\n", yytext);} INTEGER_CONSTANT {$$= (long)malloc_number_node($2);}
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



struct node *malloc_op_node(char *operator, struct node *child_left, struct node *child_right)
{   printf("Creating an op node with these args:\n"
           "op: '%s'\n"
	   "left: '%d'\n"
	   "right: '%d'\n",
	   operator, child_left, child_right);
    printf("   * left child access test  --> %d\n", child_left->val);
    printf("   * right child access test --> %d\n", child_right->val);

    struct node *nodeptr= malloc(sizeof(struct node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_OPERATOR;
        nodeptr->operator= operator;
	nodeptr->left= child_left;
	nodeptr->right= child_right;
    }

    printf("Returning from malloc_op_node...\n");
    return nodeptr;
}

struct node *malloc_number_node(int val)
{   printf("Creating a number node with this arg: '%d'\n", val);


    struct node *nodeptr= malloc(sizeof(struct node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_NUMBER;
        nodeptr->val= val;
    }

    printf("Number node allocated... access test --> %d\n", nodeptr->val);
    return nodeptr;
}

void print_tree(struct node *nodeptr)
{   struct node *tmp= nodeptr;
    printf("preparing to print tree...\n");
    printf("node_type: %s\n", display_node_type(tmp->type));
    printf("Exiting print_tree()...\n");
}

char *display_node_type(int i)
{   if(i == 0)
    {   return "Operator Node";
    }
    else
    {   return "Number Node";
    }
}
