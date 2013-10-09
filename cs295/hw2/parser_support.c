#include "parser_support.h"


yyerror(char *s)
{   fprintf(stderr, "Problem in Parseville!\n\t");
    fprintf(stderr, "error: %s\n", s);
}




void print_tree(ast *nodeptr)
{   printf("Entering print_tree()...\n");
    printf("(");

    /* The only type of AST that is passed to print_tree is a *tld_list.
     | we will cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each one pointing to either
     | a decl or a funcdef.  AT this point, we only handle decls.
     +-----------------------------------------------------------------*/
    do
    {   struct decl *de= (struct decl *)tldlist->tld->d;
        switch(de->nodetype)
        {   case DECL:
               printf("this tldlist node is a DECL...\n");
	       printf("  - type of decl is '%d'\n", de->typespecifier);
               switch( de->typespecifier)
	       {   case SIGNED_SHORT_INT:
	              printf("signed short int ");
		      break;
                   case SIGNED_LONG_INT:
	              printf("signed long int ");
		      break;
                   case SIGNED_INT:
	              printf("signed int ");
		      break;
                   case SIGNED_CHAR:
	              printf("signed char ");
		      break;
                   case UNSIGNED_SHORT_INT:
	              printf("unsigned short int ");
		      break;
                   case UNSIGNED_LONG_INT:
	              printf("unsigned long int ");
		      break;
                   case UNSIGNED_INT:
	              printf("unsigned int ");
		      break;
                   case UNSIGNED_CHAR:
	              printf("unsigned char ");
		      break;
                   case VOID:
	              printf("void ");
		      break;
	       }
	       declarator_list *dl= de->dl;
	       dl= reverse_declarator_list(dl);
	       do
	       {   printf(" %s", dl->d->id);
	           if(dl->next != NULL)
	           {  printf(",");
                   }
	       }while( (dl= dl->next) != NULL);
	       printf(";\n");
	       break;
        }
    }while( (tldlist= tldlist->next) != NULL );
    printf(")\n");
}





declarator *new_simple_declarator(char *id)
{   printf("new_simple_declarator() was called with id '%s'\n", id);
    declarator *d= malloc(sizeof(declarator));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->id= strdup(id);
    }

    return d;
}


declarator_list *new_declarator_list(declarator *d, declarator_list *next)
{   printf("new_declarator_list() was called...\n");
    declarator_list *dl= malloc(sizeof(struct declarator_list));
    if(dl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   dl->d= d;
        dl->next= next;
    }
    return dl;
}


ast *new_tld_list(tld *t, ast *next)
{   printf("new_tld_list() was called...\n");
    tld_list *tl= malloc(sizeof(struct tld_list));
    if(tl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   tl->tld= t;
        tl->next= (struct tld_list *)next;
    }
    return (struct ast *)tl;
}


/*
 | The declarator list is defined using left recursion.
 | For this reason, the linked list is created in the
 | reverse order of the way that it was entered.  This
 | method reverses the linked list, and is based on the
 | linked list reversal method posted on stackoverflow.com.
 +---------------------------------------------------------*/
declarator_list *reverse_declarator_list(declarator_list *dl)
{   declarator_list *newroot= NULL;   
    while(dl)
    {   declarator_list *next= dl->next;
        dl->next= newroot;
	newroot= dl;
	dl= next;
    }
    return newroot;
}




ast *new_decl(int typespecifier, declarator_list *dl)
{   printf("new_decl() was called...\n");
    decl *d= malloc(sizeof(struct decl));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->nodetype= DECL;
        d->typespecifier= typespecifier;
        d->dl=  dl;
    }

    return  (struct ast *)d;
}

tld *new_tld(int datatype, ast *tld)
{   printf("new_tld() was called...\n");
    struct tld *t= malloc(sizeof(struct tld));
    if(t == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   t->nodetype= TLD;
        t->datatype= datatype;
        if(datatype == DECL)
	{   t->d= (struct decl *)tld;
	}
	if(datatype == FUNCDEF)
	{   t->f= (struct funcdef *)tld;
	}
    }    
    return t;
}







