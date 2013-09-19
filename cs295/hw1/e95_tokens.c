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
      |  Step 1 of 3:  Define all token types
      +----------------------------------------------------------------*/
    char *math_op= "Arithmetic Operator";
    char *general_op= "General Operator";
    char *bitwise_op= "Bitwise Operator";
    char *separator= "Separator";






    /*-----------------------------------------------------------------+
      |  Step 2 of 3: Define all tokens 
      +---------------------------------------------------------------*/

    /* Basic Arithmetic Operators 
     *----------------------------*/
    e95_token op_addition_t        = {"Addition Operator",        math_op};
    e95_token op_subtraction_t     = {"Subtraction Operator",     math_op};
    e95_token op_multiplication_t  = {"Multiplication Operator",  math_op};
    e95_token op_division_t        = {"Division Operator",        math_op};



    /* Compound Assignment Operators
     *-------------------------------*/
    e95_token op_assignment_add_t       = {"Compound Assignment (Add) Oper",       math_op};
    e95_token op_assignment_subtract_t  = {"Compound Assignment (Subtract) Oper",  math_op};
    e95_token op_assignment_multiply_t  = {"Compound Assignment (Multiply) Oper",  math_op};
    e95_token op_assignment_divide_t    = {"Compound Assignment (Divide) Oper",    math_op};



    /* General Assignment Operator
     *-------------------------------*/
    e95_token op_assignment_t  = {"Assignment Operator", general_op};



    /* Reserved Words
     *----------------*/
    e95_token reserved_word_t  = {"Reserved Word", "Reserved Word"};    



    /* Identifiers
     *----------------*/
    e95_token identifier_t     = {"Identifier", "Identifier"};


    /* Constants
     *----------------*/
    e95_token int_constant_t   = {"Integer Constant", "Constant"};


    /* Separators
     *----------------*/
    e95_token sep_semicolon_t  = {"Semicolon", "Separator"};



    /* End of line character
     *-----------------------*/
    e95_token end_of_line_t  = {"NEWLINE", "End Of Line Char"};









    /*-----------------------------------------------------------------+
      | Step 3 of 3: Place tokens in array indexed by token number
      +---------------------------------------------------------------*/
     

    /* Basic Arithmetic Operators 
     *----------------------------*/
    token_def_map[OP_ADDITION]       = op_addition_t;
    token_def_map[OP_SUBTRACTION]    = op_subtraction_t;
    token_def_map[OP_MULTIPLICATION] = op_multiplication_t;
    token_def_map[OP_DIVISION]       = op_division_t;



    /* Compound Assignment Operators
     *-------------------------------*/
    token_def_map[OP_ASSIGNMENT_ADD]       = op_assignment_add_t;
    token_def_map[OP_ASSIGNMENT_SUBTRACT]  = op_assignment_subtract_t;
    token_def_map[OP_ASSIGNMENT_MULTIPLY]  = op_assignment_multiply_t;
    token_def_map[OP_ASSIGNMENT_DIVIDE]    = op_assignment_divide_t;



    /* General Assignment Operator
     *-------------------------------*/
    token_def_map[OP_ASSIGNMENT]     = op_assignment_t;



    /* Reserved Words
     *----------------*/
    token_def_map[RESERVED_WORD]  = reserved_word_t;



    /* Identifiers
     *----------------*/
    token_def_map[IDENTIFIER]     = identifier_t;


    /* Identifiers
     *----------------*/
    token_def_map[INT_CONSTANT]   = int_constant_t;


    /* Separators
     *----------------*/
    token_def_map[SEP_SEMICOLON]  = sep_semicolon_t;


    /* End of line character
     *-----------------------*/
    token_def_map[END_OF_LINE]    = end_of_line_t;
}


