/*============================================================================
| File:		generate_ir.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the parse tree and generates IR code.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/




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
        ircodenames[101]= "LoaDADDRESS";
        ircodenames[102]= "STOREWORDINDIRECT";
        ircodenames[103]= "ENDPROC";
    };



    /* initialize irlist_front and irlist.
    +---------------------------------------*/
    irlist_front= emalloc(sizeof(struct irnode));
    irlist_front->prev=NULL;
    irlist_front->next=NULL;
    irlist_front->sid= ++irnodenum;
    irlist= irlist_front;



    printf("irlist ircode: %d\n", irlist->ircode);



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
	    printf("symtab_name: %s\n", symtab_name);
	    


	    /* switch to appropriate symbol table
	    +--------------------------------------*/
	    curr_symtab= curr_symtab->child;


	    /* Generate IR node for function
	    +----------------------------------*/
	    struct declarator *id= new_simple_declarator(funcname);

	    while(strstr(curr_symtab->id,symtab_name) == NULL )
	    {   curr_symtab= curr_symtab->rsibling;
	    }
	    printf("DEBUG: moved to symtab: %s\n", curr_symtab->id);

	    irlist= irlist_front;
	    while(irlist->next != NULL)
	    {   irlist= irlist->next;
	    }

            if(irlist->sid == 1)
	    {   irlist->ircode= BEGINPROC;
	        irlist->symptr= resolve(id,curr_symtab);
	    }
	    printf("location of irlist->symptr: %ld\n", irlist->symptr);


	    sprintf(&genstr[strlen(genstr)]," %s(", funcname);
	    fprintf(irout,"%s", genstr); clearstr(genstr);




            /* print parameter list
	    +-------------------------*/
	    print_parameter_list(plist,genstr);
	    sprintf(&genstr[strlen(genstr)],")");
	    sprintf(&genstr[strlen(genstr)],"\n#{\n\n\n");
	    fprintf(irout,"%s",genstr); clearstr(genstr);




            /* display compound statement block
            +-------------------------------------*/
            dlist= (struct decostat_list *) cstmt->l;
            ++indent_count;
            do
            {   decostat= dlist->decostat;


	        /* Generate IR node
		+-------------------------*/
		printf("decostat node type: %d\n", decostat->l->nodetype);


		/*
		switch(decostat->nodetype):
		{   case: 
		}
		*/








	        /* write IR to file
		+-------------------------*/
	        clearstr(genstr);
	        sprintf(genstr,"#");
	        indent(genstr);
                print_expr(dlist->decostat,genstr);

                /* no semicolons after compound statements */
                if(   (dlist->decostat->nodetype == COMPOUND_STATEMENT)      ||
                      ( (dlist->decostat->nodetype == LABELED_STATEMENT)     &&
                        (dlist->decostat->r->nodetype == COMPOUND_STATEMENT)
                      )
                  )
                {   fprintf(irout,"\n\n");
                }
                else
                {   fprintf(irout,"%s;\n\n\n",genstr); clearstr(genstr);
                }
                fprintf(irout,"%s",genstr); clearstr(genstr);
            } while( (dlist= dlist->next) != NULL);
            --indent_count;


            fprintf(irout,"\n#}\n\n");

	}

    }while( (tldlist= tldlist->next) != NULL );


    irlist= irlist_front;
    while(irlist->next != NULL)
    {   irlist= irlist->next;
    }
    irlist->next= emalloc(sizeof(struct irnode));
    irlist= irlist->next;
    irlist->sid= ++irnodenum;
    irlist->ircode= ENDPROC;



    fclose(irout);
    print_irnodes();
}





void print_irnodes(void)
{   struct irnode *irlist= irlist_front;

    printf("\n\n\n");
    printf("#=========================================\n");
    printf("#         IRNODE LIST DISPLAY\n");
    printf("#=========================================\n\n");


    if(irlist == NULL)
    {   printf("IRLIST IS EMPTY\n\n");
        return;
    }

    while(irlist != NULL)
    {   printf("irnode_sid: %3d, code: %s (%d), symptr: %s\n", irlist->sid, ircodenames[irlist->ircode],irlist->ircode, print_declarator_id(irlist->symptr));
        irlist= irlist->next;
    }
}




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



