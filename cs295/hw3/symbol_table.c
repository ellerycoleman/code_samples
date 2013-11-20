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



/*-----------------------------------------------
 | create_symbol_tables
 +---------------------------------------------*/
void create_symbol_tables(struct ast *parse_tree)
{   

    printf("DEBUG create_symbol_tables(): invoked.\n");


    /* Initialize basic_types data structure
    +-----------------------------------------*/
    basic_types_init();
    


    /* Initialize global symbol table
    +----------------------------------*/
    curr_symtab= emalloc(sizeof(struct symtabl));
    curr_symtab->id= "global_top_level";
    curr_symtab->parent= NULL;
    curr_symtab->sibling= NULL;
    curr_symtab->child= NULL;



    /* The only type of AST that is passed to print_tree is a *tld_list,
    |  so we'll cast the AST into a *tld_list.
    +-------------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)parse_tree;



    /* A tld_list has a series of tld nodes; each node pointing to either
    |  a decl or a funcdef.
    +---------------------------------------------------------------------*/
    struct decl *tdecl;
    struct function_def *funcdef;
    int i=1;
    struct declarator *d;


    
    do  /* cycle through all of the TLD's */
    {
        /*--------------------------------------------+
        |            if TLD is a DECL
        +---------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;
            printf("DEBUG create_symbol_tables(): TLD is a decl...\n");

            /* Add decl to symbol table */
            decl_to_symtab(tdecl);
	}




        /*--------------------------------------------+
        |      if TLD is a FUNCTION_DEFINITION
        +---------------------------------------------*/
        if(tldlist->tld->datatype == FUNCTION_DEFINITION  &&  0  )
        {

	    /* retrieve function definition */
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;

            /* a function defintion is composed of a
	     * fuction_defspec and a compound statement.
	     */
	    struct function_defspec *fdspec= funcdef->fdspec;
	    struct ast *cstmt= funcdef->cstmt;
	    struct decostat_list *dlist;


	    /* print function return type */
	    printf("\n\n%s ", print_type(fdspec->typespec));
	    printf("%s(", fdspec->d->adeclarator->id);
	    print_parameter_list(fdspec->d->plist);
	    printf(")");
	    printf("\n{\n");


            /* display compound statement block */
	    dlist= (struct decostat_list *) cstmt->l;
	    do
	    {   print_expr(dlist->decostat);
	        printf(";\n");
	    } while( (dlist= dlist->next) != NULL);


	    printf("\n}\n\n");
	}



    }while( (tldlist= tldlist->next) != NULL );
}



void decl_to_symtab(struct decl *tdecl)
{   struct declarator_list *tmpdl;
    struct declarator *dp;

    tmpdl= tdecl->dl;
    do
    {   dp= tmpdl->d;
        addref("stdin",yylineno,dp);
    }while(tmpdl= tmpdl->next);
        
}




/*-----------------------------------------------
 | symbol_table_init
 +---------------------------------------------*/
void basic_types_init(void)
{   int i;



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
{   extern int global_allocation;
    struct declarator *sp;
    int i;


    
    /* ascend to top level, global symbol table
    +--------------------------------------------*/
    while(curr_symtab->parent != NULL)
    {   curr_symtab= curr_symtab->parent;
    }


    /* Recurse all symbol tables, printing them to STDOUT
    +-----------------------------------------------------*/
    do
    {

        printf("\n\n\n\n\n\n\n");
        printf("=====================================================\n");
        printf(" Symbol Table '%s'\n", curr_symtab->id);
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
    }while(curr_symtab= curr_symtab->child);



    printf("\n\nTotal amount of memory dynamically allocated by parser: %d bytes\n", global_allocation);

}




