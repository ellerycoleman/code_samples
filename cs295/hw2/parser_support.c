#include "parser_support.h"


yyerror(char *s,...)
{   fprintf(stderr, "Problem in Parseville!\n\t");
    fprintf(stderr, "error: %s\n", s);
}




void print_tree(ast *nodeptr)
{   printf("\n\n\n\n");
    printf("#----------------------------------------------------------\n");
    printf("#           Parse Tree for E95 C Language                  \n");
    printf("#----------------------------------------------------------\n");
    printf("(\n\n\n");



    /* The only type of AST that is passed to print_tree is a *tld_list.
     | we will cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each node pointing to either
     | a decl or a funcdef.
     +-----------------------------------------------------------------*/
    struct decl *de;
    do
    {
        de= (struct decl *)tldlist->tld->d;
        switch(de->nodetype)
        {   case DECL:
	       /* print type */
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


               /* print list */
	       declarator_list *dl= de->dl;
	       declarator *pd;
	       dl= reverse_declarator_list(dl);
	       do
	       {   switch( dl->d->nodetype )
		   {   case SIMPLE_DECLARATOR:
		          printf(" %s", dl->d->id);
	                  if(dl->next != NULL)
	                  {  printf(",");
                          }
			  break;


		       case POINTER_DECLARATOR:
		          pd= dl->d;
		          do
			  {   if( pd->nodetype == POINTER_DECLARATOR )
			      {   printf("*");
			      }
			      else if( pd->nodetype == SIMPLE_DECLARATOR )
			      {   printf("%s", pd->id);
	                         if(dl->next != NULL)
	                         {  printf(",");
                                 }
			      }

                          }while( (pd= pd->next) != NULL);
			  break;


                   }
	       }while( (dl= dl->next) != NULL);
	       printf(";\n");
	       break;
        }
    }while( (tldlist= tldlist->next) != NULL );
    printf("\n\n\n)\n\n\n\n");
}





declarator *new_simple_declarator(char *id)
{   declarator *d= malloc(sizeof(declarator));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->nodetype= SIMPLE_DECLARATOR;
        d->id= strdup(id);
    }
    return d;
}


declarator *new_pointer_declarator(declarator *next)
{   declarator *pd= malloc(sizeof(declarator));
    if(pd == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   pd->nodetype= POINTER_DECLARATOR;
        pd->next= next;
    }
    return pd;
}






declarator_list *new_declarator_list(declarator *d, declarator_list *next)
{   declarator_list *dl= malloc(sizeof(struct declarator_list));
    if(dl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   dl->d= d;
        dl->next= next;
    }
    return dl;
}


tld_list *new_tld_list(ast *t, ast *next)
{   tld_list *tl= malloc(sizeof(struct tld_list));
    if(tl == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   tl->tld= (struct tld *)t;
        tl->next= (struct tld_list *)next;
    }
    return tl;
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



tld_list *reverse_tld_list(struct tld_list *tl)
{   tld_list *newroot= NULL;
    while(tl)
    {   tld_list *next= tl->next;
        tl->next= newroot;
	newroot= tl;
	tl= next;
    }
    return newroot;
}


declarator *reverse_declarators(declarator *dp)
{   declarator *newroot= NULL;
    while(dp)
    {   declarator *next= dp->next;
        dp->next= newroot;
	newroot= dp;
	dp= next;
    }
    return newroot;
}



ast *new_decl(int typespecifier, declarator_list *dl)
{   decl *d= malloc(sizeof(struct decl));
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

ast *new_tld(int datatype, ast *tld)
{   struct tld *t= malloc(sizeof(struct tld));
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
    return (struct ast *)t;
}







