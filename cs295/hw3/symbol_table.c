/*============================================================================
| File:		symbol_table.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements symbol table functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

extern struct basic_type basic_types[];
extern struct symtabl *symtab;



/*-----------------------------------------------
 | create_symbol_tables
 +---------------------------------------------*/
void create_symbol_tables(struct ast *parse_tree)
{   symbol_table_init();


}




/*-----------------------------------------------
 | symbol_table_init
 +---------------------------------------------*/
void symbol_table_init(void)
{   int i;


    /* Initialize global symbol table */
    symtab= emalloc(sizeof(struct symtabl));
    symtab->id= "global_top_level";
    symtab->parent= NULL;
    symtab->sibling= NULL;



    /* Initialize basic types */
    basic_types[SIGNED_SHORT_INT].type     = SIGNED_SHORT_INT;
    basic_types[SIGNED_SHORT_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_SHORT_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_SHORT_INT].attrs[2] = SCALAR_T;


    basic_types[SIGNED_LONG_INT].type     = SIGNED_LONG_INT;
    basic_types[SIGNED_LONG_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_LONG_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_LONG_INT].attrs[2] = SCALAR_T;


    basic_types[SIGNED_INT].type     = SIGNED_INT;
    basic_types[SIGNED_INT].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_INT].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_INT].attrs[2] = SCALAR_T;


    basic_types[SIGNED_CHAR].type     = SIGNED_CHAR;
    basic_types[SIGNED_CHAR].attrs[0] = INTEGRAL_T;
    basic_types[SIGNED_CHAR].attrs[1] = ARITHMETIC_T;
    basic_types[SIGNED_CHAR].attrs[2] = SCALAR_T;


    basic_types[UNSIGNED_SHORT_INT].type     = UNSIGNED_SHORT_INT;
    basic_types[UNSIGNED_SHORT_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_SHORT_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_SHORT_INT].attrs[2] = SCALAR_T;


    basic_types[UNSIGNED_LONG_INT].type     = UNSIGNED_LONG_INT;
    basic_types[UNSIGNED_LONG_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_LONG_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_LONG_INT].attrs[2] = SCALAR_T;


    basic_types[UNSIGNED_INT].type     = UNSIGNED_INT;
    basic_types[UNSIGNED_INT].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_INT].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_INT].attrs[2] = SCALAR_T;


    basic_types[UNSIGNED_CHAR].type     = UNSIGNED_CHAR;
    basic_types[UNSIGNED_CHAR].attrs[0] = INTEGRAL_T;
    basic_types[UNSIGNED_CHAR].attrs[1] = ARITHMETIC_T;
    basic_types[UNSIGNED_CHAR].attrs[2] = SCALAR_T;


    basic_types[VOID].type     = VOID;
    basic_types[VOID].attrs[0] = VOID_T;
    basic_types[VOID].attrs[1] = VOID_T;
    basic_types[VOID].attrs[2] = VOID_T;


    basic_types[OTHER].type     = OTHER;
    basic_types[OTHER].attrs[0] = OTHER;
    basic_types[OTHER].attrs[1] = OTHER;
    basic_types[OTHER].attrs[2] = OTHER;



    /* initializing symbtab array */
    printf("original symtab:\n");
    printf("----------------\n");
    for(i=0; i<NHASH; i++)
    {   printf("(%ld) symtab[%d]: %d\n", &symtab->symtab[i], i, symtab->symtab[i]);   
    }
    printf("\n\n\n");
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
    printrefs();

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







/*-----------------------------------------------
 | lookup
 +---------------------------------------------*/
struct declarator *lookup(struct declarator *sym)
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
    sp= (struct declarator *)&symtab->symtab[hash];



    int scount= NHASH;
    while(--scount >= 0)
    {   

        /* fastforward to the the name of the declarator that we're currently
	|  pointing to in the symbol table.
	+---------------------------------------------------------------------*/
        spname= symtab->symtab[hash];

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
	if(symtab->symtab[hash] != 0)  /* if cell is not empty... */
        {   
            /* and if cell contains the same id as the declarator param, then it's a dup. */
            if(spname->id  &&  !strcmp(spname->id,sym->id))
	    {   printf("Error: the variable '%s' has already been defined.\n", spname->id);
                exit(-1);
            }
        }



	/* if this cell is empty, store sym parameter there
	 +--------------------------------------------------*/
	if(symtab->symtab[hash] == 0)
	{   symtab->symtab[hash]= symorig;
	    return sp;
        }



        /* if the cell is not empty and DOES NOT store the current symbol,
	|  then a collision has occured and we need to move ahead to the
	|  next cell.  Be sure to wrap around sure to the front of the symtab
	|  if you happen to reach the back.     
	+------------------------------------------------------------------*/
	if( symtab->symtab[hash] != 0   &&   strcmp(spname->id,sym->id) )
	{   ++hash;

            if(hash > NHASH-1)
            {   hash=0;
		sp= (struct declarator *) &symtab->symtab[hash];
            }
        }
    }

    printf("Symbol table overflow\n");
    exit(-1);
}




/*-----------------------------------------------
 | addref
 +---------------------------------------------*/
void addref(char *filename, int lineno, struct declarator *dp)
{
    lookup(dp);
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
void printrefs(void)
{   struct declarator *sp;
    int i;

    /* sort the symbol table */
    qsort(symtab->symtab, NHASH, sizeof(struct declarator *), symcompare);


    for(i=0; i<NHASH; i++)
    {
	if(symtab->symtab[i])
	{   printf("(%ld) symtab[%d]: ", &symtab->symtab[i],i);
            printf("(contents %ld) ", symtab->symtab[i]);
            
	    sp= symtab->symtab[i];

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




