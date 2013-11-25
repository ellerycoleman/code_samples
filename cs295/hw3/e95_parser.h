/*============================================================================
| File:		e95_parser.h
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Declarations for user defined functions for parser.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



extern char *yytext;
extern int num_of_tokens_processed;

typedef enum ntype {   DECL,
                       DECLARATOR,
		       TLD,
		       TLD_LIST,
		       FUNCDEF,
		       DIRECT_DECLARATOR,
		       DECLARATOR_LIST,
		       POINTER_DECLARATOR,
		       SIMPLE_DECLARATOR,
		       FUNCTION_DECLARATOR,
		       DAD_PAREN_SINGLE_ARG,
		       ARRAY_DECLARATOR,
/* ^11 */

		       STATEMENT,
		       EXPRESSION_STATEMENT,
		       COMPOUND_STATEMENT,
		       CONDITIONAL_STATEMENT,
		       ITERATIVE_STATEMENT,
		       BREAK_STATEMENT,
		       CONTINUE_STATEMENT,
		       RETURN_STATEMENT,
		       GOTO_STATEMENT,
		       NULL_STATEMENT,
		       IF_STATEMENT,
		       IF_ELSE_STATEMENT,
		       WHILE_STATEMENT,
		       DO_STATEMENT,
		       FOR_STATEMENT,
		       LABELED_STATEMENT,
		       LABEL,
/* ^28 */

		       COMMA_EXPR,
		       CAST_EXPR,
		       UNARY_MINUS_EXPR,
		       UNARY_PLUS_EXPR,
		       PREDECREMENT_EXPR,
		       POSTDECREMENT_EXPR,
		       PREINCREMENT_EXPR,
		       POSTINCREMENT_EXPR,
		       INDIRECTION_EXPR,
		       ADDRESS_EXPR,
		       BITWISE_NEGATION_EXPR,
		       LOGICAL_NEGATION_EXPR,
		       PARENTHESIZED_EXPR,
		       SUBSCRIPT_EXPR,
		       CONDITIONAL_EXPR,
		       LOGICAL_OR_EXPR,
		       LOGICAL_AND_EXPR,
		       BITWISE_OR_EXPR,
		       BITWISE_XOR_EXPR,
		       BITWISE_AND_EXPR,
/* ^48 */



		       FUNCTION_DEFINITION,
		       FUNCTION_DEF_SPECIFIER,
		       FUNCTION_CALL,
		       DECOSTAT_LIST,
		       COMMA_LIST,
		       DIRECT_ABSTRACT_DECLARATOR

		   } ntype;



typedef enum tspec {   SIGNED_SHORT_INT,
		       SIGNED_LONG_INT,
                       SIGNED_INT,
		       SIGNED_CHAR,
		       UNSIGNED_SHORT_INT,
		       UNSIGNED_LONG_INT,
		       UNSIGNED_INT,
		       UNSIGNED_CHAR,
		       VOID,
		       OTHER     /* needed in one case of direct abstract declarators and one case of parameter_decl */
                   } tspec;


enum type_attrs {INTEGRAL_T,ARITHMETIC_T,SCALAR_T,VOID_T};


typedef enum dadtype {PAREN_ENCLOSED=900,BRACKET_NO_EXPR,BRACKET_EXPR,DAD_LIST} dadtype;


struct ast
{   ntype nodetype;
    struct ast *l;
    struct ast *r;
};


struct numval
{   ntype nodetype;
    int val;
};


struct constant
{   ntype nodetype;
    void *value;
};


struct expr
{   ntype nodetype;
    struct ast *l;
    struct ast *r;
};


/* All declarators are handled with a single declarator struct */
typedef struct declarator
{   ntype nodetype;               /* signifies what type of declarator this is  */
    struct basic_type *tspecptr;  /* typespec for parameter_decl's              */
    dadtype dadtype;          /* dadtype for direcect abstract declarators  */
    char *id;
    struct declarator *next;
    struct declarator *adeclarator; /* for use with function/abstract declarators */
    struct expr *exp;
    struct parameter_list *plist;
    int funcdef_true;   /* set by symbol table generator */
} declarator;



typedef struct parameter_list
{   declarator *pd;
    struct parameter_list *next;
} parameter_list;


typedef struct declarator_list
{   ntype nodetype;
    declarator *d;
    struct declarator_list *next;
} declarator_list;



struct decl
{   ntype nodetype;
    struct basic_type *tspecptr;
    struct declarator_list *dl;
};


struct tld
{   ntype nodetype;
    int datatype; /* 1 is decl, 2 is funcdef */
    struct decl    *d;
    struct ast *f;
};


struct tld_list
{   ntype nodetype;
    struct tld *tld;
    struct tld_list *next;
};


struct decostat_list
{   ntype nodetype;
    struct ast *decostat;
    struct decostat_list *next;
};


struct function_defspec
{   ntype nodetype;
    int typespec;
    struct declarator *d;
};


struct function_def
{   ntype nodetype;
    struct function_defspec *fdspec;
    struct ast *cstmt; /* decostat_list */
};


struct flow
{   ntype nodetype;
    struct ast *cond;
    struct ast *thendo;
    struct ast *elsedo;
    struct ast *forinit;
    struct ast *forupdate;
};


struct cond_expr
{   ntype nodetype;
    struct ast *cond;
    struct ast *return1;
    struct ast *return2;
};


declarator *new_simple_declarator(char *id);
struct tld_list *new_tld_list(struct ast *t, struct ast *next);
struct ast *new_tld(int datatype, struct ast *t);
struct ast *new_decl(int typespecifier, declarator_list *dl);
void print_tree(struct ast *nodeptr);
declarator_list *new_declarator_list(declarator *d, declarator_list *next);
declarator_list *reverse_declarator_list(declarator_list *dl);
struct tld_list *reverse_tld_list(struct tld_list *tl);
declarator *new_pointer_declarator(declarator *next);
declarator *reverse_declarators(declarator *dp);


declarator *new_parameter_decl(int typespec, declarator *d);
parameter_list *new_parameter_list(declarator *pd, parameter_list *next);
declarator *new_function_declarator(declarator *fdecl, parameter_list *plist);
char * print_parameter_list(parameter_list *plist, char *plistr);
parameter_list *reverse_parameter_list(parameter_list *plist);

struct ast *new_constant(int type, void *value);
struct ast *new_expr(int type,struct ast *l, struct ast *r);


char *print_type(int type);
struct ast *parse_tree;
int debug_counter;


struct ast *new_function_def_specifier(int type, struct declarator *d);
struct ast *new_function_definition(struct ast *fdefspec, struct ast *compound_stmt);

struct ast *new_compound_statement(struct ast *decstmtlist);
struct ast *new_decostat_list(struct ast *decostat, struct ast *next);
struct decostat_list *reverse_decostat_list(struct decostat_list *dlist);

char * print_expr(struct ast *expr,char *exprstr);
char * print_decl(struct ast *expr,char *declstr);



declarator *new_direct_abstract_declarator(int type, struct ast *data, struct declarator *next);
declarator *new_array_declarator(int type, struct declarator *arrydec, struct ast *expr);
char * print_dad(declarator *d,char *dadstr);
struct ast *new_flow(struct ast *cond, struct ast *thendo, struct ast *els);
struct ast *new_conditional_expr(struct ast *cond, struct ast *return1, struct ast *return2);
struct ast *new_if_statement(struct ast *cond, struct ast *thendo, struct ast *elsedo);
struct ast *new_while_statement(struct ast *cond, struct ast *thendo);
struct ast *new_do_statement(struct ast *cond, struct ast *thendo);

struct ast *new_for_statement(struct ast *forinit,
                              struct ast *cond,
                              struct ast *forupdate,
                              struct ast *thendo
                             );

void * emalloc(int size);
char * funcdef_to_string(struct function_def *funcdef,char fdef[]);
char * funcdecl_to_string(struct declarator *fdecl,char *fdef);
void clearstr(char *str);



#include "symbol_table.h"



