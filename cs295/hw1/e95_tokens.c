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

    /* Define all token types */
    char *math_op= "Mathematical Operator";
    char *general_op= "General Operator";


    /*-----------------------------------------------+
      |              Define all tokens 
      +----------------------------------------------*/


    /* Mathematical Operators */
    e95_token addition_t         = {"Addition",        math_op};
    e95_token subtraction_t      = {"Subtraction",     math_op};
    e95_token multiplication_t   = {"Multiplication",  math_op};
    e95_token division_t         = {"Division",        math_op};

    e95_token assignment_t       = {"Assignment",      general_op};




    /* Reserved Word */
    e95_token reserved_word_t    = {"Reserved Word", "Reserved Word"};    



    /* Identifier    */
    e95_token identifier_t       = {"Identifier", "Identifier"};








    /* Place tokens in array indexed by token number */
    token_def_map[ADDITION]       = addition_t;
    token_def_map[SUBTRACTION]    = subtraction_t;
    token_def_map[MULTIPLICATION] = multiplication_t;
    token_def_map[DIVISION]       = division_t;
    token_def_map[ASSIGNMENT]     = assignment_t;
    token_def_map[IDENTIFIER]     = identifier_t;
    token_def_map[RESERVED_WORD]  = reserved_word_t;
}


