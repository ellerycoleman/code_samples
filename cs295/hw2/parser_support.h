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
		       POINTER_DECLARATOR,
		       SIMPLE_DECLARATOR,
		       FUNCTION_DECLARATOR
		   } ntype;

typedef enum tspec {   SIGNED_SHORT_INT,
		       SIGNED_LONG_INT,
                       SIGNED_INT,
		       SIGNED_CHAR,
		       UNSIGNED_SHORT_INT,
		       UNSIGNED_LONG_INT,
		       UNSIGNED_INT,
		       UNSIGNED_CHAR,
		       VOID
                   } tspec;


typedef struct ast
{   ntype nodetype;
    struct ast *left;
    struct ast *right;
} ast;


typedef struct numval
{   ntype nodetype;
    int val;
} numval;


typedef struct declarator
{ ntype nodetype;
  tspec typespecifier;  /* for parameter_decl's */
  char *id;
  struct declarator *next;
  struct declarator *fdeclarator;
  struct parameter_list *plist;
} declarator;


typedef struct parameter_list
{   declarator *pd;
    struct parameter_list *next;
} parameter_list;


typedef struct funcdef
{   ntype nodetype;
} funcdef;


typedef struct declarator_list
{   declarator *d;
    struct declarator_list *next;
} declarator_list;



typedef struct decl
{   ntype nodetype;
    tspec typespecifier;
    struct declarator_list *dl;
} decl;


typedef struct tld
{   ntype nodetype;
    int datatype; /* 1 is decl, 2 is funcdef */
    struct decl    *d;
    struct funcdef *f;
} tld;


typedef struct tld_list
{   ntype nodetype;
    struct tld *tld;
    struct tld_list *next;
} tld_list;



declarator *new_simple_declarator(char *id);
tld_list *new_tld_list(ast *t, ast *next);
ast *new_tld(int datatype, ast *t);
ast *new_decl(int typespecifier, declarator_list *dl);
void print_tree(ast *nodeptr);
declarator_list *new_declarator_list(declarator *d, declarator_list *next);
declarator_list *reverse_declarator_list(declarator_list *dl);
tld_list *reverse_tld_list(struct tld_list *tl);
declarator *new_pointer_declarator(declarator *next);
declarator *reverse_declarators(declarator *dp);


declarator *new_parameter_decl(int typespec, declarator *d);
parameter_list *new_parameter_list(declarator *pd, parameter_list *next);
declarator *new_function_declarator(declarator *fdecl, parameter_list *plist);
void print_parameter_list(parameter_list *plist);
parameter_list *reverse_parameter_list(parameter_list *plist);


char *print_type(int type);
struct ast *parse_tree;




