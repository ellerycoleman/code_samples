/*============================================================================
| File:		e95_tokens.c
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Implements data structures for classifying tokens.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

#include "e95_tokens.h"


void init_token_typemap()
{   token_type= malloc(sizeof(char *) * MAX_TOKENS);

    char *arithmetic_op_= "Arithmetic Operator";

    token_type[ADDITION]= arithmetic_op_;
    token_type[SUBTRACTION]= arithmetic_op_;
    token_type[MULTIPLICATION]= arithmetic_op_;
    token_type[DIVISION]= arithmetic_op_;
    token_type[ASSIGNMENT]= arithmetic_op_;
}
