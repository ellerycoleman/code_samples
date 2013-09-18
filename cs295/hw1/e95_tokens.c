/*============================================================================
| File:		e95_tokens.c
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Implements data structures for classifying tokens.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

#include "e95_tokens.h"


extern e95_token token_def_map[];





void init_token_definition_map()
{   

    /*-----------------------------------------------------------------+
      |           Define all token types
      +----------------------------------------------------------------*/
    char *math_op= "Arithmetic Operator";
    char *general_op= "General Operator";
    char *bitwise_op= "Bitwise Operator";
    char *separator= "Separator";






    /*-----------------------------------------------------------------+
      |              Define all tokens 
      +---------------------------------------------------------------*/

    /* Basic Arithmetic Operators 
     *---------------------------*/
    e95_token op_addition_t        = {"Addition",        math_op};
    e95_token op_subtraction_t     = {"Subtraction",     math_op};
    e95_token op_multiplication_t  = {"Multiplication",  math_op};
    e95_token op_division_t        = {"Division",        math_op};


    /* Compound Assignment Operators
     *-------------------------------*/
    e95_token op_assignment_add_t       = {"Compound Assignment (Add)",       math_op};
    e95_token op_assignment_subtract_t  = {"Compound Assignment (Subtract)",  math_op};
    e95_token op_assignment_multiply_t  = {"Compound Assignment (Multiply)",  math_op};
    e95_token op_assignment_divide_t    = {"Compound Assignment (Divide)",    math_op};
    e95_token op_assignment_t           = {"Assignment", general_op};




    /* Reserved Words
     *----------------*/
    e95_token reserved_word_t  = {"Reserved Word", "Reserved Word"};    



    /* Identifiers
     *----------------*/
    e95_token identifier_t     = {"Identifier", "Identifier"};



    /* Separators
     *----------------*/
    e95_token sep_semicolon_t  = {"Semicolon", "Separator"};







    /*-----------------------------------------------------------------+
      | Place tokens in array indexed by token number
      +---------------------------------------------------------------*/
     
    token_def_map[OP_ADDITION]       = op_addition_t;
    token_def_map[OP_SUBTRACTION]    = op_subtraction_t;
    token_def_map[OP_MULTIPLICATION] = op_multiplication_t;
    token_def_map[OP_DIVISION]       = op_division_t;


    token_def_map[OP_ASSIGNMENT_ADD]       = op_assignment_add_t;
    token_def_map[OP_ASSIGNMENT_SUBTRACT]  = op_assignment_subtract_t;
    token_def_map[OP_ASSIGNMENT_MULTIPLY]  = op_assignment_multiply_t;
    token_def_map[OP_ASSIGNMENT_DIVIDE]    = op_assignment_divide_t;


    token_def_map[OP_ASSIGNMENT]     = op_assignment_t;


    token_def_map[IDENTIFIER]     = identifier_t;
    token_def_map[RESERVED_WORD]  = reserved_word_t;


    token_def_map[SEP_SEMICOLON]  = sep_semicolon_t;


}


