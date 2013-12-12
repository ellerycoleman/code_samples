/*============================================================================
| File:		mips_generator.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the irnode list and emits the appropriate MIPS
|               assembly language instructions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/




/*-----------------------------------------------
 | generate_mips
 +---------------------------------------------*/
void generate_mips(void)
{

    printf("\n\n\n\n\n");
    printf("===========================================================\n");
    printf(" Generate MIPS called...\n");
    printf("===========================================================\n");
    irlist= irlist_front;





    /* Open MIPS output file
    +-------------------------*/
    strcpy(mipsfname,"stdin.mips");  /* update later to create an IR file
                                    based on input filename. */
    mipsout= fopen(mipsfname,"w+");
    if(irout == NULL)
    {   printf("Error opening IR output file: %s\n", mipsfname);
    }


    if(irlist == NULL)
    {   printf("IRLIST IS EMPTY\n\n");
        return;
    }



    /* Check top_level_symbol table for all
    |  non-function symbols.  Emit the appropriate
    |  MIPS for these symbols.
    +------------------------------------------------*/
    struct symtabl *curr_symtab;
    struct declarator *sp;
    int i;
    curr_symtab= global_top_level;
    for(i=0; i<NHASH; i++)
    {   if(curr_symtab->symtab[i])
        {   sp= curr_symtab->symtab[i];
	    if( (sp->nodetype != FUNCTION_DEFINITION)  &&  (sp->nodetype != FUNCTION_DECLARATOR) )
	    {   printf("sp name: %s", print_declarator_id(sp));   
	        printf(", sp type: %ld", sp->tspecptr);
	    }
	    printf("\n");
        }
    }




    /* iterate through the irnodes and emit the
    |  appropriate MIPS assembly code
    +--------------------------------------------*/
    struct irnode *irlist= irlist_front;
    while(irlist != NULL)
    {   /* add IR to irlist */
        fprintf(mipsout,"%18s", ircodenames[irlist->ircode]);

	if(irlist->ircode == BEGINPROC)
	{   fprintf(mipsout,"(%s)   #start function\n",print_declarator_id(irlist->symptr));
	}
	else
	{   fprintf(mipsout,"\n");
	}

        irlist= irlist->next;
    }




    close(mipsout);
}






