/*============================================================================
| File:		symbol_table.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for symbol table implementation.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#define NHASH 200
#define ROOT 1001



/*-------------------------------
| Data Structures
+------------------------------*/
struct basic_type
{   int type;
    int attrs[3];
};


struct symtabl
{   int sid;
    char id[100];
    struct declarator *symtab[NHASH];
    struct symtabl *parent;
    struct symtabl *child;
    struct symtabl *lsibling;
    struct symtabl *rsibling;
};




/*-------------------------------
| Function Prototypes
+------------------------------*/
void basic_types_init(void);
void printrefs(struct symtabl *curr_symtab);
struct declarator *addref(struct declarator *dp, struct symtabl *curr_symtab);
struct declarator *lookup(declarator *sym, struct symtabl *curr_symtab);
void create_symbol_tables(struct ast *parse_tree);
void ast_to_symtab(struct ast *sym, struct symtabl *curr_symtab);
void print_symtab(struct symtabl *curr_symtab);
void global_symtab_init();




/*-------------------------------
| Global Variables
+------------------------------*/
char *currfilename;

struct basic_type basic_types[10];

struct symtabl *curr_symtab;
struct symtabl *global_top_level;
int symtab_sid;
