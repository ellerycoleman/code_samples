/*============================================================================
| File:		generate_ir.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the parse tree and generates IR code.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


int statement_begin_sid=0;
int statement_end_sid=0;

/*-----------------------------------------------
 | generate_ir
 +---------------------------------------------*/
void generate_ir(struct ast *parse_tree)
{
    struct decl *tdecl;
    struct function_def *funcdef;
    struct declarator *d;
    struct ast *decostat;
    char genstr[TMPSTRSZ];
    struct symtabl *curr_symtab= global_top_level;
    struct irnode *irlist;
    char symtab_name[100];


    /* initialize ircodenames
    +--------------------------*/
    {   ircodenames[0]  = "UNINITIALIZED";
        ircodenames[100]= "BEGINPROC";
        ircodenames[101]= "LOADADDRESS";
	ircodenames[102]= "LOADCONSTANT";
        ircodenames[103]= "STOREWORDINDIRECT";
        ircodenames[104]= "LOADWORDINDIRECT";
        ircodenames[105]= "ENDPROC";
    };



    /* initialize irlist_front and irlist.
    +---------------------------------------*/
    irlist_front= emalloc(sizeof(struct irnode));
    irlist_front->prev=NULL;
    irlist_front->next=NULL;
    irlist_front->sid= ++irnodenum;
    irlist= irlist_front;




    /* Open IR output file
    +-------------------------*/
    strcpy(irfname,"stdin.ir");  /* update later to create an IR file
                                    based on input filename. */
    irout= fopen(irfname,"w+");
    if(irout == NULL)
    {   printf("Error opening IR output file: %s\n", irfname);
    }


    /* generate_ir() accepts the root of parse tree as its param.
    |  The root of the parse tree is a *tld_list so we'll cast the param
    |  into a *tld_list.
    +--------------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)parse_tree;



    /* A tld_list has a series of tld nodes; each node pointing to either
    |  a decl or a funcdef.  We'll create and populate symtabs as We cycle
    |  through all of the TLD's in a do-while loop.
    +---------------------------------------------------------------------*/
    do
    {   curr_symtab= global_top_level; /* always start with the global symtab */


        /*--------------------------------------------+
        |            if TLD is a DECL
        +---------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;
	    print_expr((struct ast *)tdecl,genstr);
	    fprintf(irout,"# %s\n", genstr);
	    clearstr(genstr);
	}



        /*--------------------------------------------+
        |      if TLD is a FUNCTION_DEFINITION
        +---------------------------------------------*/
        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {
	    /* retrieve function definition */
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;


            /* a function defintion is composed of a
            |  fuction_defspec and a compound statement.
            |  A compound statement has a decostat_list.
            +---------------------------------------------*/
            struct function_defspec *fdspec= funcdef->fdspec;
            struct ast *cstmt= funcdef->cstmt;
	    struct decostat_list *dlist;



	    /*
	    |  fdspec contains the function declarator.
	    +-------------------------------------------*/
	    struct declarator *d= fdspec->d;



	    /* print function return type */
	    fprintf(irout,"\n\n# %s ", print_type(fdspec->typespec));




            /* Retrieve and print function name from this declarator,
            |  considering that pointers may be present.  Also grab
	    |  the parameter_list.
            +------------------------------------------------------------*/
	    struct parameter_list *plist;
            if(d->nodetype == POINTER_DECLARATOR)
            {   while(d->next)
                {   d= d->next;
		    sprintf(&genstr[strlen(genstr)],"*");
	        }


		plist= d->plist;


	        if(!d->id)
	        {   d= d->adeclarator;
	        }
            }
	    else
	    {   d= d->adeclarator;
	        plist= fdspec->d->plist;
	    }
            char *funcname= d->id;
	    strcpy(symtab_name,funcname);
	    strcat(symtab_name,"_funcdef");



	    /* Generate IR node for function
	    +----------------------------------*/
	    irlist= irlist_front;
	    while(irlist->next != NULL)
	    {   irlist= irlist->next;
	    }

            if(irlist->sid == 1)
	    {   irlist->ircode= BEGINPROC;
	        irlist->symptr= fdspec->d;
	    }


	    sprintf(&genstr[strlen(genstr)]," %s(", funcname);
	    fprintf(irout,"%s", genstr); clearstr(genstr);



            /* print parameter list
	    +-------------------------*/
	    print_parameter_list(plist,genstr);
	    sprintf(&genstr[strlen(genstr)],")");
	    /*
	    sprintf(&genstr[strlen(genstr)],"\n#{\n");
	    fprintf(irout,"%s",genstr); clearstr(genstr);
	    */


	    /* print IR for entering function
	    +----------------------------------*/
	    fprintf(irout,"BEGINPROC(%s)\n\n\n", funcname);
            statement_begin_sid= irnodenum + 1;


            /* Generate IR for compound statement block
            +-------------------------------------------*/
	    compound_to_ir(cstmt);



            /* Generate endproc node for function.
	    +---------------------------------------*/
	    irlist= irlist_front;
            while(irlist->next != NULL)
            {   irlist= irlist->next;
            }
            irlist->next= emalloc(sizeof(struct irnode));
	    irlist->next->prev= irlist;
            irlist= irlist->next;
            irlist->sid= ++irnodenum;
            irlist->ircode= ENDPROC;
	    irlist->symptr= fdspec->d;


	}

    }while( (tldlist= tldlist->next) != NULL );


    fclose(irout);
    printf("EXITING\n");
    print_irnodes();
}








/*-----------------------------------------------
 | print_irnodes
 +---------------------------------------------*/
void print_irnodes(void)
{   struct irnode *irlist= irlist_front;
    char oprnd1[5];
    char oprnd2[5];
    char oprnd3[5];

    printf("\n\n\n");
    printf("#=========================================\n");
    printf("#         IRNODE LIST DISPLAY\n");
    printf("#=========================================\n\n");


    if(irlist == NULL)
    {   printf("IRLIST IS EMPTY\n\n");
        return;
    }

    while(irlist != NULL)
    {
        /* record oprnd1 num */
        if(irlist->oprnd1 == 0)
	{   sprintf(oprnd1, "%s", "N/A");
	}
	else
	{   sprintf(oprnd1, "r%d", irlist->oprnd1);
	}

        /* record oprnd2 num */
        if((irlist->oprnd2 == 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd2, "%s", "N/A");
	}
	else if((irlist->oprnd2 != 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd2, "r%d", irlist->oprnd2);
	}
	else if((irlist->oprnd2 != 0) &&  (irlist->ircode == LOADCONSTANT))
	{   sprintf(oprnd2, "%d", irlist->oprnd2);
	}

        /* record oprnd3 num */
        if((irlist->oprnd3 == 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd3, "%s", "N/A");
	}
	else if((irlist->oprnd3 != 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd3, "r%d", irlist->oprnd3);
	}
	else if((irlist->oprnd3 != 0) &&  (irlist->ircode == LOADCONSTANT))
	{   sprintf(oprnd3, "%d", irlist->oprnd3);
	}



        /* add IR to irlist */
        printf("irnode_sid: %3d, code: %18s (%d), symptr: %10s, oprnd1: %5s,     oprnd2: %5s,    oprnd3: %5s\n",
                irlist->sid,
		ircodenames[irlist->ircode],
		irlist->ircode,
		print_declarator_id(irlist->symptr),
		oprnd1,
		oprnd2,
		oprnd3
	      );
        irlist= irlist->next;
    }
}









/*-----------------------------------------------
 | print_irnode_sids
 +---------------------------------------------*/
void print_irnode_sids(int begin_sid,int end_sid)
{   struct irnode *irlist= irlist_front;
    char oprnd1[5];
    char oprnd2[5];
    char oprnd3[5];

    printf("\n\n\n");
    printf("#=========================================\n");
    printf("#         IRNODE LIST DISPLAY\n");
    printf("#=========================================\n\n");


    if(irlist == NULL)
    {   printf("IRLIST IS EMPTY\n\n");
        return;
    }

    while(irlist != NULL)
    {

        if( (irlist->sid >= begin_sid)  &&  (irlist->sid <= end_sid) )
	{

        /* record oprnd1 num */
        if(irlist->oprnd1 == 0)
	{   sprintf(oprnd1, "%s", "N/A");
	}
	else
	{   sprintf(oprnd1, "r%d", irlist->oprnd1);
	}

        /* record oprnd2 num */
        if((irlist->oprnd2 == 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd2, "%s", "N/A");
	}
	else if((irlist->oprnd2 != 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd2, "r%d", irlist->oprnd2);
	}
	else if((irlist->oprnd2 != 0) &&  (irlist->ircode == LOADCONSTANT))
	{   sprintf(oprnd2, "%d", irlist->oprnd2);
	}

        /* record oprnd3 num */
        if((irlist->oprnd3 == 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd3, "%s", "N/A");
	}
	else if((irlist->oprnd3 != 0) &&  (irlist->ircode != LOADCONSTANT))
	{   sprintf(oprnd3, "r%d", irlist->oprnd3);
	}
	else if((irlist->oprnd3 != 0) &&  (irlist->ircode == LOADCONSTANT))
	{   sprintf(oprnd3, "%d", irlist->oprnd3);
	}



        /* add IR to irlist */
        fprintf(irout,"irnode_sid: %3d, code: %18s (%d), symptr: %10s, oprnd1: %5s,     oprnd2: %5s,    oprnd3: %5s\n",
                irlist->sid,
		ircodenames[irlist->ircode],
		irlist->ircode,
		print_declarator_id(irlist->symptr),
		oprnd1,
		oprnd2,
		oprnd3
	      );
        } /* end if */
        irlist= irlist->next;
    }/* end while */
}











/*-----------------------------------------------
 | print_declarator_id
 +---------------------------------------------*/
char *print_declarator_id(struct declarator *sym)
{
    if(sym == NULL)
    {   return "NULL";
    }

    if(sym->nodetype == ARRAY_DECLARATOR || sym->nodetype == FUNCTION_DECLARATOR)
    {   sym= sym->adeclarator;
    }
    else if(sym->nodetype == POINTER_DECLARATOR)  /* ffwd to appropriate declarator */
    {   while(sym->next)
        {   sym= sym->next;
        }
        if(!sym->id)
        {   sym= sym->adeclarator;
        }
    }

    return sym->id;
}






/*-----------------------------------------------
 | compound_to_ir
 +---------------------------------------------*/
void compound_to_ir(struct ast *cstmt)
{   char genstr[TMPSTRSZ];
    struct ast *decostat;
    struct decostat_list *dlist= (struct decostat_list *) cstmt->l;


    sprintf(&genstr[strlen(genstr)],"#");
    indent(genstr);
    sprintf(&genstr[strlen(genstr)],"{\n");
    fprintf(irout,"%s",genstr); clearstr(genstr);

    ++indent_count;
    do
    {   decostat= dlist->decostat;


        /* Generate IR node
        +--------------------*/
	regnum=0;   /* reset register count to 0 */
	printf("c2ir calling d2ir...\n");
        decostat_to_ir(decostat);


        /* write C statement to file
	+----------------------------*/
        clearstr(genstr);
        sprintf(genstr,"#");
        indent(genstr);
	if(decostat->nodetype == COMPOUND_STATEMENT)
	{   clearstr(genstr);
	    compound_to_ir(decostat);
	}
	else
        {   print_expr(dlist->decostat,genstr);
	}

        /* no semicolons after compound statements */
        if(   (dlist->decostat->nodetype == COMPOUND_STATEMENT)      ||
              ( (dlist->decostat->nodetype == LABELED_STATEMENT)     &&
                (dlist->decostat->r->nodetype == COMPOUND_STATEMENT)
              )
          )
        {   fprintf(irout,"\n\n");
        }
        else
        {   fprintf(irout,"%s;\n",genstr); clearstr(genstr);
        }
        fprintf(irout,"%s",genstr); clearstr(genstr);




        /* write IR nodes to file
	+----------------------------*/
	statement_end_sid= irnodenum;
	print_irnode_sids(statement_begin_sid,statement_end_sid);
	statement_begin_sid= ++statement_end_sid;
	fprintf(irout,"\n\n\n");


    } while( (dlist= dlist->next) != NULL);
    --indent_count;


    sprintf(&genstr[strlen(genstr)],"#");
    indent(genstr);
    sprintf(&genstr[strlen(genstr)],"}\n");
    fprintf(irout,"%s",genstr); clearstr(genstr);
}






/*-----------------------------------------------
 | decostat_to_ir - converts each statement to IR
 +---------------------------------------------*/
void decostat_to_ir(struct ast *decostat)
{
    struct ast *dstat;
    struct decostat_list *dlist= (struct decostat_list *)decostat;
    struct irinfo *prepl;
    struct irinfo *prepr;
    int i;


    printf("decostat_to_ir() invoked with decostat type: %d \n", decostat->nodetype);


    switch(decostat->nodetype)
    {   case DECL:
           printf("found a decl...\n\n\n");
           /* do nothing */
	   break;


        case DECOSTAT_LIST:
	   printf("found a decostat_list...\n\n\n");
	   i=0;
	   dlist= (struct decostat_list *)decostat;
	   do
	   {   dstat= dlist->decostat;
	       printf("calling decostat_to_ir from decostat_to_ir\n");
	       decostat_to_ir(dstat);
	       printf("\t\t\t----> %d\n", ++i);
	   }while( (dlist= dlist->next) != NULL);
	   break;


        case OP_ASSIGNMENT:
	   printf("found an OP_ASSIGNMENT...\n\n\n");
	   prepl= typecheck(decostat->l);
	   prepr= typecheck(decostat->r);

	   if((prepl->nodetype == LVALUE) && (prepr->nodetype == RVALUE))
	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

	       /* add node for storewordindirect */
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= STOREWORDINDIRECT;
               irlist->oprnd1= prepr->regnum;
	       irlist->oprnd2= prepl->regnum;
           }
	   else if((prepl->nodetype == LVALUE) && (prepr->nodetype == LVALUE))
	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }


	       /* add nodes for load/storewordindirect */
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= LOADWORDINDIRECT;
               irlist->oprnd1= ++regnum;
	       irlist->oprnd2= prepr->regnum;

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= STOREWORDINDIRECT;
               irlist->oprnd1=  regnum;
	       irlist->oprnd2=  prepl->regnum;

           }

	   printf(" left type: %d\n", prepl->nodetype);
	   printf("right type: %d\n", prepr->nodetype);
	   break;


        case INTEGER_CONSTANT:
	   printf("found an integer constant...\n\n\n");
	   break;


        default:
	   printf("default\n");
	   break;
    }
}






struct irinfo *typecheck(struct ast *subtree)
{

    if(subtree == NULL)
    {   printf("received a null subtree...\n");
        return NULL;
    }

    struct irinfo *tcresult=  emalloc(sizeof(struct irinfo));


    /* figure out what type of node i'm looking at */
    printf("\ttypecheck called with decostat type: %d\n", subtree->nodetype);


    if(subtree->nodetype == INTEGER_CONSTANT)
    {   tcresult->nodetype= RVALUE;
        tcresult->regnum= ++regnum;

        irlist= irlist_front;
        while(irlist->next != NULL)
        {   irlist= irlist->next;
        }
        irlist->next= emalloc(sizeof(struct irnode));
	irlist->next->prev= irlist;
        irlist= irlist->next;
        irlist->sid= ++irnodenum;
        irlist->ircode= LOADCONSTANT;
        irlist->oprnd1= tcresult->regnum;
	irlist->oprnd2= (int)(long)((struct constant *)subtree)->value;

        return tcresult;
    }

    else if(subtree->nodetype == SIMPLE_DECLARATOR)
    {   tcresult->nodetype= LVALUE;
        tcresult->regnum= ++regnum;

        irlist= irlist_front;
        while(irlist->next != NULL)
        {   irlist= irlist->next;
        }
        irlist->next= emalloc(sizeof(struct irnode));
	irlist->next->prev= irlist;
        irlist= irlist->next;
        irlist->sid= ++irnodenum;
        irlist->ircode= LOADADDRESS;
        irlist->oprnd1= tcresult->regnum;
        irlist->symptr= (struct declarator *)subtree;

        return tcresult;
    }


/*

    if((subtree->l != NULL)  &&  (subtree->l->nodetype != INTEGER_CONSTANT))
    {   typecheck(subtree->l);
    }

    if((subtree->r != NULL)  &&  (subtree->r->nodetype != INTEGER_CONSTANT))
    {   typecheck(subtree->r);
    }

*/

    return tcresult;
}






