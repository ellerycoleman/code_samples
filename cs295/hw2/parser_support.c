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
     | We will cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each node pointing to either
     | a decl or a funcdef.
     +-----------------------------------------------------------------*/
    struct decl *de;
    do
    {   de= (struct decl *)tldlist->tld->d;
        switch(de->nodetype)
        {   case DECL:
	       /* print type */
	       printf("%s ", print_type(de->typespecifier));


               /* print declarator list */
	       declarator_list *dl= de->dl;
	       declarator *d;
	       parameter_list *plist;
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
		          d= dl->d;
		          do
			  {   if( d->nodetype == POINTER_DECLARATOR )
			      {   printf("*");
			      }
			      else if( d->nodetype == SIMPLE_DECLARATOR )
			      {   printf("%s", d->id);
	                         if(dl->next != NULL)
	                         {  printf(",");
                                 }
			      }

                          }while( (d= d->next) != NULL);
			  break;


		       case FUNCTION_DECLARATOR:
		          /* print function name */
			  d= dl->d->fdeclarator;
			  printf("%s(", d->id);

                          /* print parameter list */
			  print_parameter_list(dl->d->plist);
			  printf(")");
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
{   declarator *d= malloc(sizeof(declarator));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->nodetype= POINTER_DECLARATOR;
        d->next= next;
    }
    return d;
}


declarator *new_function_declarator(declarator *fdecl, parameter_list *plist)
{   declarator *d= malloc(sizeof(declarator));
    {   d->nodetype= FUNCTION_DECLARATOR;
        d->fdeclarator= fdecl;
        d->plist= plist;
	if(plist == NULL)
	{   printf("Warning: new_function_decl: plist is null!\n");
	}
	printf("function declarator addr: %d\n", d);
	printf("list was set to: %d\n", plist);
    }
    return d;
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



parameter_list *new_parameter_list(parameter_decl *pd, parameter_list *next)
{   parameter_list *pl= malloc(sizeof(struct parameter_list));
    pl->pd= pd;
    pl->next= next;
    return pl;
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


parameter_list *reverse_parameter_list(parameter_list *plist)
{   parameter_list *newroot= NULL;
    while(plist)
    {   parameter_list *next= plist->next;
        plist->next= newroot;
	newroot= plist;
	plist= next;
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



parameter_decl *new_parameter_decl(int typespec, declarator *d)
{   parameter_decl *pd= malloc(sizeof(struct parameter_decl));
    pd->typespecifier= typespec;
    pd->d= d;
    return pd;
}


char * print_type(int type)
{   char *p;
    switch(type)
    {   case SIGNED_SHORT_INT:
           p= "signed short int";
           break;
        case SIGNED_LONG_INT:
	   p= "signed long int";
	   break;
        case SIGNED_INT:
	   p= "signed int";
	   break;
        case SIGNED_CHAR:
	   p= "signed char";
	   break;
        case UNSIGNED_SHORT_INT:
	   p= "unsigned short int";
	   break;
        case UNSIGNED_LONG_INT:
	   p= "unsigned long int";
	   break;
        case UNSIGNED_INT:
	   p= "unsigned int";
	   break;
        case UNSIGNED_CHAR:
	   p= "unsigned char";
	   break;
        case VOID:
	   p= "void";
	   break;
    }


    return p;
}


void print_simple_declarator(declarator *d)
{
    printf(" %s", d->id);
    /* if(dl->next != NULL) */
    {  printf(",");
    }
}


void print_parameter_list(parameter_list *plist)
{   declarator *d;
    do
    {   switch(plist->pd->d->nodetype)
        {   
	
	    case SIMPLE_DECLARATOR:
               printf("%s %s", print_type(plist->pd->typespecifier),plist->pd->d->id);
	       break;



	    case POINTER_DECLARATOR:
               printf("%s ", print_type(plist->pd->typespecifier));
	       d= plist->pd->d;
	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   printf("*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
		   {   printf("%s", d->id);
		   }
               }while( (d= d->next) != NULL);
	       break;
        }


        if(plist->next != NULL)
        {   printf(", ");
        }
    }while((plist= plist->next) != NULL);
}









