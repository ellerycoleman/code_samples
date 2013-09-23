/*============================================================================
| File:		e95_tokens.h
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Token definitions for hw1 scanner.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



#ifndef _E95_TOKENS_H
#define _E95_TOKENS_H  1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_TOKEN_DEFS 500



/* Simple Operators   */
enum e95_tokens
{   

    OP_ADDITION = 258,
    OP_DIVISION,
    OP_REMAINDER,


    OP_BITWISE_OR,
    OP_BITWISE_XOR,
    OP_ONES_COMPLIMENT,
    OP_LEFT_BITSHIFT,
    OP_RIGHT_BITSHIFT,


    OP_RELATIONAL_GTE,
    OP_RELATIONAL_LT,
    OP_RELATIONAL_LTE,


    OP_ASSIGNMENT_ADD,
    OP_ASSIGNMENT_SUBTRACT,
    OP_ASSIGNMENT_MULTIPLY,
    OP_ASSIGNMENT_DIVIDE,
    OP_ASSIGNMENT_REMAINDER,
    OP_ASSIGNMENT_LEFT_BITSHIFT,
    OP_ASSIGNMENT_RIGHT_BITSHIFT,
    OP_ASSIGNMENT_BITWISE_AND,
    OP_ASSIGNMENT_BITWISE_OR,
    OP_ASSIGNMENT_BITWISE_XOR,


    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_LOGICAL_NOT,


    OP_EQUALITY,
    OP_NON_EQUALITY,


    OP_INCREMENT,
    OP_DECREMENT,


    OP_ASSIGNMENT,
    OP_ASTERISK,
    OP_QUESTION_MARK,


    SEP_LEFT_PAREN,
    SEP_RIGHT_PAREN,
    SEP_LEFT_CURLY_BRACE,
    SEP_RIGHT_CURLY_BRACE,
    SEP_LEFT_SQUARE_BRACKET,
    SEP_RIGHT_SQUARE_BRACKET,
    SEP_SEMICOLON,
    SEP_COLON,
    SEP_COMMA,


    RESERVED_WORD_DO,
    RESERVED_WORD_FOR,
    RESERVED_WORD_RETURN,
    RESERVED_WORD_BREAK,
    RESERVED_WORD_SHORT,
    RESERVED_WORD_ELSE,
    RESERVED_WORD_GOTO,
    RESERVED_WORD_SIGNED,
    RESERVED_WORD_UNSIGNED,
    RESERVED_WORD_CHAR,
    RESERVED_WORD_IF,
    RESERVED_WORD_VOID,
    RESERVED_WORD_INT,
    RESERVED_WORD_CONTINUE,
    RESERVED_WORD_LONG,
    RESERVED_WORD_WHILE,




    IDENTIFIER,
    INTEGER_CONSTANT,
    CHARACTER_CONSTANT,
    CHARACTER_CONSTANT_OCTAL,
    STRING_CONSTANT,


    APOSTROPHE,
    DOUBLEQUOTE,
    BACKSLASH,
    BACKTICK,
    PERIOD,
    AMPERSAND,
    DOLLAR_SIGN,
    AT_SIGN,
    NUMBER_SIGN,
    MINUS_SIGN,
    GREATER_THAN_SYMBOL,

    END_OF_LINE,
    UNDEFINED
};



/* creating token structure */
typedef struct e95_token 
{   char *name;
    char *type;
} e95_token;



/* defining a token map to hold all token definitions */
e95_token token_def_map[MAX_TOKEN_DEFS];
int cvalue;


void init_token_definition_map(void);


#endif
