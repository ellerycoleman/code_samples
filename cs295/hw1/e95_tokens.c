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
 *    1. Define common token types
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
      |  Step 1 of 3:  Define common token types
      +----------------------------------------------------------------*/
    char *math_op           = "Arithmetic Operator";
    char *bitwise_op        = "Bitwise Operator";
    char *relational_op     = "Relational Operator";
    char *logical_op        = "Logical Operator";
    char *ambiguous_op      = "Ambiguous Operator";
    char *equality_op       = "Equality Operator";
    char *separator         = "Separator";
    char *standalone_char   = "Standalone Character";







    /*-----------------------------------------------------------------+
      |  Step 2 of 3: Define all tokens 
      +---------------------------------------------------------------*/

    /* Basic Arithmetic Operators 
     *----------------------------*/
    e95_token op_addition_t     = {"Addition Operator",   math_op};
    e95_token op_division_t     = {"Division Operator",   math_op};
    e95_token op_remainder_t    = {"Remainder Operator",  math_op};



    /* Bitwise Arithmetic Operators
     *------------------------------*/
    e95_token op_bitwise_or_t       = {"Bitwise OR Operator",       bitwise_op};
    e95_token op_bitwise_xor_t      = {"Bitwise XOR Operator",      bitwise_op};
    e95_token op_ones_compliment_t  = {"Ones' Compliment Operator", bitwise_op};
    e95_token op_left_bitshift_t    = {"Left Bitshift Operator",    bitwise_op};
    e95_token op_right_bitshift_t   = {"Right Bitshift Operator",   bitwise_op};



    /* Relational Operators
     *----------------------*/
    e95_token op_relational_gte_t   = {"Greater Than/Equal To Operator", relational_op};
    e95_token op_relational_lt_t    = {"Less Than Operator",             relational_op};
    e95_token op_relational_lte_t   = {"Less Than/Equal To Operator",    relational_op};
    

  
    /* Compound Assignment Operators
     *-------------------------------*/
    e95_token op_assignment_add_t            = {"Compound Assignment (Add) Oper",       math_op};
    e95_token op_assignment_subtract_t       = {"Compound Assignment (Subtract) Oper",  math_op};
    e95_token op_assignment_multiply_t       = {"Compound Assignment (Multiply) Oper",  math_op};
    e95_token op_assignment_divide_t         = {"Compound Assignment (Divide) Oper",    math_op};
    e95_token op_assignment_remainder_t      = {"Compound Assignment (Remainder) Oper", math_op};
    e95_token op_assignment_left_bitshift_t  = {"Compound Assignment Left Bitshift Op", bitwise_op};
    e95_token op_assignment_right_bitshift_t = {"Compound Assignment Right Bitshift Op",bitwise_op};
    e95_token op_assignment_bitwise_and_t    = {"Compound Assignment Bitwise AND",      bitwise_op};
    e95_token op_assignment_bitwise_or_t     = {"Compound Assignment Bitwise OR",       bitwise_op};
    e95_token op_assignment_bitwise_xor_t    = {"Compound Assignment Bitwise XOR",      bitwise_op};



    /* Logical Operators
     *-------------------*/
    e95_token op_logical_and_t = {"Logical AND Operator", logical_op};
    e95_token op_logical_or_t  = {"Logical OR Operator",  logical_op};
    e95_token op_logical_not_t = {"Logical NOT Operator", logical_op};



    /* Equality Operators
     *--------------------*/
    e95_token op_equality_t     = {"Equality Operator", equality_op};
    e95_token op_non_equality_t = {"Non Equality Operator", equality_op};
   


    /* Increment/Decrement Operators
     *-------------------------------*/
    e95_token op_increment_t  = {"Increment Operator", math_op};
    e95_token op_decrement_t  = {"Decrement Operator", math_op};


    e95_token op_assignment_t    = {"Assignment Operator", "General Operator"};
    e95_token op_asterisk_t      = {"Asterisk", "N/A"};
    e95_token op_question_mark_t = {"Question mark", "Ternary Operator"};



    /* Separators
     *----------------*/
    e95_token sep_left_paren_t            = {"Left Paren", separator};
    e95_token sep_right_paren_t           = {"Right Paren", separator};
    e95_token sep_left_curly_brace_t      = {"Left Curly Brace", separator};
    e95_token sep_right_curly_brace_t     = {"Right Curly Brace", separator};
    e95_token sep_left_square_bracket_t   = {"Left Square Bracket", separator};
    e95_token sep_right_square_bracket_t  = {"Right Square Bracket", separator};
    e95_token sep_semicolon_t             = {"Semicolon", separator};
    e95_token sep_colon_t                 = {"Colon", separator};
    e95_token sep_comma_t                 = {"Comma", separator};



    /* Reserved Words
     *----------------*/
    e95_token reserved_word_t  = {"Reserved Word", "Reserved Word"};    



    /* Identifiers
     *----------------*/
    e95_token identifier_t     = {"Identifier", "Identifier"};



    /* Constants
     *----------------*/
    e95_token integer_constant_t   = {"Integer Constant", "Constant"};
    e95_token character_constant_t = {"Character Constant", "Constant"};
    e95_token string_constant_t    = {"String Constant", "Constant"};



    /* Potential stand-alone characters
     *----------------------------------*/
    e95_token char_apostrophe_t          = {"Apostrophe",   standalone_char};
    e95_token char_doublequote_t         = {"Double Quote", standalone_char};
    e95_token char_backslash_t           = {"Backslash",    standalone_char};
    e95_token char_backtick_t            = {"Backtick",     standalone_char};
    e95_token char_period_t              = {"Period",       standalone_char};
    e95_token char_ampersand_t           = {"Ampersand",    standalone_char};
    e95_token char_minus_sign_t          = {"Minus Sign",   standalone_char};
    e95_token char_dollar_sign_t         = {"Dollar Sign",  standalone_char};
    e95_token char_at_sign_t             = {"At Sign",      standalone_char};
    e95_token char_number_sign_t         = {"Number Sign",  standalone_char};
    e95_token char_greater_than_symbol_t = {"Greater Than Symbol", standalone_char};



    /* End of line character
     *-----------------------*/
    e95_token end_of_line_t  = {"NEWLINE", "End Of Line Char"};



    /* Undefined
     *------------*/
    e95_token undefined_t  = {"UNDEFINED", "UNDEFINED"};










    /*-----------------------------------------------------------------+
      | Step 3 of 3: Place tokens in array indexed by token number
      +---------------------------------------------------------------*/
     

    token_def_map[OP_ADDITION]   = op_addition_t;
    token_def_map[OP_DIVISION]   = op_division_t;
    token_def_map[OP_REMAINDER]  = op_remainder_t;


    token_def_map[OP_BITWISE_OR]      = op_bitwise_or_t;
    token_def_map[OP_BITWISE_XOR]     = op_bitwise_xor_t;
    token_def_map[OP_ONES_COMPLIMENT] = op_ones_compliment_t;
    token_def_map[OP_LEFT_BITSHIFT]   = op_left_bitshift_t;
    token_def_map[OP_RIGHT_BITSHIFT]  = op_right_bitshift_t;


    token_def_map[OP_RELATIONAL_GTE] = op_relational_gte_t;
    token_def_map[OP_RELATIONAL_LT]  = op_relational_lt_t;
    token_def_map[OP_RELATIONAL_LTE] = op_relational_lte_t;


    token_def_map[OP_ASSIGNMENT_ADD]            = op_assignment_add_t;
    token_def_map[OP_ASSIGNMENT_SUBTRACT]       = op_assignment_subtract_t;
    token_def_map[OP_ASSIGNMENT_MULTIPLY]       = op_assignment_multiply_t;
    token_def_map[OP_ASSIGNMENT_DIVIDE]         = op_assignment_divide_t;
    token_def_map[OP_ASSIGNMENT_REMAINDER]      = op_assignment_remainder_t;
    token_def_map[OP_ASSIGNMENT_LEFT_BITSHIFT]  = op_assignment_left_bitshift_t;
    token_def_map[OP_ASSIGNMENT_RIGHT_BITSHIFT] = op_assignment_right_bitshift_t;
    token_def_map[OP_ASSIGNMENT_BITWISE_AND]    = op_assignment_bitwise_and_t;
    token_def_map[OP_ASSIGNMENT_BITWISE_OR]     = op_assignment_bitwise_or_t;
    token_def_map[OP_ASSIGNMENT_BITWISE_XOR]    = op_assignment_bitwise_xor_t;


    token_def_map[OP_LOGICAL_AND]   = op_logical_and_t;
    token_def_map[OP_LOGICAL_OR]    = op_logical_or_t;
    token_def_map[OP_LOGICAL_NOT]   = op_logical_not_t;


    token_def_map[OP_EQUALITY]      = op_equality_t;
    token_def_map[OP_NON_EQUALITY]  = op_non_equality_t;


    token_def_map[OP_INCREMENT]     = op_increment_t;
    token_def_map[OP_DECREMENT]     = op_decrement_t;


    token_def_map[OP_ASSIGNMENT]    = op_assignment_t;
    token_def_map[OP_ASTERISK]      = op_asterisk_t;
    token_def_map[OP_QUESTION_MARK] = op_question_mark_t;


    token_def_map[SEP_LEFT_PAREN]           = sep_left_paren_t;
    token_def_map[SEP_RIGHT_PAREN]          = sep_right_paren_t;
    token_def_map[SEP_LEFT_CURLY_BRACE]     = sep_left_curly_brace_t;
    token_def_map[SEP_RIGHT_CURLY_BRACE]    = sep_right_curly_brace_t;
    token_def_map[SEP_LEFT_SQUARE_BRACKET]  = sep_left_square_bracket_t;
    token_def_map[SEP_RIGHT_SQUARE_BRACKET] = sep_right_square_bracket_t;
    token_def_map[SEP_SEMICOLON]            = sep_semicolon_t;
    token_def_map[SEP_COLON]                = sep_colon_t;
    token_def_map[SEP_COMMA]                = sep_comma_t;


    token_def_map[RESERVED_WORD]       = reserved_word_t;
    token_def_map[IDENTIFIER]          = identifier_t;
    token_def_map[INTEGER_CONSTANT]    = integer_constant_t;
    token_def_map[CHARACTER_CONSTANT]  = character_constant_t;
    token_def_map[STRING_CONSTANT]     = string_constant_t;


    token_def_map[APOSTROPHE]          = char_apostrophe_t;
    token_def_map[DOUBLEQUOTE]         = char_doublequote_t;
    token_def_map[BACKSLASH]           = char_backslash_t;
    token_def_map[PERIOD]              = char_period_t;
    token_def_map[AMPERSAND]           = char_ampersand_t;
    token_def_map[BACKTICK]            = char_backtick_t;
    token_def_map[MINUS_SIGN]          = char_minus_sign_t;
    token_def_map[DOLLAR_SIGN]         = char_dollar_sign_t;
    token_def_map[AT_SIGN]             = char_at_sign_t;
    token_def_map[NUMBER_SIGN]         = char_number_sign_t;
    token_def_map[GREATER_THAN_SYMBOL] = char_greater_than_symbol_t;


    token_def_map[END_OF_LINE]  = end_of_line_t;
    token_def_map[UNDEFINED]    = undefined_t;
}


