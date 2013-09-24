/*============================================================================
| File:		calc_tokens.c
| Date:		Tue Sep 24 15:12:01 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Implements the data structures used for classifying tokens.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*---------------------------------------------------------------------------
 * This file implements a single function: init_token_definition_map().
 *
 * The token definition map is actually an array of type calc_token.  The
 * array is indexed by token names, and those token names and values are
 * defined in the calc_token.h file.
 *
 * The function init_token_definition_map() follows 3 main steps:
 *    1. Define common token types
 *    2. Define all calc_tokens
 *    3. Place all of the calc_tokens in an array using the token name
 *       as defined in calc_token.h as the array index.
 *
 *--------------------------------------------------------------------------*/



#include "calc_tokens.h"

extern calc_token token_def_map[];




void init_token_definition_map()
{   

    /*-----------------------------------------------------------------+
      |  Step 1 of 3:  Define common token types
      +----------------------------------------------------------------*/
    char *math_op           = "Arithmetic Operator";
    char *standalone_char   = "Standalone Character";







    /*-----------------------------------------------------------------+
      |  Step 2 of 3: Define all tokens 
      +---------------------------------------------------------------*/

    /* Basic Arithmetic Operators 
     *----------------------------*/
    calc_token op_addition_t        = {"Addition Operator",       math_op};
    calc_token op_subtraction_t     = {"Subtraction Operator",    math_op};
    calc_token op_multiplication_t  = {"Multiplication Operator", math_op};
    calc_token op_division_t        = {"Division Operator",       math_op};
    calc_token op_absvalue_t        = {"Absolute Value Operator", math_op};



    /* Constants
     *----------------*/
    calc_token integer_constant_t         = {"Integer Constant", "Constant"};




    /* End of line character
     *-----------------------*/
    calc_token end_of_line_t  = {"NEWLINE", "End Of Line Char"};



    /* Undefined
     *------------*/
    calc_token undefined_t  = {"UNDEFINED", "UNDEFINED"};










    /*-----------------------------------------------------------------+
      | Step 3 of 3: Place tokens in array indexed by token number
      +---------------------------------------------------------------*/
     

    token_def_map[OP_ADDITION]       = op_addition_t;
    token_def_map[OP_SUBTRACTION]    = op_subtraction_t;
    token_def_map[OP_MULTIPLICATION] = op_multiplication_t;
    token_def_map[OP_DIVISION]       = op_division_t;
    token_def_map[OP_ABSVALUE]       = op_absvalue_t;


    token_def_map[INTEGER_CONSTANT]  = integer_constant_t;

    token_def_map[END_OF_LINE]  = end_of_line_t;

    token_def_map[UNDEFINED]    = undefined_t;
}


