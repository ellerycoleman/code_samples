/*============================================================================
| File:		parser_support.h
| Date:		Mon Sep 30 23:00:16 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	Header file for parser support functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


/* interface to lexer */
extern char *yytext;
extern int yylineno;


extern int num_of_tokens_processed;

typedef enum node_type {NODE_OPERATOR,NODE_NUMBER} node_type;

typedef struct node
{   node_type type;
    char *operator;
    struct node *left;
    struct node *right;
    int val;
} node;

node *malloc_op_node(char *operator, node *child_left, node *child_right);
node *malloc_number_node(int val);
void print_tree(node *nodeptr);
char *display_node_type(int i);
int eval(node *nodeptr);
void treefree(node *nodeptr);
