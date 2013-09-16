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

    /* Define all tokens */
    e95_token addition_t         = {"Addition", "Mathematical Operator"};
    e95_token subtraction_t      = {"Subtraction", "Mathematical Operator"};
    e95_token multiplication_t   = {"Multiplication", "Mathematical Operator"};
    e95_token division_t         = {"Division", "Mathematical Operator"};
    e95_token assignment_t       = {"Assignment", "General Operator"};



    /* Place tokens in array indexed by token number */
    token_def_map[ADDITION]       = addition_t;
    token_def_map[SUBTRACTION]    = subtraction_t;
    token_def_map[MULTIPLICATION] = multiplication_t;
    token_def_map[DIVISION]       = division_t;
    token_def_map[ASSIGNMENT]     = assignment_t;
}


