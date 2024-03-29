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
struct goto_queue
{   int populated;
    struct ast *dstat;
    struct symtabl *symtab;
};
struct goto_queue goto_q[100];

struct change
{   struct declarator *unresolved;
    struct declarator *resolved;
};

struct change_list
{   struct change *c;
    struct change_list *next;
};

struct change_list *clist;
struct change_list *clist_front;



/*-----------------------------------------------
 | create_symbol_tables
 +---------------------------------------------*/
void create_symbol_tables(struct ast *parse_tree)
{
    struct decl *tdecl;
    struct declarator_list *decolist;
    struct function_def *funcdef;
    int i=1;
    int tldloop=0;
    struct declarator *d;
    struct declarator *dporig;
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


        /*--------------------------------------------+
        |            if TLD is a DECL
        +---------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;

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




    /* Cycle through all symbols in the global_top_level symtab and
    |  verify that there are no more function declarators left.
    |  Also set global member within each declarator.
    +------------------------------------------------------------------*/
    for(i=0; i<NHASH; i++)
    {   /*printf("global_top_level->symtab[%d]: %ld\n",    i,global_top_level->symtab[i]); */

        if(global_top_level->symtab[i])
	{   d= global_top_level->symtab[i];
	    dporig= d;



	    /* Set global member
	    +---------------------*/
	    d->global=1;



            /* ffwd pointer declarators */
            if(d->nodetype == POINTER_DECLARATOR)
            {   /* fast_forward */
	        while(d->next)
	        {   d= d->next;
	        }
            }


            /* if this is a function declarator...
            +---------------------------------------*/
            if(d->nodetype == FUNCTION_DECLARATOR)
            {
	        /* Allow exceptions for the builtin functions.
		+----------------------------------------------*/
		if( (! strcmp(print_declarator_id(d),"printint"))      ||
                    (! strcmp(print_declarator_id(d),"printstring"))   ||
                    (! strcmp(print_declarator_id(d),"readint")) 
		  )
		{   /* do nothing */
		}



                /* if this declarator doesn't belong to a function def,
                |  then it's a declarator that has not been defined.
	        |  Exit with an error.
	        +--------------------------------------------------------*/
	        else if(!dporig->funcdef_true)
	        {   printf("\nError: function prototype for '%s' was never defined.\n", d->adeclarator->id);
		    exit(-1);
	        }

            }
        }
    }


    printf("\n\n");
    process_change_list();

}








/*-----------------------------------------------
 | ast_to_symtab
 +---------------------------------------------*/
void ast_to_symtab(struct ast *sym, struct symtabl *curr_symtab)
{   struct declarator_list *decolist;
    struct declarator *dp;
    struct declarator *dporig;
    struct decl *tdecl;
    struct function_defspec *fdspec;




    if(sym->nodetype == DECL)
    {   tdecl= (struct decl *)sym;
        decolist= tdecl->dl;
        do
        {   dp= decolist->d;
	    dporig= decolist->d;
	    dporig->tspecptr= tdecl->tspecptr;


	    /* ffwd pointer declarators */
	    if(dp->nodetype == POINTER_DECLARATOR)
	    {   while(dp->next)
	        {   dp= dp->next;
		}
            }

	    /* copy declarator type to function declarators */
	    if(dp->nodetype == FUNCTION_DECLARATOR)
	    {   dporig->tspecptr= (struct basic_type *)tdecl->tspecptr;
	        dp->tspecptr= (struct basic_type *)tdecl->tspecptr;
	    }


            addref(dporig,curr_symtab);
        }while(decolist= decolist->next);
    }



    else if(sym->nodetype == FUNCTION_DEF_SPECIFIER)
    {   fdspec= (struct function_defspec *)sym;
	dp= fdspec->d;
	addref(dp,curr_symtab);
    }



    else if(sym->nodetype == POINTER_DECLARATOR)
    {   dp= (struct declarator *)sym;
	addref(dp,curr_symtab);
    }



    else if(sym->nodetype == FUNCTION_DECLARATOR)
    {   dp= (struct declarator *)sym;
	addref(dp,curr_symtab);
    }



    /* check for any identifiers */
    else
    {   printf("WARNING: ast_to_symtab(): called with unknown nodetype: %d\n", sym->nodetype);
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
    printtabs();
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
 | lookup - returns the address of the declarator
 |          in the symtab cell.
 +-------------------------------------------------*/
struct declarator *lookup(struct declarator *sym, struct symtabl *curr_symtab)
{   int i;
    int hash;
    struct declarator *sp;               /* used to keep current place in symbol table     */
    struct declarator *spname= sym;      /* used to investigate name of symbol in table    */
    struct declarator *symorig= sym;     /* used to keep location of original param        */



    /* fastforward to the id of the declarator parameter
     +----------------------------------------------------*/
    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    if(sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    if(sym->nodetype == POINTER_DECLARATOR)
    {   while(sym->next)
        {   sym= sym->next;
	}
	if(!sym->id)
	{   sym= sym->adeclarator;
	}
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
	    {   return (struct declarator *) curr_symtab->symtab[hash];
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









/*---------------------------------------------------
 | resolve - Just like lookup (returns the address of
 |           the declarator in the symtab cell) but it
 |           also searches all parent symtabs.
 +-------------------------------------------------*/
struct declarator *resolve(struct declarator *sym, struct symtabl *curr_symtab)
{   int i;
    int hash;
    struct declarator *sp;               /* used to keep current place in symbol table     */
    struct declarator *spname= sym;      /* used to investigate name of symbol in table    */
    struct declarator *symorig= sym;     /* used to keep location of original param        */



    /* fastforward to the id of the declarator parameter
     +----------------------------------------------------*/
    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    if(sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    if(sym->nodetype == POINTER_DECLARATOR)
    {   while(sym->next)
        {   sym= sym->next;
	}
	if(!sym->id)
	{   sym= sym->adeclarator;
	}
    }


    /* hash the symbol name */
    hash= symhash(sym->id) % NHASH;


    /* set symbol pointer 'sp' to the address of the cell that
    |  this symbol hashed to.
    +---------------------------------------------------------------*/
    sp= (struct declarator *)&curr_symtab->symtab[hash];



search_table:
    ;
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
	    {   return (struct declarator *) curr_symtab->symtab[hash];
            }
        }



	/* if this cell is empty, switch to the parent symtab
	|  if possible and continue the search.
	+----------------------------------------------------*/
	if(curr_symtab->symtab[hash] == 0)
	{   
	    if(curr_symtab->parent != 0)
	    {   curr_symtab= curr_symtab->parent;
	        goto search_table;
	    }
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

    if(curr_symtab->parent != NULL)
    {   curr_symtab= curr_symtab->parent;
        goto search_table;
    }

    printf("Error: Symbol '%s' not found in symbol table\n", sym->id);
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


    symorig->curr_symtab= curr_symtab;



    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    else if(sym->nodetype == POINTER_DECLARATOR)  /* fast forward to appropriate declarator */
    {   while(sym->next)
        {   sym= sym->next;
	}
	if(!sym->id)
	{   sym= sym->adeclarator;
	}
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



        /* if the symbol is in this cell already...
	+----------------------------------------------------*/
	if(curr_symtab->symtab[hash] != 0)
        {
            /* and if cell contains the same id as the declarator param, then it's a dup. */
            if(spname->id  &&  !strcmp(spname->id,sym->id))
	    {   printf("Error: the variable '%s' has already been defined.\n", spname->id);
                exit(-1);
            }
        }



	/* if this cell is empty, store sym parameter there
	|  AND set the curr_symtab setting for the declarator.
        +-----------------------------------------------------*/
	if(curr_symtab->symtab[hash] == 0)
	{   curr_symtab->symtab[hash]= symorig;
	    curr_symtab->symtab[hash]->curr_symtab= curr_symtab;
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
 | remref - removes a reference from the symtab
 +---------------------------------------------*/
void remref(struct declarator *sym, struct symtabl *curr_symtab)
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



        /* if the symbol is in this cell already...
	+------------------------------------------*/
	if(curr_symtab->symtab[hash] != 0)
        {
            /* and if cell contains the same id as the declarator param, then it's a dup. */
            if(spname->id  &&  !strcmp(spname->id,sym->id))
	    {   curr_symtab->symtab[hash]=0;
	        return;
            }
        }



	/* if this cell is empty, then there's nothing to do
	 +--------------------------------------------------*/
	if(curr_symtab->symtab[hash] == 0)
	{  ;
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

    printf("Error trying to remove symbol '%s'\n", spname->id);
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
 | printtabs
 +---------------------------------------------*/
void printtabs(struct symtabl *curr_symtab)
{
    if(curr_symtab != NULL)
    {
        print_symtab(curr_symtab);
	print_symtab(curr_symtab->labels);
        printtabs(curr_symtab->rsibling);
        printtabs(curr_symtab->child);
    }
}




/*-----------------------------------------------
 | print_symtab
 +---------------------------------------------*/
void print_symtab(struct symtabl *curr_symtab)
{
    /* protection against printing null symtabs */
    if(curr_symtab == NULL)
    {   return;
    }



    int i;
    struct declarator *sp;
    char typename[30];


    printf("\n\n\n\n\n\n\n");
    printf("=====================================================\n");
    printf(" Symbol Table: '%s'\t\t(sid %d)\n", curr_symtab->id,curr_symtab->sid);
    if(curr_symtab->parent)
    {   printf(" parent_table: (%s,%d)\n", curr_symtab->parent->id, curr_symtab->parent->sid);
    }
    else
    {   printf(" parent_table: NULL\n");
    }
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
	    if(sp->tspecptr)
	    {   sprintf(typename,"%s",print_type(sp->tspecptr->type));
	    }
	    else
	    {   sprintf(typename,"%s", "N/A");
	    }


            /* fastforward to declarator name for sp */
            while(sp->next != NULL)
            {   sp= sp->next;
            }
            if(sp->nodetype == ARRAY_DECLARATOR || sp->nodetype == FUNCTION_DECLARATOR)
            {   sp= sp->adeclarator;
            }

	    printf("     name: %s", sp->id);
	    printf("     type: %s\n", typename);
	}
    }
    printf("\n\n\n");
}






/*-----------------------------------------------
 | global_symtab_init
 +---------------------------------------------*/
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







/*-----------------------------------------------
 | funcdef_to_symtab
 +---------------------------------------------*/
void funcdef_to_symtab(struct function_def *funcdef)
{

    /* a function defintion is composed of a
    |  fuction_defspec and a compound statement.
    |  A compound statement has a decostat_list.
    +---------------------------------------------*/
    struct function_defspec *fdspec= funcdef->fdspec;
    struct ast *cstmt= funcdef->cstmt;


    struct decostat_list *decolist;
    struct parameter_list *plist;
    struct ast *dstat;
    char symtab_name[100];
    char tmpstr[TMPSTRSZ];
    struct declarator *d;
    struct declarator *dporig;
    struct declarator *func;
    char *funcname;





    /* A function defspec contains a typespec (needs to be changed)
    |  and a declarator.  Retrieve the declarator.
    +--------------------------------------------------------------*/
    dporig= fdspec->d;
    dporig->tspecptr= &basic_types[fdspec->typespec];





    /*------------------------------------------------------
    |  Check to see if the function name is already in the
    |  symbol table from a function prototype.  If so,
    |  make sure that the types from the function
    |  prototype and the function defintion are the same.
    +------------------------------------------------------*/


    /* if function name is in symbol table...
    +-------------------------------------------*/
    if(func= lookup(dporig,curr_symtab))
    {   /* printf("funcname IS ALREADY in table\n"); */


        /* if the symbol references a function definition...  exit with error
	+---------------------------------------------------------------------*/
        if(func->funcdef_true)
	{   printf("Error: redefinition of function '%s' not allowed\n", func->id);
	    exit(-1);
        }


        /* if the symbol references a function prototype,
	|  make sure the prototype matches the function definition.
	+----------------------------------------------------------*/
	struct parameter_list *fplist;
	if(func->nodetype == POINTER_DECLARATOR)   /* ffwd thru pointers */
	{   while(func->next)
	    {   func= func->next;
	    }
	}


	if(func->nodetype == FUNCTION_DECLARATOR)
	{
            fplist= func->plist;
	
	    char tmp1[TMPSTRSZ];
	    char tmp2[TMPSTRSZ];

	    funcdecl_to_string(lookup(dporig,curr_symtab),tmp1);
	    funcdef_to_string(funcdef,tmp2);

            if(strcmp(tmp1,tmp2))
	    {   printf("ERROR: function prototype and function definition do not match.\n");

	        printf("\nfunc decl: %s\n", tmp1);
	        printf(" func def: %s\n", tmp2);
	        printf("\n\n");

		exit(-1);
            }
	}

	remref(func,curr_symtab);
    }
    else
    {   /* printf("funcname IS NOT in table\n"); */
    }





    /* Add the function name to current symbol table
    +-------------------------------------------------*/
    dporig->funcdef_true=1;
    ast_to_symtab((struct ast *)dporig, curr_symtab);



    /* Allocate a new symbol table.  If this is the first
    |  child of the global symbol table, then attach the new
    |  symbol table as a child of the global symbol table.
    |
    |  Otherwise, this symbol table should be a sibling of
    |  the current symbol table.
    +---------------------------------------------------------*/
    if(  (curr_symtab->sid == ROOT)  &&  (curr_symtab->child == NULL) )
    {   curr_symtab->child= emalloc(sizeof(struct symtabl));
        curr_symtab->child->parent= curr_symtab;
        curr_symtab= curr_symtab->child;
    }
    else if(  (curr_symtab->sid == ROOT)  &&  (curr_symtab->child != NULL) )
    {   
        curr_symtab= curr_symtab->child;
        while(curr_symtab->rsibling != NULL)
        {   curr_symtab= curr_symtab->rsibling;
	}

        curr_symtab->rsibling= emalloc(sizeof(struct symtabl));
        curr_symtab->rsibling->parent= curr_symtab;
        curr_symtab= curr_symtab->rsibling;
    }
    else
    {   while(curr_symtab->rsibling != NULL)
        {   curr_symtab= curr_symtab->rsibling;
	}

        curr_symtab->rsibling= emalloc(sizeof(struct symtabl));
        curr_symtab->rsibling->parent= curr_symtab;
        curr_symtab= curr_symtab->rsibling;
    }




    /*------------------------------------------------
    |  Generate and apply appropriate name and sid to
    |  newly created symbol table.
    +------------------------------------------------*/




    /* Retrieve the function name from this declarator, considering
    |  that pointers may be present.  Grab plist also.
    +---------------------------------------------------------------*/
    d= fdspec->d;
    if(d->nodetype == POINTER_DECLARATOR)
    {   while(d->next)
        {   d= d->next;
	}

	plist= d->plist;

	if(!d->id)
	{   d= d->adeclarator;
	}
    }
    else
    {   plist= d->plist;
        d= d->adeclarator;
    }
    funcname= d->id;

    strcpy(symtab_name,funcname);
    strcat(symtab_name,"_funcdef");
    strcpy(curr_symtab->id,symtab_name);
    curr_symtab->sid= ++symtab_sid;




    /* Add the parameters to the newly created symtab
    +-------------------------------------------------*/
    int i=0;
    while(plist)
    {   d= plist->pd;
        if(d->tspecptr->type != VOID)
        {   addref(d,curr_symtab);
	}
        plist= plist->next;
    }




    /* Process the compound statement, adding symbols to the
    |  appropriate symtabs.
    +---------------------------------------------------------*/
    compound_to_symtab(cstmt,curr_symtab);



    /* Resolve goto statements.
    +-----------------------------*/
    int j;
    for(j=0; j<100; j++)
    {   if(goto_q[j].populated == 1)
        {   locate_ids(goto_q[j].dstat,goto_q[j].symtab);
	}
    }

}










void compound_to_symtab(struct ast *cstmt, struct symtabl *curr_symtab)
{   struct decostat_list *decolist;
    struct ast *dstat;
    int i=0, j;



    /* search the compound statement block for decls, labels,
    |  compound statements or simple declarators.
    |  Add decls to the symtab.  If labels are found, create
    |  a sibling symtab to store them.  make sure
    +-----------------------------------------------------------*/
    decolist= (struct decostat_list *) cstmt->l;
    do
    {   dstat= decolist->decostat;
        if(dstat->nodetype == DECL)
	{   ast_to_symtab(dstat,curr_symtab);
	}
	else if(dstat->nodetype == COMPOUND_STATEMENT)
	{

            /* Allocate a new symbol table.  Compound statements can
            |  only occur in the context of a function, but they can
            |  be nested.
            |
            |  Case 1:
            |  If the current symbol table is for a function, and the child
            |  table is NULL, then we'll create a table to be the child
            |  of the function table.
            |
            |  Case 2:
            |  If the current symbol table is for a function, and the child
            |  table is NOT NULL, then we'll create a sibling table.
            |
            |  Case 3:
            |  If the current symbol table is for a compound statement,
            |  and the child is null, then we'll create a table to be
            |  the child of this compound statement table.
            |
            |  Case 4:
            |  If the current symbol table is for a compound statement,
            |  and the child is NOT NULL, then we'll create a sibling table.
            +------------------------------------------------------------------*/

            /* CASE 1: Create child table for function */
            if(  (strstr(curr_symtab->id, "_funcdef"))  &&  (curr_symtab->child == NULL) )
            {   compound_to_symtab_case1(curr_symtab,dstat);
            }


            /* CASE 2: Create child-sibling table for function */
            else if(  (strstr(curr_symtab->id, "_funcdef"))  &&  (curr_symtab->child != NULL) )
            {   compound_to_symtab_case2(curr_symtab,dstat);
            }


            /* CASE 3: Create child table for compound statement */
            else if(  (strstr(curr_symtab->id, "_child"))  &&  (curr_symtab->child == NULL) )
            {   compound_to_symtab_case3(curr_symtab,dstat);
            }


            /* CASE 4: Create child-sibling table for compound statement */
            else if(  (strstr(curr_symtab->id, "_child"))  &&  (curr_symtab->child != NULL) )
            {   compound_to_symtab_case4(curr_symtab,dstat);
            }


	}
	else if(dstat->nodetype == LABELED_STATEMENT)
	{   label_to_symtab(dstat,curr_symtab);
	}




	/* skip goto statements in first pass */
	else if(dstat->nodetype == RW_GOTO)
	{   for(j=0; j<100; j++)
	    {   if(goto_q[j].populated != 1)
	        {
                    /* Navigate to the appropriate symtab for labels. */
		    struct symtabl *tsymtab= curr_symtab;
		    while( (strstr(curr_symtab->id,"_funcdef") == NULL)  &&
		           (curr_symtab->parent != NULL)
		         )
		    {   tsymtab= tsymtab->parent;
		    }

		    goto_q[j].populated= 1;
		    goto_q[j].dstat= dstat;
		    goto_q[j].symtab= tsymtab;
		    j=200;
                 }
            }
	}


        /* locate any identifiers  */
	else
	{
	    locate_ids(dstat,curr_symtab);
	}


    } while( (decolist= decolist->next) != NULL);


}









void locate_ids(struct ast *dstat, struct symtabl *curr_symtab)
{   struct ast *tmpdstat= dstat;
    struct declarator *d;
    char tmpstr[TMPSTRSZ];

    if(dstat == NULL)
    {   return;
    }


    if(dstat->nodetype == RW_GOTO)
    {   resolve_id(dstat,curr_symtab->labels);
	return;
    }

    else if(dstat->nodetype == POINTER_DECLARATOR)
    {   while(d->next != NULL)
        {   d= d->next;
	}
    }

    else if(dstat->nodetype == SIMPLE_DECLARATOR)
    {
        d= (struct declarator *)dstat;
	resolve_id(dstat,curr_symtab);
    }


    else if(dstat->nodetype == FOR_STATEMENT)
    {
        /* retrieve statement components
	+--------------------------------*/
        struct flow *tflow    = (struct flow *)dstat;
        struct ast *forinit   = tflow->forinit;
        struct ast *cond      = tflow->cond;
        struct ast *forupdate = tflow->forupdate;
        struct ast *thendo    = tflow->thendo;

        
	locate_ids(forinit,curr_symtab);
	locate_ids(cond,curr_symtab);
	locate_ids(forupdate,curr_symtab);
	locate_ids(thendo,curr_symtab);
    }


    else if(dstat->nodetype == IF_STATEMENT)
    {
        /* retrieve statement components
	+--------------------------------*/
        struct flow *tflow    = (struct flow *)dstat;
        struct ast *cond      = tflow->cond;
        struct ast *thendo    = tflow->thendo;

        
	locate_ids(cond,curr_symtab);
	locate_ids(thendo,curr_symtab);
    }


    else if(dstat->nodetype == STRING_CONSTANT)
    {   return;
    }



    else if(dstat->nodetype == IF_ELSE_STATEMENT)
    {
        /* retrieve statement components
	+--------------------------------*/
        struct flow *tflow    = (struct flow *)dstat;
        struct ast *cond      = tflow->cond;
        struct ast *thendo    = tflow->thendo;
        struct ast *elsedo    = tflow->elsedo;

        
	locate_ids(cond,curr_symtab);
	locate_ids(thendo,curr_symtab);
	locate_ids(elsedo,curr_symtab);
    }



    else if(dstat->nodetype == STRING_CONSTANT)
    {   return;
    }



    if( (dstat->l != NULL)  &&  (dstat->l->nodetype != INTEGER_CONSTANT) )
    {   locate_ids(dstat->l,curr_symtab);
    }


    if( (dstat->r != NULL)  &&  (dstat->r->nodetype != INTEGER_CONSTANT) )
    {   locate_ids(dstat->r,curr_symtab);
    }
}





void resolve_id(struct ast *dstat, struct symtabl *curr_symtab)
{
    struct declarator *d;
    struct declarator *resolved;


    if(dstat->nodetype == RW_GOTO)
    {   d= new_simple_declarator((char *)dstat->l);
        d= lookup(d,curr_symtab);
	if(d == NULL)
	{   printf("ERROR: Label '%s' used but not defined.\n", dstat->l);
	    exit(-1);
        }
    }



    /* if((dstat->nodetype == SIMPLE_DECLARATOR)  ||  (dstat->nodetype == POINTER_DECLARATOR)) */
    {

        d= (struct declarator *)dstat;
        resolved= resolve(d,curr_symtab);


        /* record change for change list */
        if(clist_front == NULL)
        {   clist_front= emalloc(sizeof(struct change_list));
	    clist=clist_front;
	    clist->c= emalloc(sizeof(struct change));
	    clist->c->unresolved= d;
	    clist->c->resolved= resolved;
	}
	else
	{   clist=clist_front;
	    while(clist->next != NULL)
	    {   clist= clist->next;
	    }
	    clist->next= emalloc(sizeof(struct change_list));
	    clist= clist->next;
	    clist->c= emalloc(sizeof(struct change));
	    clist->c->unresolved= d;
	    clist->c->resolved= resolved;
	}
    }
}




void compound_to_symtab_case1(struct symtabl *curr_symtab, struct ast *dstat)
{
    /* create new symtab */
    curr_symtab->child= emalloc(sizeof(struct symtabl));
    curr_symtab->child->parent= curr_symtab;

    /* switch to new symtab and provide parameters */
    char newname[100];
    strcpy(newname,curr_symtab->id);
    char *edit;
    edit= strstr(newname,"_funcdef");
    strcpy(edit,"_func");

	
    curr_symtab= curr_symtab->child;
    strcpy(curr_symtab->id,newname);
    strcat(curr_symtab->id,"_child");
    sprintf(&curr_symtab->id[strlen(curr_symtab->id)],"%d",
            ++curr_symtab->parent->child_count
    );
    curr_symtab->sid= ++symtab_sid;

    compound_to_symtab(dstat,curr_symtab);
}




void compound_to_symtab_case2(struct symtabl *curr_symtab, struct ast *dstat)
{
    /* create new symtab */
    struct symtabl *rightmost_sibling;
    rightmost_sibling= curr_symtab->child;

    while(rightmost_sibling->rsibling)
    {   rightmost_sibling= rightmost_sibling->rsibling;
    }

    rightmost_sibling->rsibling= emalloc(sizeof(struct symtabl));
    rightmost_sibling->rsibling->parent= rightmost_sibling->parent;
    rightmost_sibling->rsibling->lsibling= rightmost_sibling;


    /* switch to new symtab and provide parameters */
    curr_symtab= rightmost_sibling->rsibling;
    char newname[100];
    strcpy(newname,curr_symtab->parent->id);
    char *edit;
    edit= strstr(newname,"_funcdef");
    strcpy(edit,"_func");

		
    strcpy(curr_symtab->id,newname);
    strcat(curr_symtab->id,"_child");
    sprintf(&curr_symtab->id[strlen(curr_symtab->id)],"%d",
            ++curr_symtab->parent->child_count
    );
    curr_symtab->sid= ++symtab_sid;
    compound_to_symtab(dstat,curr_symtab);
}





void compound_to_symtab_case3(struct symtabl *curr_symtab, struct ast *dstat)
{
    /* create new symtab */
    curr_symtab->child= emalloc(sizeof(struct symtabl));
    curr_symtab->child->parent= curr_symtab;

    /* switch to new symtab and provide parameters */
    curr_symtab= curr_symtab->child;
    strcpy(curr_symtab->id,curr_symtab->parent->id);
    strcat(curr_symtab->id,"_child");
    sprintf(&curr_symtab->id[strlen(curr_symtab->id)],"%d",
            ++curr_symtab->parent->child_count
    );
    curr_symtab->sid= ++symtab_sid;
    compound_to_symtab(dstat,curr_symtab);
}






void compound_to_symtab_case4(struct symtabl *curr_symtab, struct ast *dstat)
{
    /* create new symtab */
    struct symtabl *rightmost_sibling;
    rightmost_sibling= curr_symtab->child;

    while(rightmost_sibling->rsibling)
    {   rightmost_sibling= rightmost_sibling->rsibling;
    }

    rightmost_sibling->rsibling= emalloc(sizeof(struct symtabl));
    rightmost_sibling->rsibling->parent= rightmost_sibling->parent;
    rightmost_sibling->rsibling->lsibling= rightmost_sibling;


    /* switch to new symtab and provide parameters */
    curr_symtab= rightmost_sibling->rsibling;
    char newname[100];
    strcpy(newname,curr_symtab->parent->id);

    strcpy(curr_symtab->id,newname);
    strcat(curr_symtab->id,"_child");
    sprintf(&curr_symtab->id[strlen(curr_symtab->id)],"%d",
	    ++curr_symtab->parent->child_count
    );
    curr_symtab->sid= ++symtab_sid;
    compound_to_symtab(dstat,curr_symtab);

}






void label_to_symtab(struct ast *labelstmt, struct symtabl *curr_symtab)
{

    /* retrieve the label name.
    +---------------------------*/
    char *label= (char *)((struct constant *)labelstmt->l)->value;


    /* navigate left and up in the tree of symbol tables,
    |  looking for the first occurence of a function definition.
    +------------------------------------------------------------*/
    while(curr_symtab->lsibling != NULL)
    {   curr_symtab= curr_symtab->lsibling;
    }
    while( (strstr(curr_symtab->id,"_funcdef") == NULL)  &&
           (strcmp(curr_symtab->id,"global_top_level") != 0)
         )
    {   curr_symtab= curr_symtab->parent;
    }




    /* allocate space for the label symtab if necessary.
    +----------------------------------------------------*/
    if(curr_symtab->labels == NULL)
    {   curr_symtab->labels= emalloc(sizeof(struct symtabl));

        /* provide id and parameters for label symtab
        +---------------------------------------------*/
        char newname[100];
        strcpy(newname,curr_symtab->id);
        strcat(strstr(newname,"_funcdef"),"_labels");
        strcpy(curr_symtab->labels->id, newname);
        curr_symtab->labels->sid= ++symtab_sid;
        curr_symtab->labels->parent= curr_symtab;
    }




    /* switch to labels symtab
    +---------------------------*/
    curr_symtab= curr_symtab->labels;



    /* add current label to symbol table
    +-------------------------------------*/
    addref(new_label_declarator(labelstmt),curr_symtab);
}







/*-----------------------------------------------
 | process_change_list()
 +---------------------------------------------*/
void process_change_list(void)
{   struct declarator *unresolved;
     struct declarator *resolved;


     clist= clist_front;
     while(clist)
     {   unresolved= clist->c->unresolved;
         resolved= clist->c->resolved;


#ifdef DEBUG
	 printf("moving '%s' (%ld) to '%s' (%ld) -- unresolved symtab: %ld, resolved symtab: %ld\n",
	        print_declarator_id(unresolved),
		unresolved,
	        print_declarator_id(resolved),
		resolved,
		unresolved->curr_symtab,
		resolved->curr_symtab
               );
#endif


        *unresolved= *resolved;
	clist= clist->next;
     }
}



