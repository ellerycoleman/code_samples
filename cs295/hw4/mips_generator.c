/*============================================================================
| File:		mips_generator.c
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Traverses the irnode list and emits the appropriate MIPS
|               assembly language instructions.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

#define max(i,j) ( (i) > (j) ? (i) : (j) )

char *reglist[]= { "NULL",
               "$s0",
               "$s1",
               "$s2",
               "$s3",
               "$s4",
               "$s5",
               "$s6",
               "$s7",
               "$v0",
	       "$a0"
             };




/*-----------------------------------------------
 | generate_mips
 +---------------------------------------------*/
void generate_mips(void)
{
    int stacksize;
    int offset;

    printf("\n\n\n\n\n");
    printf("===========================================================\n");
    printf(" Generate MIPS called...\n");
    printf("===========================================================\n\n\n");
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
    |  Also create necessary variables for any string constants.
    +-------------------------------------------------------------*/
    declare_global_vars();




    /* Print MIPS instructions for entering a function.
    +----------------------------------------------------*/
    fprintf(mipsout,"\t.text\n");
    fprintf(mipsout,"\t.globl\tmain\n\n");




    /* iterate through the irnodes and emit the
    |  appropriate MIPS assembly code
    +--------------------------------------------*/
    irlist= irlist_front;
    while(irlist != NULL)
    {   /* add IR to irlist */
        

        switch(irlist->ircode)
	{
	    case BEGINPROC:

               /* Calculate the stack size. It will be 56 bytes at minimum,
               |  plus space for each of the symbols within current function
               |  symbol table and its nested symbol tables.
               +--------------------------------------------------------------*/

               ;/* establish name of symbol table that we're looking for */
               char symtab_name[128];
               strcpy(symtab_name,print_declarator_id(irlist->symptr));
               strcat(symtab_name,"_funcdef");


               /* navigate to the symbol table */
               struct symtabl *curr_symtab= global_top_level;
               curr_symtab= curr_symtab->child;
               while(strcmp(symtab_name,curr_symtab->id))
               {   curr_symtab= curr_symtab->rsibling;
               }

               offset=0;
               stacksize= calculate_stack_size(curr_symtab,offset) + 56;


	       /* round up to nearest double-word boundary */
	       stacksize= round_to_doubleword(stacksize);


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
               write_function_entry_code(stacksize);

	       break;


            case LOADADDRESS:
	       if(irlist->symptr->global)
	       {   fprintf(mipsout,"\tla\t%s,_VAR_%s\n",reglist[irlist->oprnd1],print_declarator_id(irlist->symptr));
	       }
	       else
	       {   
	           struct declarator *sp= resolve(irlist->symptr,irlist->symptr->curr_symtab);
		   irlist->symptr= sp;
	           fprintf(mipsout,"\tla\t%s,-%d($fp)\n",reglist[irlist->oprnd1],(irlist->symptr->offset + 56));
	       }
	       break;


            case LOADWORD:
	       if(irlist->symptr->global)
	       {   fprintf(mipsout,"\tlw\t%s,_VAR_%s\n",reglist[irlist->oprnd1],print_declarator_id(irlist->symptr));
	       }
	       else
	       {   
	           struct declarator *sp= resolve(irlist->symptr,irlist->symptr->curr_symtab);
		   irlist->symptr= sp;
	           fprintf(mipsout,"\tlw\t%s,-%d($fp)\n",reglist[irlist->oprnd1],(irlist->symptr->offset + 56));
	       }
	       break;



            case LOADCONSTANT:
	       fprintf(mipsout,"\tli\t%s,%d\n",reglist[irlist->oprnd1],irlist->oprnd2);
	       break;


            case STOREWORDINDIRECT:
	       fprintf(mipsout,"\tsw\t%s,(%s)\n",reglist[irlist->oprnd1],reglist[irlist->oprnd2]);
	       break;


            case LOADWORDINDIRECT:
	       fprintf(mipsout,"\tlw\t%s,(%s)\n",reglist[irlist->oprnd1],reglist[irlist->oprnd2]);
	       break;


	    case ENDPROC:
               write_function_exit_code(stacksize);
	       break;


            case PRINTINT:
               fprintf(mipsout,"\tli\t$v0,1\t\t# load syscall code for printint\n");
               break;


            case READINT:
               fprintf(mipsout,"\tli\t$v0,5\t\t# load syscall code for readint\n");
               break;


            case PRINTSTRING:
               fprintf(mipsout,"\tli\t$v0,4\t\t# load syscall code for printstring\n");
               break;


            case CREATE_STRINGVAR:
	       /* The irlist was processed by declare_global_vars(); this created
	       |  all the stringvars.  Now we will load the specified stringvar
	       |  into the $a0 register for printing.
	       +------------------------------------------------------------------*/
               fprintf(mipsout,"\tla\t$a0,%s\t\t# address of string for printint\n", irlist->label);
	       break;


            case SYSCALL:
               fprintf(mipsout,"\tsyscall\t\t\t# execute syscall\n");
               break;


            case BRANCH_EQ:
	       fprintf(mipsout,"\tbeq\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], irlist->label);
	       break;


            case BRANCH_GT:
	       fprintf(mipsout,"\tbgt\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], irlist->label);
	       break;


            case BRANCH_GTE:
	       fprintf(mipsout,"\tbge\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], irlist->label);
	       break;


            case BRANCH_LT:
	       fprintf(mipsout,"\tblt\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], irlist->label);
	       break;


            case BRANCH_LTE:
	       fprintf(mipsout,"\tble\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], irlist->label);
	       break;


            case MIPSLABEL:
	       fprintf(mipsout,"%s:\n", irlist->label);
	       break;


            case JUMP:
	       fprintf(mipsout,"\tj %s\n", irlist->label);
	       break;


            case COMMENT:
	       fprintf(mipsout,"\t\t\t\t# %s\n", irlist->label);
	       break;


            case ADD:
	       fprintf(mipsout,"\tadd\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], reglist[irlist->oprnd3]); 
	       break;


            case ADD1:
	       fprintf(mipsout,"\taddi\t%s, %s, 1\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], reglist[irlist->oprnd3]); 
	       break;


	    case SUBTRACT:
	       fprintf(mipsout,"\tsub\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], reglist[irlist->oprnd3]);
	       break;


            case REMAINDER:
	       fprintf(mipsout,"\trem\t%s, %s, %s\n", reglist[irlist->oprnd1], reglist[irlist->oprnd2], reglist[irlist->oprnd3]); 
	       break;


	    default:
	       fprintf(mipsout,"# encountered unknown IR code: %s\n", ircodenames[irlist->ircode]);
	       break;
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

    /* Create global variable definitions for all non-function symbols in
    |  the global_top_level symtable.
    +---------------------------------------------------------------------*/
    fprintf(mipsout,"\n\n\t.data\n\n");
    fprintf(mipsout,"#Global Variable Declarations\n\n");
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


    /* Cycle through all irnodes and look for CREATE_STRINGVAR nodes.
    |  Create a variable for each node.
    +-----------------------------------------------------------------*/
    irlist= irlist_front;
    while(irlist != NULL)
    {   if(irlist->ircode == CREATE_STRINGVAR)
        {   fprintf(mipsout,"%s:\t.asciiz\t\"%s\"\n",irlist->label, (char *)irlist->symptr);
	}
	irlist= irlist->next;
    }
    fprintf(mipsout,"\n\n\n#String variables for printstring function\n\n");
    fprintf(mipsout,"\n\n\n");
}






/*-----------------------------------------------
 | write_function_entry_code(int stacksize)
 +---------------------------------------------*/
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





/*-----------------------------------------------
 | write_function_exit_code(int stacksize)
 +---------------------------------------------*/
void write_function_exit_code(int stacksize)
{
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
        stacksize
    );

}





/*-----------------------------------------------
 | calculate_stack_size()
 +---------------------------------------------*/
int calculate_stack_size(struct symtabl *curr_symtab, int offset)
{
    int currsymtabsize;
    int maxchildsize;
    int i;
    struct declarator *sp;


    /* Count the size of the current symtable's variables
    +------------------------------------------------------*/
    currsymtabsize=0;
    offset=0;
    for(i=0; i<NHASH; i++)
    {   if(curr_symtab->symtab[i])
        {   sp= curr_symtab->symtab[i];
            switch(sp->tspecptr->type)
	    {   case SIGNED_INT:
	        case UNSIGNED_INT:
	        case SIGNED_LONG_INT:
	        case UNSIGNED_LONG_INT:
	           currsymtabsize += 4;
		   sp->offset= offset + 4;
		   offset+= 4;
	           break;


	        case SIGNED_SHORT_INT:
	        case UNSIGNED_SHORT_INT:
	           currsymtabsize += 2;
		   sp->offset= offset + 2;
		   offset+= 2;
	           break;


	        case SIGNED_CHAR:
	        case UNSIGNED_CHAR:
	           currsymtabsize += 1;
		   sp->offset= offset + 1;
		   offset+= 1;
	           break;


                default:
		   ;
        	   break;
	    }
	}
    }


    /* if there are any immediate child symbol tables, calculate
    |  the size of the largest child symbol table and return it.
    +------------------------------------------------------------*/
    maxchildsize=0;
    curr_symtab= curr_symtab->child;
    if(curr_symtab)
    {   while(curr_symtab)
        {   i= calculate_stack_size(curr_symtab,offset);
	    if(i>maxchildsize)
	    {   maxchildsize= i;
	    }
	    curr_symtab= curr_symtab->rsibling;
        }
    }

    return currsymtabsize + maxchildsize;
}




/*-----------------------------------------------
 | round_to_doubleword()
 +---------------------------------------------*/
int round_to_doubleword(int stacksize)
{   int align=8;
    return ((stacksize + align - 1) / align) * align;
}





