/*============================================================================
| File:		symbol_table.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for symbol table implementation.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylineno;


#define NHASH 9997

void printrefs(void);
struct symbol *lookup(char *);
void addref(int, char *, char *, int);
struct symbol *lookup(char *sym);


struct symbol
{   char *name;
    struct ref *reflist;
};


struct ref
{   struct ref *next;
    char *filename;
    int flags;
    int lineno;
};


struct symbol symtab[NHASH];


char *currfilename;

