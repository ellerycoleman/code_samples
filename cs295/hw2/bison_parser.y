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



%token    RW_DO
%token    RW_FOR
%token    RW_RETURN
%token    RW_BREAK
%token    RW_SHORT
%token    RW_ELSE
%token    RW_GOTO
%token    RW_SIGNED
%token    RW_UNSIGNED
%token    RW_CHAR
%token    RW_IF
%token    RW_VOID
%token    RW_INT
%token    RW_CONTINUE
%token    RW_LONG
%token    RW_WHILE


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
%token    OP_QUESTION_MARK


%token    SEP_LEFT_PAREN
%token    SEP_RIGHT_PAREN
%token    SEP_LEFT_BRACE
%token    SEP_RIGHT_BRACE
%token    SEP_LEFT_BRACKET
%token    SEP_RIGHT_BRACKET
%token    SEP_SEMICOLON
%token    SEP_COLON
%token    SEP_COMMA


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
%token    ASTERISK
%token    DOLLAR_SIGN
%token    AT_SIGN
%token    NUMBER_SIGN
%token    MINUS_SIGN
%token    PLUS_SIGN
%token    GREATER_THAN_SYMBOL

%token    END_OF_LINE
%token    END_OF_FILE 0
%token    UNDEFINED


%right "then" RW_ELSE


/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%



translation_unit:   top_level_decl
		    {  printf("made it back to Point A...\n");
		       putchar('\n');
	            }
|                   translation_unit top_level_decl
                    {  printf("made it back to Point B...\n");
		       putchar('\n');
	            }
;


top_level_decl:  decl
|                function_definition
;


decl:  type_specifier initialized_declarator_list SEP_SEMICOLON
;


type_specifier:  integer_type_specifier
|                RW_VOID
;


integer_type_specifier:  signed_type_specifier
|                        unsigned_type_specifier
|                        character_type_specifier
;


signed_type_specifier:   RW_SHORT
|                        RW_SHORT RW_INT
|                        RW_SIGNED RW_SHORT
|                        RW_SIGNED RW_SHORT RW_INT
|                        RW_INT
|                        RW_SIGNED RW_INT
|                        RW_SIGNED
|                        RW_LONG
|                        RW_LONG RW_INT
|                        RW_SIGNED RW_LONG
|                        RW_SIGNED RW_LONG RW_INT
;


unsigned_type_specifier:  RW_UNSIGNED RW_SHORT RW_INT
|                         RW_UNSIGNED RW_SHORT
|                         RW_UNSIGNED RW_INT
|                         RW_UNSIGNED
|                         RW_UNSIGNED RW_LONG RW_INT
|                         RW_UNSIGNED RW_LONG
;


character_type_specifier:  RW_CHAR
|                          RW_SIGNED RW_CHAR
|                          RW_UNSIGNED RW_CHAR
;


initialized_declarator_list:   declarator
|                              initialized_declarator_list SEP_COMMA declarator
;



declarator:       pointer_declarator
|                 direct_declarator
;


pointer_declarator:     pointer direct_declarator
;


pointer:    ASTERISK
|           ASTERISK pointer
;


direct_declarator:    simple_declarator
|                     SEP_LEFT_PAREN declarator SEP_RIGHT_PAREN
|                     function_declarator
|                     array_declarator
;



simple_declarator:    IDENTIFIER
;


function_declarator:  direct_declarator SEP_LEFT_PAREN parameter_list SEP_RIGHT_PAREN
;


parameter_list:   parameter_decl
|                 parameter_list SEP_COMMA parameter_decl
;


parameter_decl:  type_specifier declarator
|                type_specifier
|                type_specifier abstract_declarator
;


abstract_declarator:   pointer
|                      direct_abstract_declarator
|                      pointer direct_abstract_declarator
;


direct_abstract_declarator:   SEP_LEFT_PAREN abstract_declarator SEP_RIGHT_PAREN
|                             SEP_LEFT_BRACKET SEP_RIGHT_BRACKET
|                             SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET
|                             direct_abstract_declarator SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET
;


array_declarator:  direct_declarator SEP_LEFT_BRACKET INTEGER_CONSTANT SEP_RIGHT_BRACKET
;


function_definition:  function_def_specifier compound_statement
;


function_def_specifier: type_specifier declarator
;


compound_statement: SEP_LEFT_BRACE SEP_RIGHT_BRACE
|                   SEP_LEFT_BRACE declaration_or_statement_list SEP_RIGHT_BRACE
;


declaration_or_statement_list:   declaration_or_statement
|                                declaration_or_statement_list declaration_or_statement
;


declaration_or_statement:   decl
|                           statement
;


statement:  expression_statement
|           labeled_statement
|           compound_statement
|           conditional_statement
|           iterative_statement
;


expression_statement:   comma_expr SEP_SEMICOLON
;


comma_expr:  assignment_expr
|            comma_expr SEP_COMMA assignment_expr
;


assignment_expr:  conditional_expr
|                 unary_expr assignment_op assignment_expr
;


assignment_op:  OP_ASSIGNMENT
|               OP_ASSIGNMENT_ADD
|               OP_ASSIGNMENT_SUBTRACT
|               OP_ASSIGNMENT_MULTIPLY
|               OP_ASSIGNMENT_DIVIDE
|               OP_ASSIGNMENT_REMAINDER
|               OP_ASSIGNMENT_LEFT_BITSHIFT
|               OP_ASSIGNMENT_RIGHT_BITSHIFT
|               OP_ASSIGNMENT_BITWISE_AND
|               OP_ASSIGNMENT_BITWISE_XOR
|               OP_ASSIGNMENT_BITWISE_OR
;


conditional_expr:  logical_or_expr
|                  logical_or_expr OP_QUESTION_MARK comma_expr SEP_COLON conditional_expr
;


logical_or_expr:   logical_and_expr
|                  logical_or_expr OP_LOGICAL_OR logical_and_expr
;


logical_and_expr:  bitwise_or_expr
|                  logical_and_expr OP_LOGICAL_AND bitwise_or_expr
;


bitwise_or_expr:  bitwise_xor_expr
|                 bitwise_or_expr OP_BITWISE_OR bitwise_xor_expr
;


bitwise_xor_expr:  bitwise_and_expr
|                  bitwise_xor_expr OP_BITWISE_XOR bitwise_and_expr
;


bitwise_and_expr:  equality_expr
|                  bitwise_and_expr AMPERSAND equality_expr
;


equality_expr:  relational_expr
|               equality_expr equality_op relational_expr
;


equality_op:  OP_EQUALITY
|             OP_NON_EQUALITY
;


relational_expr:  shift_expr
|                 relational_expr relational_op shift_expr
;


relational_op:  OP_RELATIONAL_LT
|               OP_RELATIONAL_LTE
|               GREATER_THAN_SYMBOL
|               OP_RELATIONAL_GTE
;


shift_expr:   additive_expr
|             shift_expr shift_op additive_expr
;


shift_op:  OP_LEFT_BITSHIFT
|          OP_RIGHT_BITSHIFT
;


additive_expr:  multiplicative_expr
|               additive_expr add_op multiplicative_expr
;


add_op:   PLUS_SIGN
|         MINUS_SIGN
;


multiplicative_expr:  cast_expr
|                     multiplicative_expr mult_op cast_expr
;


mult_op:  ASTERISK
|         OP_DIVISION
|         OP_REMAINDER
;


cast_expr:  unary_expr
|           SEP_LEFT_PAREN type_name SEP_RIGHT_PAREN cast_expr
;


unary_expr:  postfix_expr
|            unary_minus_expr
|            unary_plus_expr
|            logical_negation_expr
|            bitwise_negation_expr
|            address_expr
|            indirection_expr
|            preincrement_expr
|            predecrement_expr
;


postfix_expr:  primary_expr
|              subscript_expr
|              function_call
|              postincrement_expr
|              postdecrement_expr
;


primary_expr:  IDENTIFIER
|              constant
|              parenthesized_expr
;


constant:  INTEGER_CONSTANT
|          CHARACTER_CONSTANT
|          STRING_CONSTANT
;


parenthesized_expr:  SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN
;


subscript_expr:  postfix_expr SEP_LEFT_BRACKET comma_expr SEP_RIGHT_BRACKET
;


function_call:   postfix_expr SEP_LEFT_PAREN SEP_RIGHT_PAREN
|                postfix_expr SEP_LEFT_PAREN expression_list SEP_RIGHT_PAREN
;


expression_list:   assignment_expr
|                  expression_list SEP_COMMA assignment_expr
;


postincrement_expr:  postfix_expr OP_INCREMENT
;


postdecrement_expr:  postfix_expr OP_DECREMENT
;


unary_minus_expr:  MINUS_SIGN cast_expr
;


unary_plus_expr:  PLUS_SIGN cast_expr
;


logical_negation_expr:  OP_LOGICAL_NOT cast_expr
;


bitwise_negation_expr:  OP_ONES_COMPLIMENT cast_expr
;


address_expr:  AMPERSAND cast_expr
;


indirection_expr:  ASTERISK cast_expr
;


preincrement_expr:  OP_INCREMENT unary_expr
;


predecrement_expr:  OP_DECREMENT unary_expr
;


labeled_statement:  label SEP_COLON statement
;

conditional_statement:  if_statement
|                       if_else_statement
;


if_statement:       RW_IF SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN statement %prec "then"
; /* this symbol uses %prec to resolve the shift/reduce error, per advice on stackoverflow.com */


if_else_statement:  RW_IF SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN statement RW_ELSE statement
;


iterative_statement:  while_statement
|                     do_statement
|                     for_statement
;


while_statement:  RW_WHILE SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN statement
;


do_statement:  RW_DO statement RW_WHILE SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN SEP_SEMICOLON
;


for_statement: RW_FOR for_expr statement
;


for_expr:  SEP_LEFT_PAREN SEP_SEMICOLON SEP_SEMICOLON SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN comma_expr SEP_SEMICOLON SEP_SEMICOLON SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN SEP_SEMICOLON comma_expr SEP_SEMICOLON SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN SEP_SEMICOLON SEP_SEMICOLON comma_expr SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN comma_expr SEP_SEMICOLON comma_expr SEP_SEMICOLON SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN comma_expr SEP_SEMICOLON SEP_SEMICOLON comma_expr SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN SEP_SEMICOLON comma_expr SEP_SEMICOLON comma_expr SEP_RIGHT_PAREN
|          SEP_LEFT_PAREN comma_expr SEP_SEMICOLON comma_expr SEP_SEMICOLON comma_expr SEP_RIGHT_PAREN
;










type_name:  type_specifier
|           type_specifier abstract_declarator
;


constant_expr:   conditional_expr
;


label:  IDENTIFIER
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
