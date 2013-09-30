/*============================================================================
| File:		bison_parser.y
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A bison program to generate a parser for the E95 subset of
|               the C programming language.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%{

#include "e95_tokens.h"

extern char *yytext;
extern int num_of_tokens_processed;
int answer=0;

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
void print_tree(node *nodeptr);
char *display_node_type(int i);

%}


/* Declare tokens. All the tokens formerly declared in
 | e95_tokens.h file will now be declared here.
 */


%token    OP_ADDITION
%token    OP_DIVISION
%token    OP_REMAINDER


%token    OP_BITWISE_OR
%token    OP_BITWISE_XOR
%token    OP_ONES_COMPLIMENT
%token    OP_LEFT_BITSHIFT
%token    OP_RIGHT_BITSHIFT


%token    OP_RELATIONAL_GTE
%token    OP_RELATIONAL_LT
%token    OP_RELATIONAL_LTE


%token    OP_ASSIGNMENT_ADD
%token    OP_ASSIGNMENT_SUBTRACT
%token    OP_ASSIGNMENT_MULTIPLY
%token    OP_ASSIGNMENT_DIVIDE
%token    OP_ASSIGNMENT_REMAINDER
%token    OP_ASSIGNMENT_LEFT_BITSHIFT
%token    OP_ASSIGNMENT_RIGHT_BITSHIFT
%token    OP_ASSIGNMENT_BITWISE_AND
%token    OP_ASSIGNMENT_BITWISE_OR
%token    OP_ASSIGNMENT_BITWISE_XOR


%token    OP_LOGICAL_AND
%token    OP_LOGICAL_OR
%token    OP_LOGICAL_NOT


%token    OP_EQUALITY
%token    OP_NON_EQUALITY


%token    OP_INCREMENT
%token    OP_DECREMENT


%token    OP_ASSIGNMENT
%token    OP_ASTERISK
%token    OP_QUESTION_MARK


%token    SEP_LEFT_PAREN
%token    SEP_RIGHT_PAREN
%token    SEP_LEFT_CURLY_BRACE
%token    SEP_RIGHT_CURLY_BRACE
%token    SEP_LEFT_SQUARE_BRACKET
%token    SEP_RIGHT_SQUARE_BRACKET
%token    SEP_SEMICOLON
%token    SEP_COLON
%token    SEP_COMMA


%token    RESERVED_WORD_DO
%token    RESERVED_WORD_FOR
%token    RESERVED_WORD_RETURN
%token    RESERVED_WORD_BREAK
%token    RESERVED_WORD_SHORT
%token    RESERVED_WORD_ELSE
%token    RESERVED_WORD_GOTO
%token    RESERVED_WORD_SIGNED
%token    RESERVED_WORD_UNSIGNED
%token    RESERVED_WORD_CHAR
%token    RESERVED_WORD_IF
%token    RESERVED_WORD_VOID
%token    RESERVED_WORD_INT
%token    RESERVED_WORD_CONTINUE
%token    RESERVED_WORD_LONG
%token    RESERVED_WORD_WHILE


%token    IDENTIFIER
%token    INTEGER_CONSTANT
%token    CHARACTER_CONSTANT
%token    CHARACTER_CONSTANT_OCTAL
%token    STRING_CONSTANT


%token    APOSTROPHE
%token    DOUBLEQUOTE
%token    BACKSLASH
%token    BACKTICK
%token    PERIOD
%token    AMPERSAND
%token    DOLLAR_SIGN
%token    AT_SIGN
%token    NUMBER_SIGN
%token    MINUS_SIGN
%token    GREATER_THAN_SYMBOL

%token    END_OF_LINE
%token    END_OF_FILE 0
%token    UNDEFINED




/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%



translation_unit:   top_level_decl END_OF_LINE
		    {  printf("made it back to Point A...\n");
                       print_tree((node *)(long)$1);
		       putchar('\n');
	            }
|                   translation_unit top_level_decl END_OF_LINE
                    {  printf("made it back to Point B...\n");
                       print_tree((node *)(long)$2);
		       putchar('\n');
	            }
;




top_level_decl:  decl { $$= $1; }
;




decl:  declaration_specifier SEP_SEMICOLON
       {  $$= $1;
          node *tmpnode= (node *)(long)$1;
	  printf("validating nodeptr from decl symbol in grammar: %d\n", tmpnode->val);
       }
;




declaration_specifier:  type_specifier { $$= $1; }
;




type_specifier:   integer_type_specifier { $$ = $1; }
|                 void_type_specifier    { $$ = $1; }
;




integer_type_specifier:  term1 { $$ = $1; }
;





void_type_specifier:  term2 { $$ = $1; }
;




term1:  RESERVED_WORD_SHORT { $$= (long)malloc_number_node($1);}
|       RESERVED_WORD_SHORT RESERVED_WORD_INT { $$= (long)malloc_number_node($1);}
;



term2:  CHARACTER_CONSTANT { $$= (long)malloc_number_node($1);}
;




%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/

yyerror(char *s)
{   fprintf(stderr, "Problem in Parseville!\n\t");
    fprintf(stderr, "error: %s\n", s);
}



node *malloc_op_node(char *operator, node *child_left, node *child_right)
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

    node *nodeptr= malloc(sizeof(node));
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

node *malloc_number_node(int val)
{   printf("Entering malloc_number_node()... ");
    node *nodeptr= malloc(sizeof(node));
    if(nodeptr == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   nodeptr->type= NODE_NUMBER;
        nodeptr->val= val;
    }

    printf("returning.\n");
    return nodeptr;
}

void print_tree(node *nodeptr)
{   printf("Entering print_tree()...\n");
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
    {   printf("%d", nodeptr->val);
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
