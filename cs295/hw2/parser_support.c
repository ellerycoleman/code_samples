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



    /* The only type of AST that is passed to print_tree is a *tld_list,
     | so we'll cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each node pointing to either
     | a decl or a funcdef.
     +-----------------------------------------------------------------*/
    struct decl *de;
    struct ast *funcdef;
    int i=1;


    do  /* cycle through all of the TLD's */
    {   if(tldlist->tld->datatype == DECL)
        {   de= (struct decl *)tldlist->tld->d;
        printf("tld %d: %s\n", i++, print_type(de->nodetype));
        switch(de->nodetype)
        {   case DECL:
	       /* print type */
	       printf("%s ", print_type(de->typespecifier));


	       declarator_list *dl= de->dl;
	       declarator *d;
	       parameter_list *plist;
	       dl= reverse_declarator_list(dl);


               /* print declarator list */
	       do
	       {   
	       
	           switch( dl->d->nodetype )
		   {   
		   
		   
		       case SIMPLE_DECLARATOR:
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
			  d= dl->d->adeclarator;

                          /* print parameter list */
			  printf("%s(", d->id);
			  print_parameter_list(dl->d->plist);
			  printf(")");
                          break;

                   }
	       }while( (dl= dl->next) != NULL);
	    printf(";\n");
	    break;
        }
	}/* end if DECL */




        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {   funcdef= (struct ast *)tldlist->tld->f;
	    /* print function return type */
	    printf("%s fn();\n", print_type((long)funcdef->l->l));
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
        d->adeclarator= fdecl;
        d->plist= plist;
	if(plist == NULL)
	{   printf("Warning: new_function_decl: plist is null!\n");
	}
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



parameter_list *new_parameter_list(declarator *pd, parameter_list *next)
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


struct ast *new_decostat_list(struct ast *decostat, struct ast *next)
{   decostat_list *dl= malloc(sizeof(struct decostat_list));
    {   dl->nodetype= DECOSTAT_LIST;
        dl->decostat= decostat;
	dl->next= (struct decostat_list *)next;
    }

    return (struct ast *)dl;
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
	if(datatype == FUNCTION_DEFINITION)
	{   t->f= (struct ast *)tld;
	}
    }
    return (struct ast *)t;
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



declarator *new_parameter_decl(int typespec, declarator *d)
{   declarator *pd= malloc(sizeof(struct declarator));
    pd->typespecifier= typespec;
    if(d != NULL)
    {   pd->nodetype= d->nodetype;
        pd->next= d->next;
        pd->id= d->id;
    }
    else
    {   pd->nodetype= (enum ntype) -1;  /* specified type only */
    }
    return pd;
}



void print_parameter_list(parameter_list *plist)
{   declarator *d;
    do
    {   
        switch(plist->pd->nodetype)
        {   
	
	    case SIMPLE_DECLARATOR:
               printf("%s ", print_type(plist->pd->typespecifier));
	       if(plist->pd->id != NULL)
	       {   printf("%s", plist->pd->id);
	       }
	       break;



	    case POINTER_DECLARATOR:
               printf("%s ", print_type(plist->pd->typespecifier));
	       d= plist->pd;
	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   printf("*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
		   {   printf("%s", d->id);
		   }
               }while( (d= d->next) != NULL);
	       break;


            case -1:  /* print type only */
               printf("%s ", print_type(plist->pd->typespecifier));
	       d= plist->pd;
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





struct ast *new_expr(int type,struct ast *l, struct ast *r)
{   struct expr *expr= malloc(sizeof(struct expr));
    expr->nodetype= type;
    expr->l= l;
    expr->r= r;

    return (struct ast *)expr;
}



struct ast *new_constant(int type, void *value)
{   struct constant *k= malloc(sizeof(struct constant));
    k->nodetype= type;
    k->value= value;
    return (struct ast *)k;
}



struct ast *new_function_def_specifier(int type, struct declarator *d)
{   struct ast *fdefspec= malloc(sizeof(struct ast));
    fdefspec->nodetype= FUNCTION_DEF_SPECIFIER;
    fdefspec->l= (struct ast *)(long)type;
    fdefspec->r= (struct ast *)d;
    return fdefspec;
}



struct ast *new_function_definition(struct ast *fdefspec, struct ast *compound_stmt)
{   struct ast *fdef= malloc(sizeof(struct ast));
    fdef->nodetype= FUNCTION_DEFINITION;
    fdef->l= fdefspec;
    fdef->r= compound_stmt;
    return fdef;
}


struct ast *new_compound_statement(struct ast *decstmtlist)
{   struct ast *cstmt= malloc(sizeof(struct ast));
    cstmt->nodetype= COMPOUND_STATEMENT;
    cstmt->l= decstmtlist;

    return cstmt;

}




