/*============================================================================
| File:		symbol_table.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements symbol table functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

extern struct basic_type basic_types[];
extern struct symtabl *curr_symtab;
extern struct symtabl *global_top_level;
extern int symtab_sid;


/*-----------------------------------------------
 | create_symbol_tables
 +---------------------------------------------*/
void create_symbol_tables(struct ast *parse_tree)
{
    struct decl *tdecl;
    struct function_def *funcdef;
    int i=1;
    int tldloop=0;
    struct declarator *d;
    char symtab_name[100];


    /* Initialize basic_types data structure
    +-----------------------------------------*/
    basic_types_init();


    /* Initialize global symbol table
    +----------------------------------*/
    global_symtab_init();


    /* create_symbol_table() accepts the root of parse tree as its param.
    |  The root of the parse tree is a *tld_list so we'll cast the param
    |  into a *tld_list.
    +--------------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)parse_tree;



    /* A tld_list has a series of tld nodes; each node pointing to either
    |  a decl or a funcdef.  We'll create and populate symtabs as We cycle
    |  through all of the TLD's in a do-while loop.
    +---------------------------------------------------------------------*/
    do
    {   curr_symtab= global_top_level; /* always start with the global symtab */

        printf("DEBUG create_symbol_tables(): TLD while loop iter: %d...\n", ++tldloop);

        /*--------------------------------------------+
        |            if TLD is a DECL
        +---------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;
            printf("DEBUG create_symbol_tables(): TLD is a decl...\n");

            /* Add decl to symbol table */
            ast_to_symtab((struct ast *)tdecl,curr_symtab);
	}



        /*--------------------------------------------+
        |      if TLD is a FUNCTION_DEFINITION
        +---------------------------------------------*/
        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {
	    /* retrieve function definition */
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;

	    /* add funcdef to symtab */
	    funcdef_to_symtab(funcdef);
	}

    }while( (tldlist= tldlist->next) != NULL );
}




/*-----------------------------------------------
 | ast_to_symtab
 +---------------------------------------------*/
void ast_to_symtab(struct ast *sym, struct symtabl *curr_symtab)
{   struct declarator_list *tmpdl;
    struct declarator *dp;
    struct decl *tdecl;
    struct function_defspec *fdspec;


    printf("DEBUG: ast_to_symtab(): invoked\n");
    if(sym->nodetype == DECL)
    {   tdecl= (struct decl *)sym;
        printf("DEBUG: ast_to_symtab(): ast type DECL\n");
        tmpdl= tdecl->dl;
        do
        {   dp= tmpdl->d;
            addref(dp,curr_symtab);
        }while(tmpdl= tmpdl->next);
    }


    else if(sym->nodetype == FUNCTION_DEF_SPECIFIER)
    {   fdspec= (struct function_defspec *)sym;
        printf("DEBUG: ast_to_symtab(): ast type FUNCTION_DEF_SPECIFIER\n");
	dp= fdspec->d;
	addref(dp,curr_symtab);
    }



    else
    {
        printf("DEBUG: ast_to_symtab(): ast type %d\n", sym->nodetype);
    }
}




/*-----------------------------------------------
 | symbol_table_init
 +---------------------------------------------*/
void basic_types_init(void)
{   int i;


    /* Initialize basic types */


    /* type 0 */
    basic_types[SIGNED_SHORT_INT].type     = SIGNED_SHORT_INT;
    basic_types[SIGNED_SHORT_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_SHORT_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_SHORT_INT].attrs[2] = SCALAR_T;


    /* type 1 */
    basic_types[SIGNED_LONG_INT].type     = SIGNED_LONG_INT;
    basic_types[SIGNED_LONG_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_LONG_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_LONG_INT].attrs[2] = SCALAR_T;


    /* type 2 */
    basic_types[SIGNED_INT].type     = SIGNED_INT;
    basic_types[SIGNED_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_INT].attrs[2] = SCALAR_T;


    /* type 3 */
    basic_types[SIGNED_CHAR].type     = SIGNED_CHAR;
    basic_types[SIGNED_CHAR].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_CHAR].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_CHAR].attrs[2] = SCALAR_T;


    /* type 4 */
    basic_types[UNSIGNED_SHORT_INT].type     = UNSIGNED_SHORT_INT;
    basic_types[UNSIGNED_SHORT_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_SHORT_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_SHORT_INT].attrs[2] = SCALAR_T;


    /* type 5 */
    basic_types[UNSIGNED_LONG_INT].type     = UNSIGNED_LONG_INT;
    basic_types[UNSIGNED_LONG_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_LONG_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_LONG_INT].attrs[2] = SCALAR_T;


    /* type 6 */
    basic_types[UNSIGNED_INT].type     = UNSIGNED_INT;
    basic_types[UNSIGNED_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_INT].attrs[2] = SCALAR_T;


    /* type 7 */
    basic_types[UNSIGNED_CHAR].type     = UNSIGNED_CHAR;
    basic_types[UNSIGNED_CHAR].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_CHAR].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_CHAR].attrs[2] = SCALAR_T;


    /* type 8 */
    basic_types[VOID].type     = VOID;
    basic_types[VOID].attrs[0] = VOID_T;
    basic_types[VOID].attrs[1] = VOID_T;
    basic_types[VOID].attrs[2] = VOID_T;


    /* type 9 */
    basic_types[OTHER].type     = OTHER;
    basic_types[OTHER].attrs[0] = OTHER;
    basic_types[OTHER].attrs[1] = OTHER;
    basic_types[OTHER].attrs[2] = OTHER;

}





/*-----------------------------------------------
 | test_main
 +---------------------------------------------*/
static int test_main(int argc, char **argv)
{   int i;

    if(argc < 2)
    {   /* we'll take input from stdin */
        currfilename= "(stdin)";
	yylineno= 1;
	yylex();
    }
    else
    {   for(i=1; i<argc; i++)
        {   FILE *f= fopen(argv[i], "r");
	    if(!f)
	    {   perror(argv[i]);
	        return 1;
            }
	    currfilename= argv[i];

	    yyrestart(f);
	    yylineno= 1;
	    yylex();
	    fclose(f);
	}
    }	
    /*
    printrefs();
    */
    return 0;
}







/*-----------------------------------------------
 | symhash
 +---------------------------------------------*/
static unsigned symhash(char *sym)
{   unsigned int hash= 0;
    unsigned c;

    while(c= *sym++)
    {   hash= (hash*9) ^ c;
    }

    return hash;
}





/*---------------------------------------------------
 | lookup - returns the address of the symtab cell
 |          holding the specified declarator.
 +-------------------------------------------------*/
struct declarator *lookup(struct declarator *sym, struct symtabl *curr_symtab)
{   int i;
    int hash;
    struct declarator *sp;               /* used to keep current place in symbol table     */
    struct declarator *spname= sym;      /* used to investigate name of symbol in table    */
    struct declarator *symorig= sym;     /* used to keep location of original param        */



    /* fastforward to the id of the declarator parameter
     +----------------------------------------------------*/
    while(sym->next != NULL)
    {   sym= sym->next;
    }
    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }


    /* hash the symbol name */
    hash= symhash(sym->id) % NHASH;


    /* set symbol pointer 'sp' to the address of the cell that
    |  this symbol hashed to.
    +---------------------------------------------------------------*/
    sp= (struct declarator *)&curr_symtab->symtab[hash];



    int scount= NHASH;
    while(--scount >= 0)
    {

        /* fastforward to the the name of the symbol that we're currently
	|  pointing to in the symbol table.
	+---------------------------------------------------------------------*/
        spname= curr_symtab->symtab[hash];

        /* fastforward to declarator name */
        while(spname && spname->next != NULL)
        {   spname= spname->next;
        }
        if(spname &&
	     (spname->nodetype == ARRAY_DECLARATOR ||
	      spname->nodetype == FUNCTION_DECLARATOR
	     )
	  )
        {   spname= spname->adeclarator;
        }



        /* if a symbol is in this cell already, check the name of
	|  the symbol.
	+-----------------------------------------------------------*/
	if(curr_symtab->symtab[hash] != 0)  /* if cell is not empty... */
        {
            /* if the cell contains the same symbol as the declarator param,
	     * then we've found what we were looking for; return the address.
             */
            if(spname->id  &&  !strcmp(spname->id,sym->id))
	    {   return (struct declarator *) &curr_symtab->symtab[hash];
            }
        }



	/* if this cell is empty, return NULL because the symbol
	|  was not found.
	+--------------------------------------*/
	if(curr_symtab->symtab[hash] == 0)
	{   return NULL;
        }



        /* if the cell is not empty and DOES NOT store the current symbol,
	|  then a collision has occured and we need to move ahead to the
	|  next cell.  Be sure to wrap around sure to the front of the symtab
	|  if you happen to reach the back.
	+------------------------------------------------------------------*/
	if( curr_symtab->symtab[hash] != 0   &&   strcmp(spname->id,sym->id) )
	{   ++hash;

            if(hash > NHASH-1)
            {   hash=0;
		sp= (struct declarator *) &curr_symtab->symtab[hash];
            }
        }
    }

    printf("Symbol table overflow\n");
    exit(-1);
}




/*-----------------------------------------------
 | addref
 +---------------------------------------------*/
struct declarator *addref(struct declarator *sym, struct symtabl *curr_symtab)
{   int i;
    int hash;
    struct declarator *sp;               /* used to keep current place in symbol table     */
    struct declarator *spname= sym;      /* used to investigate name of symbol in table    */
    struct declarator *symorig= sym;     /* used to keep location of original param        */



    /* fastforward to the id of the declarator parameter
     +----------------------------------------------------*/
    while(sym->next != NULL)
    {   sym= sym->next;
    }
    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }


    /* hash the symbol name */
    hash= symhash(sym->id) % NHASH;


    /* set symbol pointer 'sp' to the address of the cell that
    |  this symbol is supposed to be in.
    +---------------------------------------------------------------*/
    sp= (struct declarator *)&curr_symtab->symtab[hash];



    int scount= NHASH;
    while(--scount >= 0)
    {

        /* fastforward to the the name of the declarator that we're currently
	|  pointing to in the symbol table.
	+---------------------------------------------------------------------*/
        spname= curr_symtab->symtab[hash];

        /* fastforward to declarator name */
        while(spname && spname->next != NULL)
        {   spname= spname->next;
        }
        if(spname &&
	     (spname->nodetype == ARRAY_DECLARATOR ||
	      spname->nodetype == FUNCTION_DECLARATOR
	     )
	  )
        {   spname= spname->adeclarator;
        }



        /* if the symbol is in this cell already, exit with error
	+-----------------------------------------------------------*/
	if(curr_symtab->symtab[hash] != 0)  /* if cell is not empty... */
        {
            /* and if cell contains the same id as the declarator param, then it's a dup. */
            if(spname->id  &&  !strcmp(spname->id,sym->id))
	    {   printf("Error: the variable '%s' has already been defined.\n", spname->id);
                exit(-1);
            }
        }



	/* if this cell is empty, store sym parameter there
	 +--------------------------------------------------*/
	if(curr_symtab->symtab[hash] == 0)
	{   curr_symtab->symtab[hash]= symorig;
	    return sp;
        }



        /* if the cell is not empty and DOES NOT store the current symbol,
	|  then a collision has occured and we need to move ahead to the
	|  next cell.  Be sure to wrap around sure to the front of the symtab
	|  if you happen to reach the back.
	+------------------------------------------------------------------*/
	if( curr_symtab->symtab[hash] != 0   &&   strcmp(spname->id,sym->id) )
	{   ++hash;

            if(hash > NHASH-1)
            {   hash=0;
		sp= (struct declarator *) &curr_symtab->symtab[hash];
            }
        }
    }

    printf("Symbol table overflow\n");
    exit(-1);
}





/*-----------------------------------------------
 | symcompare
 | Note: Based on C FAQ: http://c-faq.com/lib/qsort2.html
 +---------------------------------------------*/
int symcompare(const void *xa, const void *xb)
{   struct declarator *a= *(struct declarator * const *)xa;
    struct declarator *b= *(struct declarator * const *)xb;



    /* case that a and b are both NULL */
    if(!a && !b)
    {   return 0;
    }


    /* a is null, and b is not null */
    if(!a  &&  b)
    {
        /* fastforward to declarator name for b */
        while(b->next != NULL)
        {   b= b->next;
        }
        if(b->nodetype == ARRAY_DECLARATOR || b->nodetype == FUNCTION_DECLARATOR)
        {   b= b->adeclarator;
        }

        if(b->id)
        {   return 1;
	}
    }

    /* b is null, and a is not null */
    if(!b && a)
    {
        /* fastforward to declarator name for a */
        while(a->next != NULL)
        {   a= a->next;
        }
        if(a->nodetype == ARRAY_DECLARATOR || a->nodetype == FUNCTION_DECLARATOR)
        {   a= a->adeclarator;
        }

        if(a->id)
        {   return -1;
	}
    }


    /* neither a nor b is null:
     | fastforward to declarator names for a and b
     +---------------------------------------------*/
    while(a->next != NULL)
    {   a= a->next;
    }
    if(a->nodetype == ARRAY_DECLARATOR || a->nodetype == FUNCTION_DECLARATOR)
    {   a= a->adeclarator;
    }

    /* fastforward to declarator name for b */
    while(b->next != NULL)
    {   b= b->next;
    }
    if(b->nodetype == ARRAY_DECLARATOR || b->nodetype == FUNCTION_DECLARATOR)
    {   b= b->adeclarator;
    }

    return strcmp(a->id,b->id);
}





/*-----------------------------------------------
 | printrefs
 +---------------------------------------------*/
void printrefs(struct symtabl *curr_symtab)
{
    if(curr_symtab != NULL)
    {   print_symtab(curr_symtab);
        printrefs(curr_symtab->rsibling);
        printrefs(curr_symtab->child);
    }
}




/*-----------------------------------------------
 | print_symtab
 +---------------------------------------------*/
void print_symtab(struct symtabl *curr_symtab)
{   int i;
    struct declarator *sp;

    printf("\n\n\n\n\n\n\n");
    printf("=====================================================\n");
    printf(" Symbol Table '%s'\t\t(sid %d)\n", curr_symtab->id,curr_symtab->sid);
    printf("=====================================================\n");


    /* sort the symbol table
    |------------------------*/
    qsort(curr_symtab->symtab, NHASH, sizeof(struct declarator *), symcompare);


    for(i=0; i<NHASH; i++)
    {
	if(curr_symtab->symtab[i])
	{   printf("(%ld) symtab[%d]: ", &curr_symtab->symtab[i],i);
            printf("(contents %ld) ", curr_symtab->symtab[i]);

	    sp= curr_symtab->symtab[i];

            /* fastforward to declarator name for sp */
            while(sp->next != NULL)
            {   sp= sp->next;
            }
            if(sp->nodetype == ARRAY_DECLARATOR || sp->nodetype == FUNCTION_DECLARATOR)
            {   sp= sp->adeclarator;
            }

	    printf("     name: %s\n", sp->id);
	}
    }
    printf("\n\n\n");
}





void global_symtab_init(void)
{
    curr_symtab= emalloc(sizeof(struct symtabl));
    curr_symtab->sid= ROOT;
    strcpy(curr_symtab->id,"global_top_level");
    curr_symtab->parent= NULL;
    curr_symtab->child= NULL;
    curr_symtab->lsibling= NULL;
    curr_symtab->rsibling= NULL;

    global_top_level= curr_symtab;
    symtab_sid= ROOT;
}


void funcdef_to_symtab(struct function_def *funcdef)
{

    printf("DEBUG funcdef_to_symtab(): TLD is a function definition...\n");


    /* a function defintion is composed of a
     * fuction_defspec and a compound statement.
     * A compound statement has a decostat_list.
     */
    struct function_defspec *fdspec= funcdef->fdspec;
    struct ast *cstmt= funcdef->cstmt;
    struct decostat_list *dlist;
    struct parameter_list *plist;
    struct ast *dstat;
    char symtab_name[100];
    struct declarator *d;


    printf("DEBUG funcdef_to_symtab(): the function encountered is '%s'...\n", fdspec->d->adeclarator->id);


    /* Add the function name to current symbol table
    +-------------------------------------------------*/
    ast_to_symtab((struct ast *)fdspec, curr_symtab);



    /* Allocate a new symbol table.  If this is the first
    |  child of the global symbol table, then attach the new
    |  symbol table as a child of the global symbol table.
    |
    |  Otherwise, this symbol table should be a sibling of
    |  the current symbol table.
    +---------------------------------------------------------*/
    if(  (curr_symtab->sid == ROOT)  &&  (curr_symtab->child == NULL) )
    {   curr_symtab->child= emalloc(sizeof(struct symtabl));
        curr_symtab= curr_symtab->child;
        printf("DEBUG funcdef_to_symtab(): added new symtab as child...\n");
    }
    else
    {   while(curr_symtab->rsibling != NULL)
        {   curr_symtab= curr_symtab->rsibling;
	}

        curr_symtab->rsibling= emalloc(sizeof(struct symtabl));
        curr_symtab= curr_symtab->rsibling;
        printf("DEBUG funcdef_to_symtab(): added new symtab as sibling...\n");
    }



    /* Generate and apply appropriate name and sid to
    |  newly created symbol table.
    +------------------------------------------------*/
    strcpy(symtab_name,fdspec->d->adeclarator->id);
    strcat(symtab_name,"_funcdef");
    printf("\t\t\tSYMTAB_NAME: %s\n", symtab_name);
    strcpy(curr_symtab->id,symtab_name);
    curr_symtab->sid= ++symtab_sid;



    /* Add the parameters to the newly created symtab
    +-------------------------------------------------*/
    plist= fdspec->d->plist;
    printf("DEBUG funcdef_to_symtab():  Here are the function parameters...\n");
    while(plist)
    {   d= plist->pd;
        if(d->tspecptr->type != VOID)
        {   addref(d,curr_symtab);
	}
        plist= plist->next;
    }



    /* search the compound statement block for decls or labels.
    |  Add decls to the symtab.  If labels are found, create
    |  a sibling symtab to store them.
    +-----------------------------------------------------------*/
    dlist= (struct decostat_list *) cstmt->l;
    do
    {   dstat= dlist->decostat;
        if(dstat->nodetype == DECL)
	{   ast_to_symtab(dstat,curr_symtab);
	}
        printf("\t\tdecostat type: %d\n", dstat->nodetype);
        printf("\n");
    } while( (dlist= dlist->next) != NULL);


    printf("\n}\n\n");
}
