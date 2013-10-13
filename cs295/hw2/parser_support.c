#include "parser_support.h"
#include "bison_parser.tab.h"


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
    struct function_def *funcdef;
    int i=1;


    do  /* cycle through all of the TLD's */
    {   if(tldlist->tld->datatype == DECL)
        {   de= (struct decl *)tldlist->tld->d;
            /* printf("tld %d: %s\n", i++, print_type(de->nodetype)); */
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
	        {   print_decl((struct ast *)dl);
		    if(dl->next != NULL)
		    {   printf(", ");
		    }
	        }while( (dl= dl->next) != NULL);
	        printf(";\n");
	        break;
            }
	}/* end if DECL */




        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;
	    struct function_defspec *fdspec= funcdef->fdspec;
	    struct ast *cstmt= funcdef->cstmt;
	    struct decostat_list *dlist;

            /* printf("tld %d:\n", i++); */


	    /* print function return type */
	    printf("\n\n%s ", print_type(fdspec->typespec));
	    printf("%s(", fdspec->d->adeclarator->id);
	    print_parameter_list(fdspec->d->plist);
	    printf(")");
	    printf("\n{");


            /* display compound statement block */
	    dlist= (struct decostat_list *) cstmt->l;
	    do
	    {   print_expr(dlist->decostat);
	        printf(";\n");
	    } while( (dlist= dlist->next) != NULL);


	    printf("\n}   ");
	}



    }while( (tldlist= tldlist->next) != NULL );
    printf("\n\n\n)\n\n\n\n");
}



void print_expr(struct ast *expr)
{   struct constant *k;
    int i=0;
    struct decostat_list *dlist;
    switch(expr->nodetype)
    {   case SEP_SEMICOLON:
	   break;

        case INTEGER_CONSTANT:
	   k= (struct constant *)expr;
	   printf("%d", k->value);
	   break;

        case CHARACTER_CONSTANT:
	   k= (struct constant *)expr;
	   printf("'%c'", k->value);
	   break;

        case CHARACTER_CONSTANT_OCTAL:
	   k= (struct constant *)expr;
	   printf("%s", k->value);
	   break;

        case STRING_CONSTANT:
	   k= (struct constant *)expr;
	   printf("\"%s\"", k->value);
	   break;

        case RW_GOTO:
	   printf("goto ");
	   printf("%s", expr->l);
	   break;

        case RW_CONTINUE:
	   printf("continue");
	   break;

        case RW_BREAK:
	   printf("break");
	   break;

        case DECOSTAT_LIST:  /* comma separated statements */
	    dlist= (struct decostat_list *)expr;
	    dlist= reverse_decostat_list(dlist);
	    while(dlist->next != NULL)
	    {   print_expr(dlist->decostat);
	        printf(", ");
	        dlist= dlist->next;
	    }
	    print_expr(dlist->decostat);
	    break;


        case PLUS_SIGN:
	   print_expr(expr->l);
	   printf("+");
	   print_expr(expr->r);
	   break;


        case MINUS_SIGN:
	   print_expr(expr->l);
	   printf("-");
	   print_expr(expr->r);
	   break;

        case ASTERISK:
	   print_expr(expr->l);
	   printf("*");
	   print_expr(expr->r);
	   break;

        case OP_DIVISION:
	   print_expr(expr->l);
	   printf("/");
	   print_expr(expr->r);
	   break;

        case OP_REMAINDER:
	   print_expr(expr->l);
	   putchar('%');
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT:
	   print_expr(expr->l);
	   printf("=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_ADD:
	   print_expr(expr->l);
	   printf("+=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_SUBTRACT:
	   print_expr(expr->l);
	   printf("-=");
	   print_expr(expr->r);
	   break;


        case SIMPLE_DECLARATOR:
	   print_decl(expr);
	   break;



        default:
	   printf("PRINT_EXPR: I'm not sure what i was called with?  nodetype: %d\n",expr->nodetype);
	   break;

    }
}




void print_decl(struct ast *expr)
{
    struct declarator_list *dl;
    struct declarator *d;

    switch(expr->nodetype)
    {   case  SIMPLE_DECLARATOR:
          d= (struct declarator *)expr;
          printf("%s", d->id);
          break;


	 case POINTER_DECLARATOR:
	    d= dl->d;
	    do
	    {   if( d->nodetype == POINTER_DECLARATOR )
	        {   printf("*");
	        }
	        else if( d->nodetype == SIMPLE_DECLARATOR )
	        {   printf("%s", d->id);
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

        default:
	   printf("PRINT_DECL: I'm not sure what i was called with?  nodetype: %d\n",expr->nodetype);
	   break;
    }

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


struct decostat_list *reverse_decostat_list(struct decostat_list *dlist)
{   struct decostat_list *newroot= NULL;
    while(dlist)
    {   struct decostat_list *next= dlist->next;
        dlist->next= newroot;
	newroot= dlist;
	dlist= next;
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
               printf("%s", print_type(plist->pd->typespecifier));
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
{   struct function_defspec *fdefspec= malloc(sizeof(struct function_defspec));
    fdefspec->nodetype= FUNCTION_DEF_SPECIFIER;
    fdefspec->typespec= type;
    fdefspec->d= d;
    return (struct ast *)fdefspec;
}



struct ast *new_function_definition(struct ast *fdefspec, struct ast *compound_stmt)
{   struct function_def *fdef= malloc(sizeof(struct function_def));
    fdef->nodetype= FUNCTION_DEFINITION;
    fdef->fdspec= (struct function_defspec *)fdefspec;
    fdef->cstmt= compound_stmt;
    return (struct ast *)fdef;
}


struct ast *new_compound_statement(struct ast *decstmtlist)
{   struct ast *cstmt= malloc(sizeof(struct ast));
    cstmt->nodetype= COMPOUND_STATEMENT;
    cstmt->l= decstmtlist;

    return cstmt;

}




