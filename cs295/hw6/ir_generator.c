/*============================================================================
| File:		ir_generator.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the parse tree and generates IR code.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#define SYSCALL_REG 9
#define SYSCALL_ARG_REG 10

int statement_begin_sid=0;
int statement_end_sid=0;

extern char *reglist[];


int context_clue=0;
int forjump=0;
int stringvarnum=0;


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
        ircodenames[106]= "PRINTINT";
        ircodenames[107]= "SYSCALL";
        ircodenames[108]= "LOADWORD";
        ircodenames[109]= "MIPSLABEL";
        ircodenames[110]= "BRANCH_GT";
        ircodenames[111]= "BRANCH_LT";
        ircodenames[112]= "JUMP";
	ircodenames[113]= "COMMENT";
	ircodenames[114]= "ADD";
	ircodenames[115]= "ADD1";
	ircodenames[116]= "PRINTSTRING";
	ircodenames[117]= "CREATE_STRINGVAR";
	ircodenames[118]= "READINT";
	ircodenames[119]= "BRANCH_GTE";
	ircodenames[120]= "BRANCH_LTE";
	ircodenames[121]= "BRANCH_EQ";
	ircodenames[122]= "REMAINDER";
	ircodenames[123]= "SUBTRACT";
	ircodenames[124]= "SUB1";
	ircodenames[125]= "FUNCTIONCALL";
	ircodenames[126]= "MULTIPLY";
	ircodenames[127]= "DIVIDE";
    };



    /* initialize irlist_front and irlist.
    +---------------------------------------*/
    irlist_front= emalloc(sizeof(struct irnode));
    irlist_front->prev=NULL;
    irlist_front->next=NULL;
    irlist_front->sid= ++irnodenum;
    irlist= irlist_front;



    /* initialize label num, used to avoid label name
    |  collisions.
    +------------------------------------------------*/
    labelnum=0;



    /* Open IR output file
    +-------------------------*/
    strcpy(irfname,"out.ir");  /* update later to create an IR file
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


            printf("\n\n\nDEBUG: generate_mips() encountered a function definition: %s\n", print_declarator_id(d));


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

	    if(irnodenum != 1)
	    {
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
	    }

            printf("\tDEBUG: current irnode sid: %d\n", irlist->sid);

	    irlist->ircode= BEGINPROC;
	    irlist->symptr= fdspec->d;
	    irlist->sid= (irnodenum == 1) ? 1 : ++irnodenum;


	    sprintf(&genstr[strlen(genstr)]," %s(", funcname);
	    fprintf(irout,"%s", genstr); clearstr(genstr);



            /* print parameter list
	    +-------------------------*/
	    print_parameter_list(plist,genstr);
	    sprintf(&genstr[strlen(genstr)],")");
	    sprintf(&genstr[strlen(genstr)],"\n");
	    fprintf(irout,"%s",genstr); clearstr(genstr);


	    /* print IR for entering function
	    +----------------------------------*/
            statement_begin_sid= irnodenum + 1;

	    print_irnode_sids(irlist->sid,irlist->sid); /* DEBUG */
	    fprintf(irout,"\n\n");


            /* Generate IR for compound statement block
            +-------------------------------------------*/
	    compound_to_ir(cstmt);



            /* Generate endproc node for function.
	    +---------------------------------------*/
	    printf("DEBUG: about to add ENDPROC node for function '%s'\n", print_declarator_id(irlist->symptr));
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
	    print_irnode_sids(irlist->sid,irlist->sid); /* DEBUG */



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
        if((irlist->oprnd2 == 0) &&  (irlist->ircode == LOADCONSTANT))
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
        printf("irnode_sid: %3d, code: %18s (%d), symptr: %10s, oprnd1: %5s,     oprnd2: %5s,    oprnd3: %5s, label: %s\n",
                irlist->sid,
		ircodenames[irlist->ircode],
		irlist->ircode,
		(irlist->ircode != CREATE_STRINGVAR) ? print_declarator_id(irlist->symptr) : "N/A",
		oprnd1,
		oprnd2,
		oprnd3,
		irlist->label
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
    printf("#         IRNODE Generation Notes\n");
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
	{   sprintf(oprnd1, "%s", reglist[irlist->oprnd1]);
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
        if((irlist->oprnd2 == 0) &&  (irlist->ircode == LOADCONSTANT))
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
        fprintf(irout,"irnode_sid: %3d, code: %18s (%d), symptr: %10s, oprnd1: %5s,     oprnd2: %5s,    oprnd3: %5s,  label: %s\n",
                irlist->sid,
		ircodenames[irlist->ircode],
		irlist->ircode,
		(irlist->ircode != CREATE_STRINGVAR) ? print_declarator_id(irlist->symptr) : "N/A",
		oprnd1,
		oprnd2,
		oprnd3,
		irlist->label
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
    struct flow *tflow;
    struct ast *cond;
    struct ast *thendo;



    printf("DEBUG: compound_to_ir called with nodetype: %d\n", cstmt->nodetype);

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
	printf("c2ir calling e2ir...\n");
        expr_to_ir(decostat);


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
    sprintf(&genstr[strlen(genstr)],"}\n\n\n");
    fprintf(irout,"%s",genstr); clearstr(genstr);
}








/*-----------------------------------------------
 | expr_to_ir
 +---------------------------------------------*/
struct irinfo *expr_to_ir(struct ast *subtree)
{
    struct irinfo *left;
    struct irinfo *right;

    /* vars for dealing with loops */
    struct flow *tflow;
    struct ast *cond;
    struct ast *thendo;
    struct ast *elsedo;
    struct ast *forinit;
    struct ast *forupdate;

    char elselabel[25]= "else";
    char thenlabel[25]= "then";
    char endlabel[25]= "end";
    char testcondlabel[25]= "test";

    struct ast *dstat;
    struct decostat_list *dlist;
    int i;
    int tmpreg;

    struct irinfo *tcresult=  emalloc(sizeof(struct irinfo));



    if(subtree == NULL)
    {   printf("received a null subtree...\n");
        return NULL;
    }
    else
    {   printf("DEBUG: expr_to_ir: ENTERED with NON NULL subtree...\n");
    }


    /* figure out what type of node i'm looking at */
    printf("\texpr_to_ir called with expr type: %d\n", subtree->nodetype);



    switch(subtree->nodetype)
    {
        case DECL:
	   break;


        case DECOSTAT_LIST:
	   printf("found a decostat_list...\n\n\n");
	   i=0;
	   dlist= (struct decostat_list *)subtree;
	   do
	   {   dstat= dlist->decostat;
	       printf("calling expr_to_ir from expr_to_ir\n");
	       left= expr_to_ir(dstat);
	       printf("dstat_list_element_%d\n", ++i);
	   }while( (dlist= dlist->next) != NULL);

	   tcresult->nodetype= left->nodetype;
	   tcresult->regnum= left->regnum;

	   return tcresult;
	   break;


        case PARENTHESIZED_EXPR:
	   printf("found parenthsized expr... type of subtree->l: %d\n\n\n", subtree->l->nodetype);

	   left= expr_to_ir(subtree->l);

	   tcresult->nodetype= left->nodetype;
	   tcresult->regnum= left->regnum;

	   printf("DEBUG: PARENTHESIZED_EXPR returning tcresult nodetype %d and regnum %d\n",
	          tcresult->nodetype,
		  tcresult->regnum
                 );

	   return tcresult;
	   break;


        case INTEGER_CONSTANT:
           tcresult->nodetype= RVALUE;
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

	   printf("DEBUG: loading constant: %d\n", (int)(long)((struct constant *)subtree)->value);
	   irlist->oprnd2= (int)(long)((struct constant *)subtree)->value;

           return tcresult;
	   break;




        case SIMPLE_DECLARATOR:
           tcresult->nodetype= LVALUE;
           tcresult->regnum= ++regnum;

	   printf("DEBUG: dealing with simple declarator, context clue is: %d\n", context_clue);

           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;

	   if(context_clue != 0)
	   {   irlist->ircode= LOADWORD;
	       printf("DEBUG: since context clue is %d, doing loadword...\n", context_clue);
	   }
	   else
	   {   irlist->ircode= LOADADDRESS;
	       printf("DEBUG: since context clue is %d, doing loadaddress...\n", context_clue);
	   }
           irlist->oprnd1= tcresult->regnum;
           irlist->symptr= (struct declarator *)subtree;

           return tcresult;
           break;




        case OP_ASSIGNMENT:
	   printf("DEBUG: found an OP_ASSIGNMENT...\n\n\n");
	   left= expr_to_ir(subtree->l);
	   right= expr_to_ir(subtree->r);


	   if((left->nodetype == LVALUE) && (right->nodetype == RVALUE))
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
               irlist->oprnd1= right->regnum;
	       irlist->oprnd2= left->regnum;
           }
	   else if((left->nodetype == LVALUE) && (right->nodetype == LVALUE))
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
	       irlist->oprnd2= right->regnum;

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= STOREWORDINDIRECT;
               irlist->oprnd1=  regnum;
	       irlist->oprnd2=  left->regnum;

           }

	   break;



        case PLUS_SIGN:
	   context_clue= PLUS_SIGN;
	   printf("DEBUG: found an PLUS_SIGN...\n\n\n");
	   left= expr_to_ir(subtree->l);
	   right= expr_to_ir(subtree->r);

	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= ADD;
               irlist->oprnd1= ++regnum;
	       irlist->oprnd2= left->regnum;
               irlist->oprnd3= right->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;

           }
	   context_clue= 0;
	   return tcresult;
           break;




        case MINUS_SIGN:
           context_clue= MINUS_SIGN;
           printf("DEBUG: found a MINUS_SIGN...\n\n\n");
           left= expr_to_ir(subtree->l);
           right= expr_to_ir(subtree->r);

           {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
               irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= SUBTRACT;
               irlist->oprnd1= ++regnum;
               irlist->oprnd2= left->regnum;
               irlist->oprnd3= right->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;

           }
           context_clue= 0;
           return tcresult;
           break;




        case ASTERISK:
           context_clue= ASTERISK;
           printf("DEBUG: found an ASTERISK...\n\n\n");
           left= expr_to_ir(subtree->l);
           right= expr_to_ir(subtree->r);

           {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
               irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= MULTIPLY;
               irlist->oprnd1= ++regnum;
               irlist->oprnd2= left->regnum;
               irlist->oprnd3= right->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;
	       printf("DEBUG: result is in reg %d\n", tcresult->regnum);

           }
           context_clue= 0;
           return tcresult;
           break;





        case OP_DIVISION:
           context_clue= OP_DIVISION;
           printf("DEBUG: found an OP_DIVISION...\n\n\n");
           left= expr_to_ir(subtree->l);
           right= expr_to_ir(subtree->r);

           {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
               irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= DIVIDE;
               irlist->oprnd1= ++regnum;
               irlist->oprnd2= left->regnum;
               irlist->oprnd3= right->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;

           }
           context_clue= 0;
           return tcresult;
           break;





        case OP_REMAINDER:
	   context_clue= OP_REMAINDER;
	   printf("DEBUG: found an OP_REMAINDER...\n\n\n");
	   left= expr_to_ir(subtree->l);
	   right= expr_to_ir(subtree->r);

	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= REMAINDER;
               irlist->oprnd1= ++regnum;
	       irlist->oprnd2= left->regnum;
               irlist->oprnd3= right->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;

           }
	   context_clue= 0;
	   return tcresult;
           break;




        case POSTINCREMENT_EXPR:
	   context_clue= POSTINCREMENT_EXPR;
	   printf("DEBUG: found a POSTINCREMENT_EXPR...\n\n\n");
	   printf("\tDEBUG: subtree->l type: %d\n", subtree->l->nodetype);
	   left= expr_to_ir(subtree->l);

	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= ADD1;
	       tmpreg= ++regnum;
               irlist->oprnd1= tmpreg;
	       irlist->oprnd2= left->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;
	       context_clue= 0;


               /* store result */
	       left= expr_to_ir(subtree->l);
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= STOREWORDINDIRECT;
               irlist->oprnd1= tmpreg;
	       irlist->oprnd2= left->regnum;

           }
	   return tcresult;
           break;





        case POSTDECREMENT_EXPR:
	   context_clue= POSTDECREMENT_EXPR;
	   printf("DEBUG: found a POSTDECREMENT_EXPR...\n\n\n");
	   printf("\tDEBUG: subtree->l type: %d\n", subtree->l->nodetype);
	   left= expr_to_ir(subtree->l);

	   {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= SUB1;
	       tmpreg= ++regnum;
               irlist->oprnd1= tmpreg;
	       irlist->oprnd2= left->regnum;

               tcresult->nodetype= RVALUE;
               tcresult->regnum= irlist->oprnd1;
	       context_clue= 0;


               /* store result */
	       left= expr_to_ir(subtree->l);
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= STOREWORDINDIRECT;
               irlist->oprnd1= tmpreg;
	       irlist->oprnd2= left->regnum;

           }
	   return tcresult;
           break;





        case IF_STATEMENT:
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

           /* retrieve if statement components
	   +-----------------------------------*/
           tflow= (struct flow *)subtree;
           cond= tflow->cond;
           thendo= tflow->thendo;

	  
	    /* print C code to IR file.
	    +---------------------------*/
	    printf("DEBUG: cond type: %d\n", cond->nodetype);
	    printf("DEBUG: thendo type: %d\n", thendo->nodetype);
	    clearstr(tmpstr);
	    sprintf(tmpstr,"#");
	    indent(tmpstr);
	    sprintf(&tmpstr[strlen(tmpstr)], "if(");
	    print_expr(cond->l,tmpstr);
	    sprintf(&tmpstr[strlen(tmpstr)], ")\n");
	    fprintf(irout,tmpstr); clearstr(tmpstr);


           /* Generate necessary labels
	   +----------------------------*/
	   sprintf(&elselabel[strlen(elselabel)], "%d", ++labelnum);
	   sprintf(&endlabel[strlen(endlabel)], "%d", ++labelnum);


           printf("\t\tDEBUG: type of condition: %d\n", cond->l->nodetype);
           printf("\t\tDEBUG: type of thendo: %d\n", thendo->nodetype);



	   /* create node for conditional statement
	   +-----------------------------------------*/
	   expr_to_ir(cond->l);
	   sprintf(&thenlabel[strlen(thenlabel)], "%d", labelnum);


	   /* create node for empty else block
	   +-----------------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, elselabel);

           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, endlabel);



           /* create nodes for then block */
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, thenlabel);
	   expr_to_ir(thendo);



           /* create node for end block
	   +-----------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, endlabel);


           break;









        case IF_ELSE_STATEMENT:
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

           /* retrieve if statement components
	   +-----------------------------------*/
           tflow= (struct flow *)subtree;
           cond= tflow->cond;
           thendo= tflow->thendo;
	   elsedo= tflow->elsedo;


           /* Generate necessary labels
	   +----------------------------*/
	   sprintf(&elselabel[strlen(elselabel)], "%d", ++labelnum);
	   sprintf(&endlabel[strlen(endlabel)], "%d", ++labelnum);


           printf("\t\tDEBUG: type of condition: %d\n", cond->l->nodetype);
           printf("\t\tDEBUG: type of thendo: %d\n", thendo->nodetype);


	   /* create node for conditional statement
	   +-----------------------------------------*/
	   expr_to_ir(cond->l);
	   sprintf(&thenlabel[strlen(thenlabel)], "%d", labelnum);


	   /* create nodes for else block
	   +------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, elselabel);

	   expr_to_ir(elsedo);

           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, endlabel);



           /* create nodes for then block */
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, thenlabel);
	   expr_to_ir(thendo);



           /* create node for end block
	   +-----------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, endlabel);


           break;






        case FOR_STATEMENT:
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

           /* retrieve for statement components
	   +-----------------------------------*/
           tflow     = (struct flow *)subtree;
	   forinit   = tflow->forinit;
           cond      = tflow->cond;
	   forupdate = tflow->forupdate;
           thendo    = tflow->thendo;


           /* Generate necessary labels
	   +----------------------------*/
	   sprintf(elselabel, "forelse%d", ++labelnum);
	   sprintf(&thenlabel[strlen(thenlabel)], "%d", ++labelnum);
	   sprintf(&endlabel[strlen(endlabel)], "%d", ++labelnum);
	   sprintf(&testcondlabel[strlen(testcondlabel)], "%d", ++labelnum);

           /*
           printf("\t\tDEBUG: type of forinit: %d\n", forinit->l->nodetype);
           printf("\t\tDEBUG: type of condition: %d\n", cond->l->nodetype);
           printf("\t\tDEBUG: type of thendo: %d\n", thendo->nodetype);
           printf("\t\tDEBUG: type of forupdate: %d\n", forupdate->l->nodetype);
	   */


	   /* create nodes for init statement
	   +-----------------------------------------*/
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= COMMENT;
               strncpy(irlist->label, "For Loop Initialization",49);

	       if(forinit != NULL)
	       {   expr_to_ir(forinit->l);
	       }



	   /* create nodes for conditional statement
	   +-----------------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= COMMENT;
           strncpy(irlist->label, "For Loop Test Condition",49);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, testcondlabel);

           forjump= atoi(&thenlabel[strlen(thenlabel)-1]);
	   if(cond != NULL)
	   {   expr_to_ir(cond->l);
	   }
	   forjump= 0;



	   /* create node for empty else block
	   +-----------------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, elselabel);

           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, endlabel);



           /* create nodes for then block
	   +--------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, thenlabel);
	   if(thendo != NULL)
	   {   expr_to_ir(thendo);
	   }



           /* create nodes for for loop update
	   +-----------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= COMMENT;
           strncpy(irlist->label, "For Loop Update Statement",49);

           if(forupdate != NULL)
	   {   expr_to_ir(forupdate->l);
	   }



           /* jump back to test condition
	   +-------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, testcondlabel);



           /* create node for end block
	   +-----------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, endlabel);

           break;






        case WHILE_STATEMENT:
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

           /* retrieve for statement components
	   +-----------------------------------*/
           tflow     = (struct flow *)subtree;
           cond      = tflow->cond;
           thendo    = tflow->thendo;



           /* Generate necessary labels
	   +----------------------------*/
	   sprintf(elselabel, "whileelse%d", ++labelnum);
	   sprintf(&thenlabel[strlen(thenlabel)], "%d", ++labelnum);
	   sprintf(&endlabel[strlen(endlabel)], "%d", ++labelnum);
	   sprintf(&testcondlabel[strlen(testcondlabel)], "%d", ++labelnum);



	   /* create nodes for conditional statement
	   +-----------------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= COMMENT;
           strncpy(irlist->label, "WHILE Loop Test Condition",49);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, testcondlabel);

           forjump= atoi(&thenlabel[strlen(thenlabel)-1]);
	   if(cond != NULL)
	   {   expr_to_ir(cond->l);
	   }
	   forjump= 0;



	   /* create node for empty else block
	   +-----------------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, elselabel);

           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, endlabel);



           /* create nodes for then block
	   +--------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, thenlabel);
	   if(thendo != NULL)
	   {   expr_to_ir(thendo);
	   }



           /* jump back to test condition
	   +-------------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= JUMP;
           strcpy(irlist->label, testcondlabel);



           /* create node for end block
	   +-----------------------------*/
           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= MIPSLABEL;
           strcpy(irlist->label, endlabel);

           break;






        case RW_BREAK:
           printf("DEBUG: found a RW_BREAK...\n\n\n");
           ;
           char tlabel[50];

           {   irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               while( (strstr(irlist->label,"forelse")) == NULL  &&   (irlist != NULL))
               {   irlist=irlist->prev;
                   printf("rewinding from irnode %d\n", irlist->sid);
               }
               if(irlist != NULL)
               {   strcpy(tlabel,irlist->label);
                   printf("tlabel set to: %s\n", tlabel);
               }




               irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

               irlist->next= emalloc(sizeof(struct irnode));
               irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= JUMP;
               strcpy(irlist->label,tlabel);
          }
          break;






        case OP_EQUALITY:
           printf("DEBUG: found a OP_EQUALITY...\n\n\n");
	   context_clue= OP_EQUALITY;
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

	   left   = expr_to_ir(subtree->l);
	   right  = expr_to_ir(subtree->r);

	   printf("DEBUG: expr_to_ir(): left regnum: %d\n", left->regnum);
	   printf("DEBUG: expr_to_ir(): right regnum: %d\n", right->regnum);

           char eqthenlabel[25]= "then";
	   sprintf(&eqthenlabel[strlen(eqthenlabel)], "%d", ++labelnum);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= BRANCH_EQ;
           irlist->oprnd1= left->regnum;
           irlist->oprnd2= right->regnum;
           strcpy(irlist->label, eqthenlabel);
	   context_clue= 0;

           tcresult->nodetype= RVALUE;
           tcresult->regnum= irlist->oprnd1;

	   return tcresult;
	   break;





        case GREATER_THAN_SYMBOL:
	   context_clue= CONDITIONAL_STATEMENT;
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

	   printf("DEBUG: expr_to_ir(): received a greater than symbol....\n");
	   left   = expr_to_ir(subtree->l);
	   right  = expr_to_ir(subtree->r);

           char gtthenlabel[25]= "then";
	   sprintf(&gtthenlabel[strlen(gtthenlabel)], "%d", (forjump > 0)? forjump : ++labelnum);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= BRANCH_GT;
           irlist->oprnd1= left->regnum;
           irlist->oprnd2= right->regnum;
           strcpy(irlist->label, gtthenlabel);
	   context_clue= 0;
	   break;






        case OP_RELATIONAL_GTE:
	   context_clue= CONDITIONAL_STATEMENT;
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

	   printf("DEBUG: expr_to_ir(): received a greater than symbol....\n");
	   left   = expr_to_ir(subtree->l);
	   right  = expr_to_ir(subtree->r);

           char gtethenlabel[25]= "then";
	   sprintf(&gtethenlabel[strlen(gtethenlabel)], "%d", (forjump > 0)? forjump : ++labelnum);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= BRANCH_GTE;
           irlist->oprnd1= left->regnum;
           irlist->oprnd2= right->regnum;
           strcpy(irlist->label, gtethenlabel);
	   context_clue= 0;
	   break;






        case OP_RELATIONAL_LT:
	   context_clue= OP_RELATIONAL_LT;
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

	   printf("DEBUG: expr_to_ir(): received a greater than symbol....\n");
	   left   = expr_to_ir(subtree->l);
	   right  = expr_to_ir(subtree->r);

           char ltthenlabel[25]= "then";
	   sprintf(&ltthenlabel[strlen(ltthenlabel)], "%d", (forjump > 0)? forjump : ++labelnum);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= BRANCH_LT;
           irlist->oprnd1= left->regnum;
           irlist->oprnd2= right->regnum;
           strcpy(irlist->label, ltthenlabel);
	   context_clue= 0;
	   break;








        case OP_RELATIONAL_LTE:
	   context_clue= OP_RELATIONAL_LTE;
           irlist= irlist_front;
           while(irlist->next != NULL)
           {   irlist= irlist->next;
           }

	   printf("DEBUG: expr_to_ir(): received a greater than symbol....\n");
	   left   = expr_to_ir(subtree->l);
	   right  = expr_to_ir(subtree->r);

           char ltethenlabel[25]= "then";
	   sprintf(&ltethenlabel[strlen(ltethenlabel)], "%d", (forjump > 0)? forjump : ++labelnum);


           irlist->next= emalloc(sizeof(struct irnode));
	   irlist->next->prev= irlist;
           irlist= irlist->next;
           irlist->sid= ++irnodenum;
           irlist->ircode= BRANCH_LTE;
           irlist->oprnd1= left->regnum;
           irlist->oprnd2= right->regnum;
           strcpy(irlist->label, ltethenlabel);
	   context_clue= 0;
	   break;





        case COMPOUND_STATEMENT:
	   printf("DEBUG: expr_to_ir() called with compound statement  ...\n");
	   printf("\tDEBUG: subtree type: %d\n", subtree->nodetype);
	   compound_to_ir(subtree);
	   break;





        case FUNCTION_CALL:
	   ;
	   char funcname[40];
	   struct declarator *d= (struct declarator *)subtree->l;
	   struct parameter_list *plist;
	   plist=d->plist;
           if(d->nodetype == SIMPLE_DECLARATOR)
           {   strcpy(funcname,d->id);
           }
           else
           {   strcpy(funcname,d->adeclarator->id);
           }
	   printf("DEBUG: expr_to_ir FUNCTION CALL: %s\n", funcname);


	   /* If this is a built-in function...
	   +-----------------------------------------*/
	   if( (!strcmp(funcname,"printint"))      ||
               (!strcmp(funcname,"printstring"))   ||
               (!strcmp(funcname,"readint"))
	     )
	   {
	       /* printint function...
	       +-------------------------*/
	       if(! strcmp(funcname,"printint"))
	       {   printf("printint invoked...\n");
	           irlist= irlist_front;
                   while(irlist->next != NULL)
                   {   irlist= irlist->next;
                   }

	           /* add nodes for printint */
                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= PRINTINT;

                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= LOADWORD;
                   irlist->oprnd1= SYSCALL_ARG_REG;
	           irlist->symptr= (struct declarator *)subtree->r->l;

                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= SYSCALL;
	       }


	       /* printstring function...
	       +--------------------------*/
	       if(! strcmp(funcname,"printstring"))
	       {
	           /* Retrieve parameter
		   +----------------------*/
		   struct constant *k= (struct constant *)subtree->r->l;
		   char *param= k->value;
		   printf("param: %s (%ld)\n", param, param);

	           irlist= irlist_front;
                   while(irlist->next != NULL)
                   {   irlist= irlist->next;
                   }


	           /* add nodes for printstring */
                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= PRINTSTRING;
		   irlist->oprnd1= ++regnum;


                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= CREATE_STRINGVAR;
		   irlist->symptr= (struct declarator *)param;
		   sprintf(irlist->label,"stringvar%d", ++stringvarnum);


                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= SYSCALL;

               }





	       /* readint function...
	       +--------------------------*/
	       if(! strcmp(funcname,"readint"))
	       {

	           irlist= irlist_front;
                   while(irlist->next != NULL)
                   {   irlist= irlist->next;
                   }

	           /* add nodes for readint */
                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= READINT;

                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= SYSCALL;

	           /* add nodes for result storage */
                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= LOADADDRESS;
                   irlist->oprnd1= ++regnum;
	           irlist->symptr= (struct declarator *)subtree->r->l;

                   irlist->next= emalloc(sizeof(struct irnode));
	           irlist->next->prev= irlist;
                   irlist= irlist->next;
                   irlist->sid= ++irnodenum;
                   irlist->ircode= STOREWORDINDIRECT;
                   irlist->oprnd1= SYSCALL_REG;
                   irlist->oprnd2= regnum;
	           irlist->symptr= (struct declarator *)subtree->r->l;

               }
	   }




	   /* If it's not a builtin function
	   +---------------------------------*/
	   else
	   {   printf("function '%s' has been called...\n",funcname);
	       irlist= irlist_front;
               while(irlist->next != NULL)
               {   irlist= irlist->next;
               }

	       /* add nodes for printint */
               irlist->next= emalloc(sizeof(struct irnode));
	       irlist->next->prev= irlist;
               irlist= irlist->next;
               irlist->sid= ++irnodenum;
               irlist->ircode= FUNCTIONCALL;
	       strcpy(irlist->label,"_VAR_");
	       strcat(irlist->label,funcname);
	       irlist->symptr= d;

           }

           break;






        default:
	   printf("DEBUG: expr_to_ir: NOT SURE WHAT TO DO WITH TYPE: %d\n", subtree->nodetype);
	   break;
    }


    return tcresult;
}






