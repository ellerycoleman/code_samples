/*============================================================================
| File:		symbol_table.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements symbol table functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

int q=0;

/*-----------------------------------------------
 | test_main
 +---------------------------------------------*/

static int test_main(int argc, char **argv)
{
    int i;

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
{

    struct declarator *sp;

    /* fastforward to declarator name */
    while(sym->next != NULL)
    {   sym= sym->next;
    }

    /* calculate which symtab cell this symbol is supposed to be in */
    sp= (struct declarator *)&symtab[symhash(sym->id)%NHASH];
    int scount= NHASH;



    while(--scount >= 0)
    {

        /* if the symbol is in this cell already, exit with error */
        if(sp->id  &&  !strcmp(sp->id,sym->id))
        {   printf("Error: the variable '%s' has already been defined.\n");
            exit(-1);
        }


        /* in case hash not currently present in symtab */
        if(!sp->id)
        {   symtab[symhash(sym->id)%NHASH]= sym;
            return sp;
        }


        /* in case of hash pointing to symtab entry that's already taken.    */
        /* Need to move ahead to the next entry, and be sure to wrap around  */
        /* to the front of the symtab if you happen to reach the back.       */
        if(++sp >= (struct declarator *)symtab+NHASH)
        {   sp= (struct declarator *)symtab;
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


    printf("comparing a.addr (%ld) and b.addr (%ld) - ", a,b);
    if(!a)
    {   printf("a is null, ");
    }

    if(!b)
    {   printf("b is null ");
    }
    printf("\n");

    

    /* case that a and b are both NULL */
    if(!a && !b)
    {   return 0;
    }


    /* a is null, and b is not null */
    if(!a  &&  b)
    {   /* fast forward to name of this declarator */
        while(b->next != NULL)
	{   b= b->next;
	}
    
        if(b->id)   
        {   return 1;
	}
    }

    /* b is null, and a is not null */
    if(!b && a)
    {   /* fast forward to name of this declarator */
        while(a->next != NULL)
	{   a= a->next;
	}

        if(a->id)   
        {   return -1;
	}
    }


    /* neither a nor b is null */
    while(a->next != NULL)
    {   a= a->next;
    }
    while(b->next != NULL)
    {   b= b->next;
    }
    return strcmp(a->id,b->id);
}




/*-----------------------------------------------
 | printrefs
 +---------------------------------------------*/
void printrefs(void)
{   struct declarator *sp;
    int i;

    struct declarator *etest= symtab[2];

    printf("address of symtab[2]: %ld\n", &symtab[2]);

    printf("\n\n\n");
    printf("Symtab addr: %ld\n", &symtab);
    printf("#-----------------\n");
    printf("#    symtab before\n");
    printf("#    q: %d\n", q     );
    printf("#-----------------\n");
    for(i=0; i<NHASH; i++)
    {   printf("(%ld) symtab[%d]: ", &symtab[i], i);
        printf("%ld\n", symtab[i]);
	if(symtab[i])
	{   printf("     name: %s\n", symtab[i]->id);
	}
    }
    printf("\n\n\n");


    /* sort the symbol table */
    qsort(symtab, NHASH, sizeof(struct declarator *), symcompare);



    printf("\n\n\n");
    printf("#-----------------\n");
    printf("#    symtab after \n");
    printf("#    q: %d\n", q     );
    printf("#-----------------\n");
    for(i=0; i<NHASH; i++)
    {   printf("symtab[%d]: ", i);
        printf("%ld\n", symtab[i]);
	if(symtab[i])
	{   printf("     name: %s\n", symtab[i]->id);
	}
    }
    printf("\n\n\n");


    for(sp= (struct declarator *)symtab; sp->id && sp < (struct declarator *)symtab+NHASH; sp++)
    {   char *prevfn= NULL;

	/* print the word and its references */
	printf("%20s", sp->id);
	printf("\n");
    }
}





