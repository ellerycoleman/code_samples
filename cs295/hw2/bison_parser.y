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
#include "parser_support.h"


%}



%union
{   struct ast *a;
    struct declarator_list *dlist;
    struct parameter_list *plist;
    struct declarator *dp;
    struct tld *tld;
    struct parameter_decl *pdecl;
    char *id;
    int i;
}



%type <a> decl
          function_definition
	  function_def_specifier
	  translation_unit
	  top_level_decl
	  statement
	  comma_expr
	  expression_statement
	  constant
	  conditional_expr
	  unary_expr
	  declaration_or_statement_list
	  declaration_or_statement
          compound_statement
	  null_statement
	  goto_statement
	  continue_statement
	  break_statement
	  assignment_expr
	  parenthesized_expr
	  additive_expr
	  multiplicative_expr
	  cast_expr
	  primary_expr
	  unary_minus_expr
	  unary_plus_expr
	  predecrement_expr
	  postdecrement_expr
	  preincrement_expr
	  postincrement_expr
	  postfix_expr
	  return_statement
	  constant_expr
	  indirection_expr
	  address_expr
	  labeled_statement
	  bitwise_negation_expr
	  logical_negation_expr
	  type_name



%type <dlist> initialized_declarator_list


%type <dp> declarator 
           direct_declarator 
	   simple_declarator 
	   pointer 
	   pointer_declarator 
	   function_declarator 
	   parameter_decl 
	   abstract_declarator 
	   direct_abstract_declarator
	   array_declarator


%type <i> type_specifier 
          signed_type_specifier 
	  unsigned_type_specifier 
	  integer_type_specifier 
	  character_type_specifier
	  add_op
	  mult_op
	  assignment_op


%type <id> label


%type <plist> parameter_list





/* All the tokens formerly declared in e95_tokens.h
 | will now be declared here.
 +-------------------------------------------------*/


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


%token    <i>OP_DIVISION
%token    <i>OP_REMAINDER


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
%token    <i>SEP_SEMICOLON
%token    SEP_COLON
%token    SEP_COMMA


%token    <id> IDENTIFIER
%token    <a>INTEGER_CONSTANT
%token    <a>CHARACTER_CONSTANT
%token    <a>CHARACTER_CONSTANT_OCTAL
%token    <a>STRING_CONSTANT


%token    APOSTROPHE
%token    DOUBLEQUOTE
%token    BACKSLASH
%token    BACKTICK
%token    PERIOD
%token    AMPERSAND
%token    <i>ASTERISK
%token    DOLLAR_SIGN
%token    AT_SIGN
%token    NUMBER_SIGN
%token    <i>MINUS_SIGN
%token    <i>PLUS_SIGN
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
		    {  parse_tree= (struct ast *)new_tld_list($1,NULL);
	            }

|                   translation_unit top_level_decl
                    {  parse_tree= (struct ast *)new_tld_list($2,parse_tree);
	            }
;


top_level_decl:  decl                  {$$= new_tld(DECL,$1);                }
|                function_definition   {$$= new_tld(FUNCTION_DEFINITION,$1); }
;


decl:  type_specifier initialized_declarator_list SEP_SEMICOLON
       {   $$= new_decl((int)(long)$1, $2);
       }
;


type_specifier:  integer_type_specifier
|                RW_VOID    {$$= VOID;}
;


integer_type_specifier:  signed_type_specifier
|                        unsigned_type_specifier
|                        character_type_specifier
;


signed_type_specifier:   RW_SHORT                    {$$=  SIGNED_SHORT_INT; }
|                        RW_SHORT RW_INT             {$$=  SIGNED_SHORT_INT; }
|                        RW_SIGNED RW_SHORT          {$$=  SIGNED_SHORT_INT; }
|                        RW_SIGNED RW_SHORT RW_INT   {$$=  SIGNED_SHORT_INT; }
|                        RW_INT                      {$$=  SIGNED_INT;       }
|                        RW_SIGNED RW_INT            {$$=  SIGNED_INT;       }
|                        RW_SIGNED                   {$$=  SIGNED_INT;       }
|                        RW_LONG                     {$$=  SIGNED_LONG_INT;  }
|                        RW_LONG RW_INT              {$$=  SIGNED_LONG_INT;  }
|                        RW_SIGNED RW_LONG           {$$=  SIGNED_LONG_INT;  }
|                        RW_SIGNED RW_LONG RW_INT    {$$=  SIGNED_LONG_INT;  }
;


unsigned_type_specifier:  RW_UNSIGNED RW_SHORT RW_INT  {$$=  UNSIGNED_SHORT_INT; }
|                         RW_UNSIGNED RW_SHORT         {$$=  UNSIGNED_SHORT_INT; }
|                         RW_UNSIGNED RW_INT           {$$=  UNSIGNED_INT;       }
|                         RW_UNSIGNED                  {$$=  UNSIGNED_INT;       }
|                         RW_UNSIGNED RW_LONG RW_INT   {$$=  UNSIGNED_LONG_INT;  }
|                         RW_UNSIGNED RW_LONG          {$$=  UNSIGNED_LONG_INT;  }
;


character_type_specifier:  RW_CHAR                     {$$= SIGNED_CHAR;   }
|                          RW_SIGNED RW_CHAR           {$$= SIGNED_CHAR;   }
|                          RW_UNSIGNED RW_CHAR         {$$= UNSIGNED_CHAR; }
;


initialized_declarator_list:   declarator
                               {$$= new_declarator_list($1,NULL); }

|                              initialized_declarator_list SEP_COMMA declarator
                               {$$= new_declarator_list($3,$1);   }
;


declarator:       pointer_declarator
|                 direct_declarator
;


pointer_declarator:     pointer direct_declarator
                        {   ($2)->next= $1;
			    $$= reverse_declarators($2);
			}
;


pointer:    ASTERISK
            {   $$= new_pointer_declarator(NULL);
            }
|           ASTERISK pointer
            {   $$= new_pointer_declarator($2);
            }
;


direct_declarator:    simple_declarator
|                     SEP_LEFT_PAREN declarator SEP_RIGHT_PAREN  { $$= $2; }
|                     function_declarator
|                     array_declarator
;


simple_declarator:    IDENTIFIER
                      {   $$= new_simple_declarator($1);
		      }
;


function_declarator:  direct_declarator SEP_LEFT_PAREN parameter_list SEP_RIGHT_PAREN
                      {   $3= reverse_parameter_list($3);
		          $$= new_function_declarator($1,$3);
		      }
;


parameter_list:   parameter_decl
                  {   $$= new_parameter_list($1,NULL);
		  }

|                 parameter_list SEP_COMMA parameter_decl
                  {   $$= new_parameter_list($3,$1);
		  }
;


parameter_decl:  type_specifier declarator                { $$= new_parameter_decl($1,$2);    }
|                type_specifier                           { $$= new_parameter_decl($1,NULL);  }
|                type_specifier abstract_declarator       { $$= new_parameter_decl($1,$2);    }
;


abstract_declarator:   pointer
|                      direct_abstract_declarator
|                      pointer direct_abstract_declarator
;


direct_abstract_declarator:   SEP_LEFT_PAREN abstract_declarator SEP_RIGHT_PAREN
                              {   printf("GRAMMAR: DAD_PAREN_SINGLE_ARG detected..\n");   
			          $$= new_direct_abstract_declarator(DAD_PAREN_SINGLE_ARG,(struct ast *)$2,NULL);
			      }
|                             SEP_LEFT_BRACKET SEP_RIGHT_BRACKET                    /*  int []  */
                              {
			      }
|                             SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET      /*  int [4] */
                              {
			      }
|                             direct_abstract_declarator SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET
;


array_declarator:  direct_declarator SEP_LEFT_BRACKET constant_expr SEP_RIGHT_BRACKET
                   {    $$= new_array_declarator(ARRAY_DECLARATOR,$1,$3);
		   }
;


function_definition:  function_def_specifier compound_statement
                      {    $$= new_function_definition($1,$2);
		      }
;


function_def_specifier: type_specifier declarator
                        {   $$= new_function_def_specifier($1,$2);
			}
;


compound_statement: SEP_LEFT_BRACE SEP_RIGHT_BRACE
                    {   $$= new_compound_statement(NULL);
		    }
|                   SEP_LEFT_BRACE declaration_or_statement_list SEP_RIGHT_BRACE
                    {   $2= (struct ast *)(long)reverse_decostat_list((struct decostat_list *)$2);
		        $$= new_compound_statement($2);
		    }
;


declaration_or_statement_list:   declaration_or_statement
                                 {   $$= new_decostat_list($1,NULL);
				 }
|                                declaration_or_statement_list declaration_or_statement
                                 {   $$= new_decostat_list($2,$1);
				 }
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
             {  $$= new_decostat_list($1,NULL);
	     }
|            comma_expr SEP_COMMA assignment_expr
             {  $$= new_decostat_list($3,$1);
	     }
;


assignment_expr:  conditional_expr
|                 unary_expr assignment_op assignment_expr  { $$= new_expr($2,$1,$3); }
;


assignment_op:  OP_ASSIGNMENT                      { $$= OP_ASSIGNMENT;                }
|               OP_ASSIGNMENT_ADD                  { $$= OP_ASSIGNMENT_ADD;            }                    
|               OP_ASSIGNMENT_SUBTRACT             { $$= OP_ASSIGNMENT_SUBTRACT;       }
|               OP_ASSIGNMENT_MULTIPLY             { $$= OP_ASSIGNMENT_MULTIPLY;       } 
|               OP_ASSIGNMENT_DIVIDE               { $$= OP_ASSIGNMENT_DIVIDE;         }
|               OP_ASSIGNMENT_REMAINDER            { $$= OP_ASSIGNMENT_REMAINDER;      }     
|               OP_ASSIGNMENT_LEFT_BITSHIFT        { $$= OP_ASSIGNMENT_LEFT_BITSHIFT;  }        
|               OP_ASSIGNMENT_RIGHT_BITSHIFT       { $$= OP_ASSIGNMENT_RIGHT_BITSHIFT; }       
|               OP_ASSIGNMENT_BITWISE_AND          { $$= OP_ASSIGNMENT_BITWISE_AND;    }   
|               OP_ASSIGNMENT_BITWISE_XOR          { $$= OP_ASSIGNMENT_BITWISE_XOR;    }  
|               OP_ASSIGNMENT_BITWISE_OR           { $$= OP_ASSIGNMENT_BITWISE_OR;     }
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
                {  $$= new_expr($2,$1,$3); 
		}
;


add_op:   PLUS_SIGN  { $$= PLUS_SIGN;  }
|         MINUS_SIGN { $$= MINUS_SIGN; }
;


multiplicative_expr:  cast_expr
|                     multiplicative_expr mult_op cast_expr
                      {  $$= new_expr($2,$1,$3); 
		      }
;


mult_op:  ASTERISK     { $$= ASTERISK;     }
|         OP_DIVISION  { $$= OP_DIVISION;  }
|         OP_REMAINDER { $$= OP_REMAINDER; }
;


cast_expr:  unary_expr
|           SEP_LEFT_PAREN type_name SEP_RIGHT_PAREN cast_expr
            {  $$= new_expr(CAST_EXPR,$2,$4); 
	       struct declarator *d;
	       d= (struct declarator *)$2;
	       printf("GRAMMAR: typespec of type_name: %d\n", d->typespecifier);
	    }
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
               {   $$= (struct ast *)new_simple_declarator($1);
	       }
|              constant
|              parenthesized_expr
;


constant:  INTEGER_CONSTANT
           {   $$= new_constant(INTEGER_CONSTANT,$1);
	   }
|          CHARACTER_CONSTANT
           {   $$= new_constant(CHARACTER_CONSTANT,$1);
	   }
|          CHARACTER_CONSTANT_OCTAL
           {   $$= new_constant(CHARACTER_CONSTANT_OCTAL,$1);
	   }
|          STRING_CONSTANT
           {   $$= new_constant(STRING_CONSTANT,$1);
	   }
          
;


parenthesized_expr:  SEP_LEFT_PAREN comma_expr SEP_RIGHT_PAREN {  $$= new_expr(PARENTHESIZED_EXPR,$2,NULL); }
;


subscript_expr:  postfix_expr SEP_LEFT_BRACKET comma_expr SEP_RIGHT_BRACKET
;


function_call:   postfix_expr SEP_LEFT_PAREN SEP_RIGHT_PAREN
|                postfix_expr SEP_LEFT_PAREN expression_list SEP_RIGHT_PAREN
;


expression_list:   comma_expr
;


postincrement_expr:  postfix_expr OP_INCREMENT   {  $$= new_expr(POSTINCREMENT_EXPR,$1,NULL); }
;


postdecrement_expr:  postfix_expr OP_DECREMENT   {  $$= new_expr(POSTDECREMENT_EXPR,$1,NULL); }
;


unary_minus_expr:  MINUS_SIGN cast_expr      {  $$= new_expr(UNARY_MINUS_EXPR,$2,NULL); }
;


unary_plus_expr:  PLUS_SIGN cast_expr        {  $$= new_expr(UNARY_PLUS_EXPR,$2,NULL);  }
;


logical_negation_expr:  OP_LOGICAL_NOT cast_expr      {  $$= new_expr(LOGICAL_NEGATION_EXPR,$2,NULL); }
;


bitwise_negation_expr:  OP_ONES_COMPLIMENT cast_expr  {  $$= new_expr(BITWISE_NEGATION_EXPR,$2,NULL); }
;


address_expr:  AMPERSAND cast_expr            {  $$= new_expr(ADDRESS_EXPR,$2,NULL);      }
;


indirection_expr:  ASTERISK cast_expr         {  $$= new_expr(INDIRECTION_EXPR,$2, NULL); }
;


preincrement_expr:  OP_INCREMENT unary_expr   {  $$= new_expr(PREINCREMENT_EXPR,$2,NULL); }
;


predecrement_expr:  OP_DECREMENT unary_expr   {  $$= new_expr(PREDECREMENT_EXPR,$2,NULL); }
;


labeled_statement:  label SEP_COLON statement 
                    {   struct ast *label= new_constant(LABEL,$1);
		        $$= new_expr(LABELED_STATEMENT,label,$3); 
		    }
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
                  {   $$= new_expr(RW_BREAK,NULL,NULL);
		  }
;


continue_statement:  RW_CONTINUE SEP_SEMICOLON
                     {   $$= new_expr(RW_CONTINUE,NULL,NULL);
		     }
;


return_statement:  RW_RETURN SEP_SEMICOLON             { $$= new_expr(RW_RETURN,NULL,NULL); }
|                  RW_RETURN comma_expr SEP_SEMICOLON  { $$= new_expr(RW_RETURN,$2,NULL);   }
;


goto_statement:  RW_GOTO label SEP_SEMICOLON { $$= new_expr(RW_GOTO, (struct ast *)$2,NULL); };
;


null_statement:  SEP_SEMICOLON { $$= new_expr(SEP_SEMICOLON,NULL,NULL); }
;











type_name:  type_specifier                           { $$= (struct ast *)new_parameter_decl($1,NULL);  }
|           type_specifier abstract_declarator       { $$= (struct ast *)new_parameter_decl($1,$2);    }
;


constant_expr:   conditional_expr
;


label:  IDENTIFIER
;




%%
/*-----------------------------------------------------------
 |                    user functions
 +---------------------------------------------------------*/


 /* all user functions for the parser are now defined in parser_support.c */
