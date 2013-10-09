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

typedef enum ntype {NODE_OPERATOR,NODE_NUMBER,DECL,DECLARATOR,TLD,TLD_LIST,FUNCDEF} ntype;

typedef enum tspec {   SIGNED_SHORT_INT,
		       SIGNED_LONG_INT,
                       SIGNED_INT,
		       SIGNED_CHAR,
		       UNSIGNED_SHORT_INT,
		       UNSIGNED_LONG_INT,
		       UNSIGNED_INT,
		       UNSIGNED_CHAR,
		       VOID
                   } tspec;


typedef struct ast
{   ntype nodetype;
    struct ast *left;
    struct ast *right;
} ast;


typedef struct numval
{   ntype nodetype;
    int val;
} numval;


typedef struct sdeclarator
{   ntype nodetype;
    char *id;
} sdeclarator;


typedef struct declarator
{   char *id;
} declarator;


typedef struct declarator_list
{   declarator *d;
    struct declarator_list *next;
} declarator_list;


typedef struct funcdef
{   ntype nodetype;
} funcdef;


typedef struct decl
{   ntype nodetype;
    tspec typespecifier;
    struct declarator_list *dl;
} decl;


typedef struct tld
{   ntype nodetype;
    int datatype; /* 1 is decl, 2 is funcdef */
    struct decl    *d;
    struct funcdef *f;
} tld;


typedef struct tld_list
{   ntype nodetype;
    struct tld *tld;
    struct tld_list *next;
} tld_list;



declarator *new_simple_declarator(char *id);
ast *new_tld_list(tld *t, ast *next);
tld *new_tld(int datatype, ast *t);
ast *new_decl(int typespecifier, declarator_list *dl);
void print_tree(ast *nodeptr);
declarator_list *new_declarator_list(declarator *d, declarator_list *next);
declarator_list *reverse_declarator_list(declarator_list *dl);

%}



%union
{   struct ast *a;
    struct decl *decl;
    struct sdeclarator *sdeclarator;
    struct declarator_list *dlist;
    struct declarator *declarator;
    struct tld_list *tlist;
    struct tld *tld;
    char *id;
}


%type <a> type_specifier signed_type_specifier unsigned_type_specifier character_type_specifier integer_type_specifier decl function_definition function_def_specifier compound_statement translation_unit 

%type <tld> top_level_decl

%type <dlist> initialized_declarator_list

%type <declarator> declarator pointer_declarator direct_declarator simple_declarator


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


%token    <id> IDENTIFIER
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
		       $$= new_tld_list($1,NULL);
		       print_tree($$);
		       putchar('\n');
	            }
|                   translation_unit top_level_decl
                    {  printf("made it back to Point B...\n");
		       $$= new_tld_list($2,$1);
		       print_tree($$);
		       putchar('\n');
	            }
;


top_level_decl:  decl                  {$$= new_tld(DECL,$1); }
|                function_definition 
;


decl:  type_specifier initialized_declarator_list SEP_SEMICOLON
       {   printf ("encountered a decl...\n");
           $$= new_decl((int)$1, $2);
       }
;


type_specifier:  integer_type_specifier  
|                RW_VOID                 {$$= (ast *)(long) VOID;             }
;


integer_type_specifier:  signed_type_specifier
|                        unsigned_type_specifier
|                        character_type_specifier
;


signed_type_specifier:   RW_SHORT                    {$$= (ast *) SIGNED_SHORT_INT; }
|                        RW_SHORT RW_INT             {$$= (ast *) SIGNED_SHORT_INT; }
|                        RW_SIGNED RW_SHORT          {$$= (ast *) SIGNED_SHORT_INT; }
|                        RW_SIGNED RW_SHORT RW_INT   {$$= (ast *) SIGNED_SHORT_INT; }
|                        RW_INT                      {$$= (ast *) SIGNED_INT;       }
|                        RW_SIGNED RW_INT            {$$= (ast *) SIGNED_INT;       }
|                        RW_SIGNED                   {$$= (ast *) SIGNED_INT;       }
|                        RW_LONG                     {$$= (ast *) SIGNED_LONG_INT;  }
|                        RW_LONG RW_INT              {$$= (ast *) SIGNED_LONG_INT;  }
|                        RW_SIGNED RW_LONG           {$$= (ast *) SIGNED_LONG_INT;  }
|                        RW_SIGNED RW_LONG RW_INT    {$$= (ast *) SIGNED_LONG_INT;  }
;


unsigned_type_specifier:  RW_UNSIGNED RW_SHORT RW_INT  {$$= (ast *) UNSIGNED_SHORT_INT; }
|                         RW_UNSIGNED RW_SHORT         {$$= (ast *) UNSIGNED_SHORT_INT; }
|                         RW_UNSIGNED RW_INT           {$$= (ast *) UNSIGNED_INT;       }
|                         RW_UNSIGNED                  {$$= (ast *) UNSIGNED_INT;       }
|                         RW_UNSIGNED RW_LONG RW_INT   {$$= (ast *) UNSIGNED_LONG_INT;  }
|                         RW_UNSIGNED RW_LONG          {$$= (ast *) UNSIGNED_LONG_INT;  }
;


character_type_specifier:  RW_CHAR                     {$$= (ast *) SIGNED_CHAR;   }
|                          RW_SIGNED RW_CHAR           {$$= (ast *) SIGNED_CHAR;   }
|                          RW_UNSIGNED RW_CHAR         {$$= (ast *) UNSIGNED_CHAR; }
;


initialized_declarator_list:   declarator                                       {$$= new_declarator_list($1,NULL); }
|                              initialized_declarator_list SEP_COMMA declarator {$$= new_declarator_list($3,$1);   }
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


simple_declarator:    IDENTIFIER   {  $$= new_simple_declarator($1); }
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


array_declarator:  direct_declarator SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET
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
|           break_statement
|           continue_statement
|           return_statement
|           goto_statement
|           null_statement
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


break_statement:  RW_BREAK SEP_SEMICOLON
;


continue_statement:  RW_CONTINUE SEP_SEMICOLON
;


return_statement:  RW_RETURN SEP_SEMICOLON
|                  RW_RETURN comma_expr SEP_SEMICOLON
;


goto_statement:  RW_GOTO label SEP_SEMICOLON
;


null_statement:  SEP_SEMICOLON {}
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




void print_tree(ast *nodeptr)
{   printf("Entering print_tree()...\n");
    printf("(");

    /* The only type of AST that is passed to print_tree is a *tld_list.
     | we will cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each one pointing to either
     | a decl or a funcdef.  AT this point, we only handle decls.
     +-----------------------------------------------------------------*/
    do
    {

    struct decl *de= (struct decl *)tldlist->tld->d;
    switch(de->nodetype)
    {   case DECL:
           printf("this tldlist node is a DECL...\n");
	   printf("  - type of decl is '%d'\n", de->typespecifier);
           switch( de->typespecifier)
	   {   case SIGNED_SHORT_INT:
	          printf("signed short int ");
		  break;
               case SIGNED_LONG_INT:
	          printf("signed long int ");
		  break;
               case SIGNED_INT:
	          printf("signed int ");
		  break;
               case SIGNED_CHAR:
	          printf("signed char ");
		  break;
               case UNSIGNED_SHORT_INT:
	          printf("unsigned short int ");
		  break;
               case UNSIGNED_LONG_INT:
	          printf("unsigned long int ");
		  break;
               case UNSIGNED_INT:
	          printf("unsigned int ");
		  break;
               case UNSIGNED_CHAR:
	          printf("unsigned char ");
		  break;
               case VOID:
	          printf("void ");
		  break;
	   }
	   declarator_list *dl= de->dl;
	   dl= reverse_declarator_list(dl);
	   do
	   {   printf(" %s", dl->d->id);
	       if(dl->next != NULL)
	       {  printf(",");
               }
	   }while( (dl= dl->next) != NULL);
	   printf(";\n");
	   break;
    }

    }while( (tldlist= tldlist->next) != NULL );


    printf(")\n");
}





declarator *new_simple_declarator(char *id)
{   printf("new_simple_declarator() was called with id '%s'\n", id);
    declarator *d= malloc(sizeof(declarator));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->id= strdup(id);
    }

    return d;
}


declarator_list *new_declarator_list(declarator *d, declarator_list *next)
{   printf("new_declarator_list() was called...\n");
    declarator_list *dl= malloc(sizeof(struct declarator_list));
    if(dl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   dl->d= d;
        dl->next= next;
    }
    return dl;
}


ast *new_tld_list(tld *t, ast *next)
{   printf("new_tld_list() was called...\n");
    tld_list *tl= malloc(sizeof(struct tld_list));
    if(tl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   tl->tld= t;
        tl->next= (struct tld_list *)next;
    }
    return (struct ast *)tl;
}


/*
 | The declarator list is defined using left recursion.
 | For this reason, the linked list is created in the
 | reverse order of the way that it was entered.  This
 | method reverses the linked list, and is based on the
 | linked list reversal method posted on stackoverflow.com.
 +---------------------------------------------------------*/
declarator_list *reverse_declarator_list(declarator_list *dl)
{   declarator_list *newroot= NULL;   
    while(dl)
    {   declarator_list *next= dl->next;
        dl->next= newroot;
	newroot= dl;
	dl= next;
    }
    return newroot;
}




ast *new_decl(int typespecifier, declarator_list *dl)
{   printf("new_decl() was called...\n");
    decl *d= malloc(sizeof(struct decl));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->nodetype= DECL;
        d->typespecifier= typespecifier;
        d->dl=  dl;
    }

    return  (struct ast *)d;
}

tld *new_tld(int datatype, ast *tld)
{   printf("new_tld() was called...\n");
    struct tld *t= malloc(sizeof(struct tld));
    if(t == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   t->nodetype= TLD;
        t->datatype= datatype;
        if(datatype == DECL)
	{   t->d= (struct decl *)tld;
	}
	if(datatype == FUNCDEF)
	{   t->f= (struct funcdef *)tld;
	}
    }    
    return t;
}







