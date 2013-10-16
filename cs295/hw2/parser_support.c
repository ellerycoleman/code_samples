/*============================================================================
| File:		parser_support.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements user defined functions for parser.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



#include "parser_support.h"
#include "bison_parser.tab.h"

extern int yylineno;


yyerror(char *s,...)
{   fprintf(stderr, "Problem in Parseville!\n");
    fprintf(stderr, "\terror: %s\n", s);
    fprintf(stderr, "\tline number: %d\n\n", yylineno);
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
    struct decl *tdecl;
    struct function_def *funcdef;
    int i=1;
    struct declarator *d;


    do  /* cycle through all of the TLD's */
    {   
    
            /*--------------------------------------------+
	     |            if TLD is a DECL
	     +--------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;

            /* print declarator list */
            print_expr((struct ast *)tdecl);

	    printf(";\n");
	}




            /*--------------------------------------------+
	     |      if TLD is a FUNCTION_DEFINITION
	     +--------------------------------------------*/
        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {

	    /* retrieve function definition */
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;

            /* a function defintion is composed of a
	     * fuction_defspec and a compound statement. 
	     */
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
    struct decl *tdecl;
    struct declarator_list *dl;
    struct declarator *d;
    struct ast *tast;
    struct cond_expr *cexpr;


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

        case LABEL:
	   k= (struct constant *)expr;
	   printf("%s", k->value);
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


        case COMPOUND_STATEMENT:
	    dlist= (struct decostat_list *)expr->l;
	    printf("{   ");
	    do
	    {   print_expr(dlist->decostat);
	        printf(";\n");
            }while( (dlist= dlist->next) != NULL);
	    printf("}\n");
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


        case OP_ASSIGNMENT_MULTIPLY:
	   print_expr(expr->l);
	   printf("*=");
	   print_expr(expr->r);
	   break;



        case OP_ASSIGNMENT_DIVIDE:
	   print_expr(expr->l);
	   printf("/=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_REMAINDER:
	   print_expr(expr->l);
	   printf("%=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_LEFT_BITSHIFT:
	   print_expr(expr->l);
	   printf("<<=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_RIGHT_BITSHIFT:
	   print_expr(expr->l);
	   printf(">>=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_BITWISE_AND:
	   print_expr(expr->l);
	   printf("&=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_BITWISE_OR:
	   print_expr(expr->l);
	   printf("|=");
	   print_expr(expr->r);
	   break;


        case OP_ASSIGNMENT_BITWISE_XOR:
	   print_expr(expr->l);
	   printf("^=");
	   print_expr(expr->r);
	   break;


        case DECL:
	   tdecl= (struct decl *)expr;
           dl= tdecl->dl;
	   dl= reverse_declarator_list(dl);


           /* print declarator list */
	   printf("%s ",print_type(tdecl->typespecifier));
	   do
	   {   d= dl->d;
	       print_decl((struct ast *)d);
	       if(dl->next != NULL)
	       {   printf(", ");
	       }
	   }while( (dl= dl->next) != NULL);
	   break;


        case SIMPLE_DECLARATOR:
	   print_decl(expr);
	   break;


        case UNARY_MINUS_EXPR:
	   printf("-");
	   print_expr(expr->l);
	   break;


        case UNARY_PLUS_EXPR:
	   printf("+");
	   print_expr(expr->l);
	   break;


        case PREDECREMENT_EXPR:
	   printf("--");
	   print_expr(expr->l);
	   break;


        case POSTDECREMENT_EXPR:
	   print_expr(expr->l);
	   printf("--");
	   break;


        case PREINCREMENT_EXPR:
	   printf("++");
	   print_expr(expr->l);
	   break;


        case POSTINCREMENT_EXPR:
	   print_expr(expr->l);
	   printf("++");
	   break;


        case INDIRECTION_EXPR:
	   printf("*");
	   print_expr(expr->l);
	   break;


        case ADDRESS_EXPR:
	   printf("&");
	   print_expr(expr->l);
	   break;


        case BITWISE_NEGATION_EXPR:
	   printf("~");
	   print_expr(expr->l);
	   break;


        case LOGICAL_NEGATION_EXPR:
	   printf("!");
	   print_expr(expr->l);
	   break;


        case LABELED_STATEMENT:
	   print_expr(expr->l);
	   printf(":   ");
	   print_expr(expr->r);
	   break;


        case PARENTHESIZED_EXPR:
	   printf("(");
	   print_expr(expr->l);
	   printf(")");
	   break;


        case CAST_EXPR:
	   tast= expr->l;
	   d= (struct declarator *)tast;
	   printf("(");
	   printf("%s ", print_type(d->typespecifier));
	   print_decl((struct ast *)d);
	   printf(")");
	   print_expr(expr->r);
	   break;


        case SUBSCRIPT_EXPR:
	   print_expr(expr->l);
	   printf("[");
	   print_expr(expr->r);
	   printf("]");
	   break;


        case CONDITIONAL_EXPR:
	   cexpr= (struct cond_expr *)expr;
	   print_expr(cexpr->cond);
	   printf(" ? ");
	   print_expr(cexpr->return1);
	   printf(" : ");
	   print_expr(cexpr->return2);
	   break;


        case LOGICAL_OR_EXPR:
	   print_expr(expr->l);
	   printf(" || ");
	   print_expr(expr->r);
	   break;


        case LOGICAL_AND_EXPR:
	   print_expr(expr->l);
	   printf(" && ");
	   print_expr(expr->r);
	   break;


        case BITWISE_OR_EXPR:
	   print_expr(expr->l);
	   printf(" | ");
	   print_expr(expr->r);
	   break;


        case BITWISE_XOR_EXPR:
	   print_expr(expr->l);
	   printf(" ^ ");
	   print_expr(expr->r);
	   break;


        case BITWISE_AND_EXPR:
	   print_expr(expr->l);
	   printf(" & ");
	   print_expr(expr->r);
	   break;


        case OP_EQUALITY:
	   print_expr(expr->l);
	   printf(" == ");
	   print_expr(expr->r);
	   break;


        case OP_NON_EQUALITY:
	   print_expr(expr->l);
	   printf(" != ");
	   print_expr(expr->r);
	   break;


        case OP_RELATIONAL_LT:
	   print_expr(expr->l);
	   printf(" < ");
	   print_expr(expr->r);
	   break;


        case OP_RELATIONAL_LTE:
	   print_expr(expr->l);
	   printf(" <= ");
	   print_expr(expr->r);
	   break;


        case GREATER_THAN_SYMBOL:
	   print_expr(expr->l);
	   printf(" > ");
	   print_expr(expr->r);
	   break;


        case OP_RELATIONAL_GTE:
	   print_expr(expr->l);
	   printf(" >= ");
	   print_expr(expr->r);
	   break;














        case RW_RETURN:
	   printf("return");
	   if( (expr->l) != NULL)
	   {   printf(" ");
	       print_expr(expr->l);
	   }
	   break;





        default:
	   printf("PRINT_EXPR: not sure what to do with nodetype: %d\n",expr->nodetype);
	   break;

    }
}




void print_decl(struct ast *expr)
{
    struct declarator_list *dl;
    struct declarator *d;
    struct declarator *ad;
    struct ast *tast;


    switch(expr->nodetype)
    {   case  SIMPLE_DECLARATOR:
           d= (struct declarator *)expr;
           printf("%s", d->id);
           break;


	 case POINTER_DECLARATOR:
	    d= (struct declarator *)expr;
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
	    d= (struct declarator *)expr;
	    printf("%s(", d->adeclarator->id);

         /* print parameter list */
	    print_parameter_list(d->plist);
	    printf(")");
            break;


        case ARRAY_DECLARATOR:
           d= (struct declarator *)expr;
	   printf(" %s", d->adeclarator->id);
	   printf("[");
	   print_expr((struct ast *)d->exp);
	   printf("]");
	   break;

      
        case DIRECT_ABSTRACT_DECLARATOR:
	   print_dad(d);
	   break;

        default:
	   printf("PRINT_DECL: not sure what to do with nodetype: %d\n",expr->nodetype);
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


declarator *new_array_declarator(int type, struct declarator *arrydec, struct ast *expr)
{   declarator *d= malloc(sizeof(declarator));
    if(d == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
    }
    else
    {   d->nodetype= ARRAY_DECLARATOR;
        d->adeclarator= arrydec;
        d->exp= (struct expr *)expr;
    }
    return d;
}


declarator *new_direct_abstract_declarator(int type, struct ast *data, declarator *next)
{   declarator *d= malloc(sizeof(struct declarator));
    
    d->nodetype= DIRECT_ABSTRACT_DECLARATOR;
    d->dadtype= type;

    if(type == PAREN_ENCLOSED)
    {   d->adeclarator= (struct declarator *)data;
    }
    else if(type == BRACKET_EXPR)
    {   d->exp= (struct expr *)data;
    }
    else if(type == DAD_LIST)
    {   d->exp= (struct expr *)data;
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
        default:
	   p= "";
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
	pd->dadtype= d->dadtype;
	pd->adeclarator= d->adeclarator;
	pd->exp= d->exp;
	pd->plist= d->plist;
    }
    else
    {   pd->nodetype= (enum ntype) -1;  /* specified type only */
    }
    return pd;
}



void print_parameter_list(parameter_list *plist)
{   declarator *d;
    declarator *ad;
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
		   else if( d->nodetype == DIRECT_ABSTRACT_DECLARATOR )
		   {   print_dad(d);
		   }
               }while( (d= d->next) != NULL);
	       break;

	    case DIRECT_ABSTRACT_DECLARATOR:
	       d= plist->pd;
	       print_dad(d);
	       break;


	    case DECL:
	       d= plist->pd;
	       print_decl((struct ast *)d);
	       

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


           default:
	      d= plist->pd;
	      printf("PRINT_PARAMETER_LIST: not sure what to do with nodetype: %d\n",d->nodetype);
	      break;

        }

        if(plist->next != NULL)
        {   printf(", ");
        }
    }while((plist= plist->next) != NULL);
}


void print_dad(declarator *d)
{   struct declarator *ad;
               

    switch(d->dadtype)
    {   
        case PAREN_ENCLOSED:
           ad= d->adeclarator;
           printf("%s ", print_type(d->typespecifier));
           printf("(");
           print_decl((struct ast *)ad);
           printf(")");
           break;


        case BRACKET_NO_EXPR:
	   printf("%s ", print_type(d->typespecifier));
           printf("[]");
	   break;


        case BRACKET_EXPR:
	   printf("%s ", print_type(d->typespecifier));
           printf("[");
	   print_expr((struct ast *)d->exp);
           printf("]");
	   break;


        case DAD_LIST:
           printf("%s ", print_type(d->typespecifier));
	   d= reverse_declarators(d);
	   do
	   {   /* print the expression ending the dad_list */
	       if(d->exp != NULL && d->next == NULL)
	       {   printf("[");
	           print_expr((struct ast *)d->exp);
                   printf("]");
               }
	       else if(d->exp != NULL && d->next != NULL)
	       {   
	           printf("[");
	           print_expr((struct ast *)d->exp);
                   printf("]");
               }

	       /* print dad_list members without typespecifiers */
	       else
	       {   
	           switch(d->dadtype)
	           {   
	               case PAREN_ENCLOSED:
                       ad= d->adeclarator;
                       printf("(");
		       print_decl((struct ast *)ad);
                       printf(")");
		       break;


                       case BRACKET_NO_EXPR:
                          printf("[]");
                          break;


                       case BRACKET_EXPR:
                          printf("[");
		          print_expr((struct ast *)d->exp);
                          printf("]");
		          break;
                                 
		       default:
		          printf("ERROR: problem printing dad_list...\n");
		          break;
		   }
               }
           }while( (d= d->next) != NULL);
	   break;

        default:
	   printf("unknown dad type: %d\n",d->dadtype);
    }

}



struct ast *new_expr(int type,struct ast *l, struct ast *r)
{   struct expr *expr= malloc(sizeof(struct expr));
    expr->nodetype= type;
    expr->l= l;
    expr->r= r;

    return (struct ast *)expr;
}


struct ast *new_conditional_expr(struct ast *cond, struct ast *return1, struct ast *return2)
{   struct cond_expr *expr= malloc(sizeof(struct cond_expr));
    expr->nodetype= CONDITIONAL_EXPR;
    expr->cond= cond;
    expr->return1= return1;
    expr->return2= return2;

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


struct ast *new_flow(struct ast *cond, struct ast *thendo, struct ast *els)
{   struct flow *tflow= malloc(sizeof(struct flow));
    tflow->cond= cond;
    tflow->thendo= thendo;
    tflow->els= els;

    return (struct ast *)tflow;
}



