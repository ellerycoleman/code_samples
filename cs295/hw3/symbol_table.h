/*============================================================================
| File:		symbol_table.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for symbol table implementation.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#define NHASH 5


void symbol_table_init(void);
void printrefs(void);
void addref(char *filename, int lineno, struct declarator *);
struct declarator *lookup(declarator *sym);

struct basic_type
{   int type;
    int attrs[3];
};
    

struct symtabl
{   char *id;
    struct declarator *symtab[NHASH];
    struct declarator *parent_symtab;
    struct declarator *child_symtab;
};


char *currfilename;

struct basic_type basic_types[10];

struct symtabl *symtab;