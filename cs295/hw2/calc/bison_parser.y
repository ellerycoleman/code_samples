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
                                   printf("\n");
                                   fprintf(stderr,"\n\nNum_of_tokens: %d\n\n", num_of_tokens_processed);
                                   num_of_tokens_processed = 0;
				 }
;


expr:  {fprintf(stderr,"Entering expr symbol with token '%s'\n", yytext);} term { $$= $2}
|      expr OP_ADDITION    term  { printf("Performing expr addition...\n"); 
                                   $$= (long)malloc_op_node("+", (struct node *)$1, (struct node *)$3);
				 }
|      expr OP_SUBTRACTION term  { printf("Performing expr addition...\n"); 
                                   $$= (long)malloc_op_node("-", (struct node *)$1, (struct node *)$3);
				 }
;


term:  {fprintf(stderr,"Entering term symbol with token '%s'\n", yytext);} factor { $$= $2}
|      term OP_MULTIPLICATION factor  { printf("Performing term multiplication...\n"); 
                                        $$= (long)malloc_op_node("*", (struct node *)$1, (struct node *)$3);
				      }
|      term OP_DIVISION       factor  { printf("Performing term multiplication...\n"); 
                                        $$= (long)malloc_op_node("/", (struct node *)$1, (struct node *)$3);
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
{   printf("Entering malloc_op_node with op '%s', left child '%d', right child '%d'...\n", operator,child_left->val, child_right->val);
#ifdef DEBUG
    printf("Creating an op node with these args:\n"
           "op: '%s'\n"
	   "left: '%d'\n"
	   "right: '%d'\n",
	   operator, child_left, child_right);
    printf("   * left child access test  --> %d\n", child_left->val);
    printf("   * right child access test --> %d\n", child_right->val);
#endif

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
{   struct node *nodeptr= malloc(sizeof(struct node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_NUMBER;
        nodeptr->val= val;
    }

    return nodeptr;
}

void print_tree(struct node *nodeptr)
{   
    printf("(");
    if(nodeptr->type == NODE_OPERATOR)
    {   
        /* print left branch of tree */
        if(nodeptr->left->type == NODE_OPERATOR)
        {   print_tree(nodeptr->left);
	}
	else
	{   printf("%d ", nodeptr->left->val);
	}

        /* print root of tree */
	printf("%s ", nodeptr->operator);


        /* print right branch of tree */
	if(nodeptr->right->type == NODE_OPERATOR)
        {   print_tree(nodeptr->right);
	}
	else
	{   printf("%d ", nodeptr->right->val);
	}
    }
    if(nodeptr->type == NODE_NUMBER)
    {   printf("%d\n", nodeptr->val);
    }
    printf(")");
}

char *display_node_type(int i)
{   if(i == 0)
    {   return "Operator Node";
    }
    else
    {   return "Number Node";
    }
}
