/*============================================================================
| File:		symbol_table.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements symbol table functions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



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
struct symbol *lookup(char *sym)
{   struct symbol *sp= &symtab[symhash(sym)%NHASH];
    int scount= NHASH;

    while(--scount >= 0)
    {   
        /* case of exact hash match */
        if(sp->name  &&  !strcasecmp(sp->name,sym))
        {   return sp;
	}

        /* case hash not currently present in symtab */
	if(!sp->name)  
	{   sp->name= strdup(sym);
	    sp->reflist= 0;
	    return sp;
        }


	/* case of hash pointing to symtab entry that's already taken.       */
	/* Need to move ahead to the next entry, and be sure to wrap around  */
	/* to the front of the symtab if you happen to reach the back.       */
	if(++sp >= symtab+NHASH)
	{   sp=symtab;
	}
    }

    printf("Symbol table overflow\n");
    exit(-1);
}




/*-----------------------------------------------
 | addref
 +---------------------------------------------*/
void addref(int lineno, char *filename, char *word, int flags)
{   struct ref *r;
    struct symbol *sp= lookup(word);

    /* don't add ref for dups of same line/file */
    if(sp->reflist                       &&
       sp->reflist->lineno   == lineno   &&
       sp->reflist->filename == filename
      )
    {   return;
    }


    r= malloc(sizeof(struct ref));
    if(!r)
    {   printf("Out of space!\n");
        exit(-1);
    }

    r->next = sp->reflist;
    r->filename= filename;
    r->lineno= lineno;
    r->flags= flags;
    sp->reflist = r;
}




/*-----------------------------------------------
 | symcompare
 +---------------------------------------------*/
static int symcompare(const void *xa, const void *xb)
{   const struct symbol *a= xa;
    const struct symbol *b= xb;

    if(!a->name)       /* symbol name for 'a' is null */
    {   if(!b->name)   /* symbol name for 'a' AND 'b' is null */
        {   return 0;
	}
	return 1;
    }

    if(!b->name)
    {   return -1;
    }

    return strcmp(a->name,b->name);
}




/*-----------------------------------------------
 | printrefs
 +---------------------------------------------*/
void printrefs(void)
{   struct symbol *sp;


    /* sort the symbol table */
    qsort(symtab, NHASH, sizeof(struct symbol), symcompare); 

    for(sp=symtab; sp->name && sp < symtab+NHASH; sp++)
    {   char *prevfn= NULL;

        /* reverse list of references */
	struct ref *rp= sp->reflist;
	struct ref *rpp= 0;           /* previous ref */
	struct ref *rpn;              /* next ref     */

	do
	{   rpn= rp->next;
	    rpp= rp;
	    rp= rpn;
        }while(rp);


	/* print the word and its references */
	printf("%20s", sp->name);
	for(rp= rpp; rp; rp = rp->next)
	{   if(rp->filename == prevfn)
	    {   printf(" %d", rp->lineno);
	    }
	    else
	    {   printf(" %s:%d", rp->filename, rp->lineno);
	        prevfn= rp->filename;
	    }
        }
	printf("\n");
    }
}





