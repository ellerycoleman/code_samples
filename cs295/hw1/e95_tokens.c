/*============================================================================
| File:		e95_tokens.c
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Implements the data structures used for classifying tokens.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*---------------------------------------------------------------------------
 * This file implements a single function: init_token_definition_map().
 *
 * The token definition map is actually an array of type e95_token.  The
 * array is indexed by token names, and those token names and values are
 * defined in the e95_token.h file.
 *
 * The function init_token_definition_map() follows 3 main steps:
 *    1. Define all token types
 *    2. Define all e95_tokens
 *    3. Place all of the e95_tokens in an array using the token name
 *       as defined in e95_token.h as the array index.
 *
 *--------------------------------------------------------------------------*/



#include "e95_tokens.h"

extern e95_token token_def_map[];




void init_token_definition_map()
{   

    /*-----------------------------------------------------------------+
      |  Step 1 of 3:  Define all token types
      +----------------------------------------------------------------*/
    char *math_op       = "Arithmetic Operator";
    char *general_op    = "General Operator";
    char *bitwise_op    = "Bitwise Operator";
    char *relational_op = "Relational Operator";
    char *separator     = "Separator";
    char *logical_op    = "Logical Operator";
    char *bitwise_logical_op= "Bitwise Logical Oper";









    /*-----------------------------------------------------------------+
      |  Step 2 of 3: Define all tokens 
      +---------------------------------------------------------------*/

    /* Basic Arithmetic Operators 
     *----------------------------*/
    e95_token op_addition_t        = {"Addition Operator",       math_op};
    e95_token op_subtraction_t     = {"Subtraction Operator",    math_op};
    e95_token op_multiplication_t  = {"Multiplication Operator", math_op};
    e95_token op_division_t        = {"Division Operator",       math_op};
    e95_token op_remainder_t       = {"Remainder Operator",      math_op};



    /* Bitwise Arithmetic Operators
     *------------------------------*/
    e95_token op_bitwise_and_t      = {"Bitwise AND Operator", math_op};
    e95_token op_bitwise_or_t       = {"Bitwise OR Operator",  math_op};
    e95_token op_bitwise_xor_t      = {"Bitwise XOR Operator", math_op};
    e95_token op_ones_compliment_t  = {"Ones' Compliment Operator", math_op};



    /* Relational Operators
     *----------------------*/
    e95_token op_relational_gt_t    = {"Relational Greater Than",       relational_op};
    e95_token op_relational_gte_t   = {"Relational Greater Than/Equal", relational_op};
    e95_token op_relational_lt_t    = {"Relational Less Than",          relational_op};
    e95_token op_relational_lte_t   = {"Relational Less Than/Equal",    relational_op};
    

  
    /* Compound Assignment Operators
     *-------------------------------*/
    e95_token op_assignment_add_t       = {"Compound Assignment (Add) Oper",       math_op};
    e95_token op_assignment_subtract_t  = {"Compound Assignment (Subtract) Oper",  math_op};
    e95_token op_assignment_multiply_t  = {"Compound Assignment (Multiply) Oper",  math_op};
    e95_token op_assignment_divide_t    = {"Compound Assignment (Divide) Oper",    math_op};



    /* Logical Operators
     *-------------------*/
    e95_token op_logical_and_t = {"Logical AND Operator", logical_op};
    e95_token op_logical_or_t  = {"Logical OR Operator",  logical_op};
    e95_token op_logical_not_t = {"Logical NOT Operator", logical_op};
    e95_token op_logical_not_equal_t = {"Logical NOT EQUAL Operator", logical_op};




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



    /* Undefined
     *------------*/
    e95_token undefined_t  = {"UNDEFINED", "UNDEFINED"};










    /*-----------------------------------------------------------------+
      | Step 3 of 3: Place tokens in array indexed by token number
      +---------------------------------------------------------------*/
     

    /* Basic Arithmetic Operators 
     *----------------------------*/
    token_def_map[OP_ADDITION]       = op_addition_t;
    token_def_map[OP_SUBTRACTION]    = op_subtraction_t;
    token_def_map[OP_MULTIPLICATION] = op_multiplication_t;
    token_def_map[OP_DIVISION]       = op_division_t;
    token_def_map[OP_REMAINDER]      = op_remainder_t;



    /* Bitwise Arithmetic Operators
     *------------------------------*/
    token_def_map[OP_BITWISE_AND]     = op_bitwise_and_t;
    token_def_map[OP_BITWISE_OR]      = op_bitwise_or_t;
    token_def_map[OP_BITWISE_XOR]     = op_bitwise_xor_t;
    token_def_map[OP_ONES_COMPLIMENT] = op_ones_compliment_t;



    /* Relational Operators
     *----------------------*/
    token_def_map[OP_RELATIONAL_GT]  = op_relational_gt_t;
    token_def_map[OP_RELATIONAL_GTE] = op_relational_gte_t;
    token_def_map[OP_RELATIONAL_LT]  = op_relational_lt_t;
    token_def_map[OP_RELATIONAL_LTE] = op_relational_lte_t;



    /* Compound Assignment Operators
     *-------------------------------*/
    token_def_map[OP_ASSIGNMENT_ADD]       = op_assignment_add_t;
    token_def_map[OP_ASSIGNMENT_SUBTRACT]  = op_assignment_subtract_t;
    token_def_map[OP_ASSIGNMENT_MULTIPLY]  = op_assignment_multiply_t;
    token_def_map[OP_ASSIGNMENT_DIVIDE]    = op_assignment_divide_t;



    /* General Assignment Operator
     *-------------------------------*/
    token_def_map[OP_ASSIGNMENT]  = op_assignment_t;



    /* Logical Operators
     *-------------------*/
    token_def_map[OP_LOGICAL_AND]       = op_logical_and_t;
    token_def_map[OP_LOGICAL_OR]        = op_logical_or_t;
    token_def_map[OP_LOGICAL_NOT]       = op_logical_not_t;
    token_def_map[OP_LOGICAL_NOT_EQUAL] = op_logical_not_equal_t;



    /* Reserved Words
     *----------------*/
    token_def_map[RESERVED_WORD] = reserved_word_t;



    /* Identifiers
     *----------------*/
    token_def_map[IDENTIFIER]    = identifier_t;



    /* Integer Constants
     *-------------------*/
    token_def_map[INT_CONSTANT]  = int_constant_t;



    /* Separators
     *----------------*/
    token_def_map[SEP_SEMICOLON] = sep_semicolon_t;



    /* End of line character
     *-----------------------*/
    token_def_map[END_OF_LINE]   = end_of_line_t;



    /* Undefined
     *------------*/
    token_def_map[UNDEFINED]     = undefined_t;
}


