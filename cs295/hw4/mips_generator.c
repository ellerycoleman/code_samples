/*============================================================================
| File:		mips_generator.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the irnode list and emits the appropriate MIPS
|               assembly language instructions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


char *reg[]= { "NULL",
               "$s0",
	       "$s1",
	       "$s2",
	       "$s3",
	       "$s4",
	       "$s5",
	       "$s6",
	       "$s7"
           };



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



    /* Check top_level_symbol table for all non-function symbols.
    |  Emit the appropriate MIPS instructions for these symbols.
    +-------------------------------------------------------------*/
    declare_global_vars();


    /* Print MIPS instructions for entering a function.
    +----------------------------------------------------*/
    int tmpstack= 56;
    fprintf(mipsout,"\t.text\n");
    fprintf(mipsout,"\t.globl\tmain\n\n");


    /* iterate through the irnodes and emit the
    |  appropriate MIPS assembly code
    +--------------------------------------------*/
    struct irnode *irlist= irlist_front;
    while(irlist != NULL)
    {   /* add IR to irlist */
        /* fprintf(mipsout,"%18s", ircodenames[irlist->ircode]); */

        switch(irlist->ircode)
	{
	    case BEGINPROC:

	       /* If the function is not "main", we should modify the function name to make
	       |  sure the name doesn't match a SPIM reserved word.
	       +---------------------------------------------------------------------------*/
	       if(strcmp("main",print_declarator_id(irlist->symptr)) )
	       {   fprintf(mipsout,"_VAR_%s:\n\n",print_declarator_id(irlist->symptr));
               }

	       /* For function "main" we will leave the name as-is.
	       +----------------------------------------------------*/
	       else
	       {   fprintf(mipsout,"%s:\n\n",print_declarator_id(irlist->symptr));
               }

               /* Print standard code to initialize stack for function execution
	       +-----------------------------------------------------------------*/
               write_function_entry_code(tmpstack);

	       break;



            case LOADADDRESS:
	       fprintf(mipsout,"\tla\t%s,_VAR_%s\n",reg[irlist->oprnd1],print_declarator_id(irlist->symptr));
	       break;



            case LOADCONSTANT:
	       fprintf(mipsout,"\tli\t%s,%d\n",reg[irlist->oprnd1],irlist->oprnd2);
	       break;


            case STOREWORDINDIRECT:
	       fprintf(mipsout,"\tsw\t%s,(%s)\n",reg[irlist->oprnd1],reg[irlist->oprnd2]);
	       break;


            case LOADWORDINDIRECT:
	       fprintf(mipsout,"\tlw\t%s,(%s)\n",reg[irlist->oprnd1],reg[irlist->oprnd2]);
	       break;


	    case ENDPROC:
               fprintf(mipsout,
"\tlw	$s7, -52($fp)	# restore $s7\n"
"\tlw	$s6, -48($fp)	# restore $s6\n"
"\tlw	$s5, -44($fp)	# restore $s5\n"
"\tlw	$s4, -40($fp)	# restore $s4\n"
"\tlw	$s3, -36($fp)	# restore $s3\n"
"\tlw	$s2, -32($fp)	# restore $s2\n"
"\tlw	$s1, -28($fp)	# restore $s1\n"
"\tlw	$s0, -24($fp)	# restore $s0\n"
"\tlw	$ra, -4($fp)	# restore $ra\n"
"\tlw	$fp, ($fp)	# restore old $fp\n"
"\taddiu	$sp, $sp, %d	# pop off our stack frame\n"
"\tjr	$ra		# return to caller\n",
        tmpstack
               );

	    default:
	       fprintf(mipsout,"# encountered unknow IR code: %s\n", ircodenames[irlist->ircode]);
	}

        irlist= irlist->next;
    }




    close(mipsout);
}








/*-----------------------------------------------
 | declare_global_vars - prints data section of
 |                       asm file.
 +---------------------------------------------*/
void declare_global_vars(void)
{
    struct symtabl *curr_symtab;
    struct declarator *sp;
    int i;

    fprintf(mipsout,"\n\n\t.data\n");
    curr_symtab= global_top_level;
    for(i=0; i<NHASH; i++)
    {   if(curr_symtab->symtab[i])
        {   sp= curr_symtab->symtab[i];
	    if( (sp->nodetype != FUNCTION_DEFINITION)  &&  (sp->nodetype != FUNCTION_DECLARATOR) )
	    {
	        switch(sp->tspecptr->type)
		{   case SIGNED_INT:
		    case UNSIGNED_INT:
		    case SIGNED_LONG_INT:
		    case UNSIGNED_LONG_INT:
		       fprintf(mipsout,"_VAR_%s:\t.word\t0\n",print_declarator_id(sp));
		       break;

		    case SIGNED_SHORT_INT:
		    case UNSIGNED_SHORT_INT:
		       fprintf(mipsout,"_VAR_%s:\t.half\t0\n",print_declarator_id(sp));
		       break;

		    case SIGNED_CHAR:
		    case UNSIGNED_CHAR:
		       fprintf(mipsout,"_VAR_%s:\t.byte\t0\n",print_declarator_id(sp));
		       break;


                    default:
		       fprintf(mipsout,"# Found an unknown global variable type.\n");
		       break;
		}
	    }
        }
    }
    fprintf(mipsout,"\n\n\n");
}


void write_function_entry_code(int stacksize)
{
    fprintf(mipsout,
            "\taddiu\t$sp, $sp, -%d	# push space for our stack frame onto the stack\n"
"\tsw\t$fp, %d($sp)	# save the old $fp\n"
"\taddiu\t$fp, $sp, %d	# $fp -> stack frame\n"
"\tsw	$ra, -4($fp)	# save the return address\n"
"\tsw\t$a0, -8($fp)\t# save parameter $a0\n"
"\tsw\t$a1, -12($fp)\t# save parameter $a1\n"
"\tsw\t$a2, -16($fp)\t# save parameter $a2\n"
"\tsw\t$a3, -20($fp)\t# save parameter $a3\n"
"\tsw	$s0, -24($fp)	# save $s0\n"
"\tsw	$s1, -28($fp)	# save $s1\n"
"\tsw	$s2, -32($fp)	# save $s2\n"
"\tsw	$s3, -36($fp)	# save $s3\n"
"\tsw	$s4, -40($fp)	# save $s4\n"
"\tsw	$s5, -44($fp)	# save $s5\n"
"\tsw	$s6, -48($fp)	# save $s6\n"
"\tsw	$s7, -52($fp)	# save $s7\n",
        stacksize,
	stacksize-4,
	stacksize-4
    );

}

