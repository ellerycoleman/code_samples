/*============================================================================
| File:		symbol_table.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for symbol table implementation.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#define NHASH 20


void symbol_table_init(void);
void printrefs(void);
void addref(char *filename, int lineno, struct declarator *);
struct declarator *lookup(declarator *sym);


struct symbol
{   char *name;
    struct ref *reflist;
};


struct basic_type
{   int type;
    int attrs[3];
};
    

struct declarator *symtab[NHASH];


char *currfilename;



