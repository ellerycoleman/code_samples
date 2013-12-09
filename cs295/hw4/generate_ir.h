/*============================================================================
| File:		generate_ir.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for IR generation subsystem.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/




/*-------------------------------
| Data Structures
+------------------------------*/
enum ircodes 
{   BEGINPROC=100,
    LOADADDRESS,
    STOREWORDINDIRECT,
    ENDPROC
};


struct irnode
{   enum ircodes ircode;
    struct irnode *prev;
    struct irnode *next;
    struct declarator *symptr;
    int sid;
};





/*-------------------------------
| Function Prototypes
+------------------------------*/
void generate_ir(struct ast *parse_tree);
void print_irnodes(void);





/*-------------------------------
| Global Variables
+------------------------------*/
FILE *irout;
char irfname[100];
struct irnode *irlist_front;
int irnodenum;


