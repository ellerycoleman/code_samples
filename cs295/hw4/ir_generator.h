/*============================================================================
| File:		generate_ir.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for IR generation subsystem.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#ifndef _GENERATE_IR_H_
#define _GENERATE_IR_H_ 1


/*-------------------------------
| Data Structures
+------------------------------*/
enum ircodes 
{   BEGINPROC=100,
    LOADADDRESS,
    LOADCONSTANT,
    STOREWORDINDIRECT,
    LOADWORDINDIRECT,
    ENDPROC,
    PRINTINT,
    SYSCALL,
    LOADWORD,
    MIPSLABEL,
    BRANCH_GT,
    BRANCH_LT,
    JUMP,
    COMMENT,
    ADD,
    ADD1,
    PRINTSTRING,
    CREATE_STRINGVAR
};


char *ircodenames[200];

struct irnode
{   
    int sid;
    enum ircodes ircode;
    int oprnd1;
    int oprnd2;
    int oprnd3;
    struct irnode *prev;
    struct irnode *next;
    struct declarator *symptr;
    char label[50];
};


#define LVALUE 0
#define RVALUE 1


struct irinfo
{  int nodetype; 
   int regnum;
};    
    







/*-------------------------------
| Function Prototypes
+------------------------------*/
void generate_ir(struct ast *parse_tree);
void print_irnodes(void);
char *print_declarator_id(struct declarator *sym);
void compound_to_ir(struct ast *cstmt);
void decostat_to_ir(struct ast *decostat);
struct irinfo *expr_to_ir(struct ast *subtree);
void print_irnode_sids(int begin_sid,int end_sid);





/*-------------------------------
| Global Variables
+------------------------------*/
FILE *irout;
char irfname[100];
int irnodenum;
struct irnode *irlist_front;
struct irnode *irlist;
int regnum;
int labelnum;


#endif
