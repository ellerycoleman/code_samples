/*============================================================================
| File:		e95_parser.c
| Date:		Sun Sep 15 02:18:18 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Implements user defined functions for parser.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


#include "e95_tokens.h"
#include "e95_parser.h"
#include "bison_parser.tab.h"



#define INT_MAX       2147483647
#define LONG_MAX      2147483647
#define ULONG_MAX     4294967295
#define TMPSTRSZ      10240


void connect_io(int,char **);
void usage(void);
int yylex();


extern char *yytext;
extern int yyleng;
extern FILE *yyin;
extern int yylineno;
extern int cvalue;              /* for converting char constants */
extern long long ivalue_tmp;    /* for converting int constants  */
extern struct ast *parse_tree;  /* root of completed parse tree  */
FILE *input, *output;


int num_of_tokens_processed=0;
int need_array_paren;
extern int yylineno;
extern char e95_strbuf2[];
char tmpstr[TMPSTRSZ];
extern int indent_count;
extern int symtab_sid;


int first_ptr;          /* pretty print support */
int global_allocation;  /* keeps track of dynamic memory allocation */










/*===============================================================
 * Function: main()
 * Abstract: main function for parser
 *===============================================================
 */
int main(int argc, char **argv)
{

    /* Configure input/output sources based on
    * user invocation of command.
    *-------------------------------------------*/
    connect_io(argc,argv);
    yyin= input;


    printf("in e95_parser.c::main() about to run yyparse()\n\n\n");


    /* Parse Input
    +---------------*/
    yyparse();


    /* Store parsed input in parse tree
    +------------------------------------*/
    parse_tree= (struct ast *) reverse_tld_list((struct tld_list *)parse_tree);


    /* traverse parse tree to construct symbol tables
    +--------------------------------------------------*/
    create_symbol_tables(parse_tree);


    /* traverse parse tree to generate IR.
    +-------------------------------------------------*/
    generate_ir(parse_tree);


    /* traverse parse tree to generate pretty print
    +-------------------------------------------------*/
    indent_count=0;
    /* print_tree(parse_tree); */


    return 0;

}






/*===============================================================
 * Function: connect_io()
 * Abstract: This method contains code from scanner_main.c provided
 *           on the section website.
 *===============================================================
 */
void connect_io(int argc,char **argv)
{
    if(argc == 3)
    {
#ifdef DEBUG
        printf("argc == 3\n\n");
	printf("argv[1]: %s\n", argv[1]);
	printf("argv[2]: %s\n", argv[2]);
#endif
        /* connect input  */
        if(strcmp("-",argv[1]))
	{

#ifdef DEBUG
	    printf("connecting input: %s\n", argv[1]);
#endif
	    input= fopen(argv[1], "r");
	}
	else
	{   input= stdin;
	}

	/* connect output */
	if(strcmp("-",argv[2]))
	{
#ifdef DEBUG
   	    printf("connecting output: %s\n", argv[2]);
#endif
	    output= fopen(argv[2], "w");
	}
	else
	{   output= stdout;
	}
    }
    else if(argc <= 2)
    {   input= stdin;
        output= stdout;
    }
    else
    {   usage();
    }
}




/*===============================================================
 * Function: usage()
 * Abstract: Writes proper scanner usage notice to STDOUT.
 *===============================================================
 */
void usage(void)
{   printf("\n\nUsage: scanner [input_file output_file]\n\n");
    exit(-1);
}





/*===============================================================
 * Function: yyerror()
 * Abstract: writes parser error messages to STDERR
 *===============================================================
 */
yyerror(char *s,...)
{   fprintf(stderr, "Problem in Parseville!\n");
    fprintf(stderr, "\terror: %s\n", s);
    fprintf(stderr, "\tline number: %d\n\n", yylineno);
    fprintf(stderr, "\t%s\n\n", e95_strbuf2);
}




/*===============================================================
 * Function: print_tree
 * Abstract: writes parse tree to STDOUT
 *===============================================================
 */
void print_tree(struct ast *nodeptr)
{   printf("\n\n\n\n");
    printf("#----------------------------------------------------------\n");
    printf("#           Parse Tree for E95 C Language                  \n");
    printf("#----------------------------------------------------------\n");
    printf("\n\n\n");



    /* The only type of AST that is passed to print_tree is a *tld_list,
     | so we'll cast the AST into a *tld_list.
     +-----------------------------------------------------------------*/
    struct tld_list *tldlist= (struct tld_list *)nodeptr;


    /* A tld_list has a series of tld nodes; each node pointing to either
    |  a decl or a funcdef.
    +-----------------------------------------------------------------*/
    struct decl *tdecl;
    struct function_def *funcdef;
    int i=1;
    struct declarator *d;


    do  /* cycle through all of the TLD's */
    {       clearstr(tmpstr);

            /*--------------------------------------------+
	     |            if TLD is a DECL
	     +--------------------------------------------*/
        if(tldlist->tld->datatype == DECL)
        {   tdecl= (struct decl *)tldlist->tld->d;

            /* print declarator list */
	    clearstr(tmpstr);
            printf("%s", print_expr((struct ast *)tdecl,tmpstr));
	    clearstr(tmpstr);

	    printf(";\n");
	}




            /*--------------------------------------------+
	     |      if TLD is a FUNCTION_DEFINITION
	     +--------------------------------------------*/
        if(tldlist->tld->datatype == FUNCTION_DEFINITION)
        {

	    clearstr(tmpstr);


	    /* retrieve function definition
	    +---------------------------------*/
	    struct function_def *funcdef= (struct function_def *)tldlist->tld->f;



            /* a function defintion is composed of a
	    |  function_defspec and a compound statement.
	    |  A compound statement contains a decostat_list.
	    +-------------------------------------------------*/
	    struct function_defspec *fdspec= funcdef->fdspec;
	    struct ast *cstmt= funcdef->cstmt;
	    struct decostat_list *dlist;



	    /*
	    |  fdspec contains the function declarator.
	    +-------------------------------------------*/
	    struct declarator *d= fdspec->d;



	    /* print function return type */
	    printf("\n\n(%s ", print_type(fdspec->typespec));




            /* Retrieve and print function name from this declarator,
            |  considering that pointers may be present.  Also grab
	    |  the parameter_list.
            +------------------------------------------------------------*/
	    struct parameter_list *plist;
            if(d->nodetype == POINTER_DECLARATOR)
            {   while(d->next)
                {   d= d->next;
		    sprintf(&tmpstr[strlen(tmpstr)],"*");
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



	    sprintf(&tmpstr[strlen(tmpstr)],") %s(", funcname);
	    printf("%s", tmpstr); clearstr(tmpstr);



            /* print parameter list */
	    print_parameter_list(plist,tmpstr);
	    sprintf(&tmpstr[strlen(tmpstr)],")");
	    sprintf(&tmpstr[strlen(tmpstr)],"\n{\n");
	    printf("%s",tmpstr); clearstr(tmpstr);



            /* display compound statement block
	    +-------------------------------------*/
	    dlist= (struct decostat_list *) cstmt->l;
	    ++indent_count;
	    do
	    {   indent(tmpstr);
	        print_expr(dlist->decostat,tmpstr);

                /* no semicolons after compound statements */
		if(   (dlist->decostat->nodetype == COMPOUND_STATEMENT)      ||
		      ( (dlist->decostat->nodetype == LABELED_STATEMENT)     &&
		        (dlist->decostat->r->nodetype == COMPOUND_STATEMENT)
                      )
		  )
		{   printf("\n");
		}
		else
		{   printf("%s;\n",tmpstr); clearstr(tmpstr);
		}
		printf("%s",tmpstr); clearstr(tmpstr);
	    } while( (dlist= dlist->next) != NULL);
	    --indent_count;


	    printf("\n}\n\n");
	}



    }while( (tldlist= tldlist->next) != NULL );


    /* print symbol tables */
    printtabs(global_top_level);
    printf("\n\nTotal amount of memory dynamically allocated by this compiler: %d bytes\n", global_allocation);
    printf("Current symtab sid: %ld\n\n", symtab_sid);
}




/*===============================================================
 * Function: print_expr
 * Abstract: writes a struct expr to char *
 *===============================================================
 */
char * print_expr(struct ast *expr,char *exprstr)
{   struct constant *k;
    int i=0;
    int initial_typespec=1;
    char *c;
    struct decostat_list *dlist;
    struct decl *tdecl;
    struct declarator_list *dl;
    struct declarator *d;
    struct ast *tast;
    struct cond_expr *cexpr;
    struct flow *tflow;




    switch(expr->nodetype)
    {   case SEP_SEMICOLON:
	   break;

        case INTEGER_CONSTANT:
	   k= (struct constant *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"%d", k->value);
	   break;

        case CHARACTER_CONSTANT:
	   k= (struct constant *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"'%c'", k->value);
	   break;

        case CHARACTER_CONSTANT_OCTAL:
	   k= (struct constant *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"%s", k->value);
	   break;

        case STRING_CONSTANT:
	   k= (struct constant *)expr;
	   c= (char *)(k->value);
	   putchar('"');
	   for(i=0; i<=strlen(c); i++)
	   {   if(c[i] == '\n')  /* unescape return chars */
	       {   sprintf(&exprstr[strlen(exprstr)],"\\n");
	       }
	       else
	       {   putchar(c[i]);
	       }
	   }
	   putchar('"');
	   break;

        case LABEL:
	   k= (struct constant *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"%s", k->value);
	   break;

        case RW_GOTO:
	   sprintf(&exprstr[strlen(exprstr)],"goto ");
	   sprintf(&exprstr[strlen(exprstr)],"%s", expr->l);
	   break;

        case RW_CONTINUE:
	   sprintf(&exprstr[strlen(exprstr)],"continue");
	   break;

        case RW_BREAK:
	   sprintf(&exprstr[strlen(exprstr)],"break");
	   break;


        case COMPOUND_STATEMENT:
	    dlist= (struct decostat_list *)expr->l;
	    sprintf(&exprstr[strlen(exprstr)],"{\n");


	    ++indent_count;
	    do
	    {   indent(exprstr);
	        print_expr(dlist->decostat,exprstr);

		if( (dlist->decostat->nodetype == COMPOUND_STATEMENT) ||
		    (dlist->decostat->nodetype == LABELED_STATEMENT)
		  )
		{   sprintf(&exprstr[strlen(exprstr)],"\n");
		}
		else
		{   sprintf(&exprstr[strlen(exprstr)],";\n");
		}
		printf("%s",exprstr); clearstr(exprstr);
            }while( (dlist= dlist->next) != NULL);
	    --indent_count;


	    indent(exprstr);
	    sprintf(&exprstr[strlen(exprstr)],"}\n\n");
	    printf("%s",exprstr); clearstr(exprstr);
	   break;


        case DECOSTAT_LIST:  /* comma separated statements */
	    dlist= (struct decostat_list *)expr;
	    dlist= reverse_decostat_list(dlist);
	    while(dlist->next != NULL)
	    {   print_expr(dlist->decostat,&exprstr[strlen(exprstr)]);
	        sprintf(&exprstr[strlen(exprstr)],", ");
	        dlist= dlist->next;
	    }
	    print_expr(dlist->decostat,exprstr);
	    break;


        case PLUS_SIGN:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"+");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case MINUS_SIGN:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"-");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case ASTERISK:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"*");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_DIVISION:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"/");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_REMAINDER:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   putchar('%');

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_ADD:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"+=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_SUBTRACT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"-=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_MULTIPLY:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"*=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;



        case OP_ASSIGNMENT_DIVIDE:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"/=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_REMAINDER:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"%=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_LEFT_BITSHIFT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"<<=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_RIGHT_BITSHIFT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],">>=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_BITWISE_AND:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"&=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_BITWISE_OR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"|=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_ASSIGNMENT_BITWISE_XOR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)],"^=");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case DECL:
	   need_array_paren= 1;
	   tdecl= (struct decl *)expr;
           dl= tdecl->dl;


           /* print declarator list */
	   if( dl->d->nodetype == SIMPLE_DECLARATOR)
	   {   sprintf(&exprstr[strlen(exprstr)],"(%s) ",print_type(tdecl->tspecptr->type));
	   }

	   else if( dl->d->nodetype == POINTER_DECLARATOR)
	   {   if( dl->d->next != NULL  &&  dl->d->next->nodetype != SIMPLE_DECLARATOR)
	       {   sprintf(&exprstr[strlen(exprstr)],"(%s ",print_type(tdecl->tspecptr->type));
	           first_ptr=1;
	       }
	       else
	       {   sprintf(&exprstr[strlen(exprstr)],"(%s) ",print_type(tdecl->tspecptr->type));
	       }
	   }
	   else
	   {   sprintf(&exprstr[strlen(exprstr)],"(%s",print_type(tdecl->tspecptr->type));
	   }


	   do
	   {   d= dl->d;
	       print_decl((struct ast *)d,exprstr);
	       if(dl->next != NULL)
	       {   sprintf(&exprstr[strlen(exprstr)],", ");
	       }
	   }while( (dl= dl->next) != NULL);
	   break;


        case SIMPLE_DECLARATOR:
	   print_decl(expr,exprstr);
	   break;


        case UNARY_MINUS_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"-");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case UNARY_PLUS_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"+");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case PREDECREMENT_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"--");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case POSTDECREMENT_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   sprintf(&exprstr[strlen(exprstr)],"--");
	   break;


        case PREINCREMENT_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"++");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case POSTINCREMENT_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   sprintf(&exprstr[strlen(exprstr)],"++");
	   break;


        case INDIRECTION_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"*");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case ADDRESS_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"&");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case BITWISE_NEGATION_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"~");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case LOGICAL_NEGATION_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"!");
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case LABELED_STATEMENT:
	   sprintf(&exprstr[strlen(exprstr)], "\n");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],":\n");
	   indent(exprstr);
	   print_expr(expr->r,exprstr);
	   break;


        case PARENTHESIZED_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case CAST_EXPR:
	   tast= expr->l;
	   d= (struct declarator *)tast;
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   sprintf(&exprstr[strlen(exprstr)],"%s ", print_type(d->tspecptr->type));
	   print_decl((struct ast *)d,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   print_expr(expr->r,exprstr);
	   break;


        case SUBSCRIPT_EXPR:
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],"[");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],"]");
	   break;


        case CONDITIONAL_EXPR:
	   cexpr= (struct cond_expr *)expr;

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(cexpr->cond,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," ? ");


	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(cexpr->return1,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," : ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(cexpr->return2,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case LOGICAL_OR_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," || ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case LOGICAL_AND_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," && ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case BITWISE_OR_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," | ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case BITWISE_XOR_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," ^ ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case BITWISE_AND_EXPR:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," & ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_EQUALITY:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," == ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_NON_EQUALITY:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," != ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_RELATIONAL_LT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," < ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_RELATIONAL_LTE:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," <= ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case GREATER_THAN_SYMBOL:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," > ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_RELATIONAL_GTE:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," >= ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_LEFT_BITSHIFT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," << ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case OP_RIGHT_BITSHIFT:
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");

	   sprintf(&exprstr[strlen(exprstr)]," >> ");

	   sprintf(&exprstr[strlen(exprstr)],"(");
	   print_expr(expr->r,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case FUNCTION_CALL:
	   print_expr(expr->l,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],"(");
	   if(expr->r != NULL)
	   {   print_expr(expr->r,exprstr);
	   }
	   sprintf(&exprstr[strlen(exprstr)],")");
	   break;


        case IF_STATEMENT:
	   tflow= (struct flow *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"if(");
	   print_expr(tflow->cond,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")\n");
	   print_expr(tflow->thendo,exprstr);
	   break;


        case IF_ELSE_STATEMENT:
	   tflow= (struct flow *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"if(");
	   print_expr(tflow->cond,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")\n");
	   print_expr(tflow->thendo,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],"else ");
	   print_expr(tflow->elsedo,exprstr);
	   break;


        case WHILE_STATEMENT:
	   tflow= (struct flow *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"while(");
	   print_expr(tflow->cond,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")\n");
	   print_expr(tflow->thendo,exprstr);
	   break;


        case DO_STATEMENT:
	   tflow= (struct flow *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"do");
	   print_expr(tflow->thendo,exprstr);
	   sprintf(&exprstr[strlen(exprstr)]," while(");
	   print_expr(tflow->cond,exprstr);
	   sprintf(&exprstr[strlen(exprstr)],")\n");
	   break;


        case FOR_STATEMENT:
	   tflow= (struct flow *)expr;
	   sprintf(&exprstr[strlen(exprstr)],"for(");
	   if(tflow->forinit != NULL)
	   {   print_expr(tflow->forinit,exprstr);
	       putchar(';');
	   }
	   if(tflow->cond != NULL)
	   {   print_expr(tflow->cond,exprstr);
	       putchar(';');
	   }
	   if(tflow->forupdate)
	   {   print_expr(tflow->forupdate,exprstr);
	       sprintf(&exprstr[strlen(exprstr)],")\n");
	   }
	   print_expr(tflow->thendo,exprstr);
	   break;


        case RW_RETURN:
	   sprintf(&exprstr[strlen(exprstr)],"return");
	   if( (expr->l) != NULL)
	   {   sprintf(&exprstr[strlen(exprstr)]," ");
	       sprintf(&exprstr[strlen(exprstr)],"(");
	       print_expr(expr->l,exprstr);
	       sprintf(&exprstr[strlen(exprstr)],")");
	   }
	   break;





        default:
	   sprintf(&exprstr[strlen(exprstr)],"PRINT_EXPR: not sure what to do with nodetype: %d\n",expr->nodetype);
	   break;

    }
    return exprstr;

}




/*===============================================================
 * Function: print_decl
 * Abstract: writes a struct decl to STDOUT
 *===============================================================
 */
char * print_decl(struct ast *expr, char *declstr)
{
    struct declarator_list *dl;
    struct declarator *d;
    struct declarator *dview;
    struct declarator *ad;
    struct ast *tast;

    switch(expr->nodetype)
    {

        case  SIMPLE_DECLARATOR:
           d= (struct declarator *)expr;
           sprintf(&declstr[strlen(declstr)],"%s", d->id);
           break;


        case POINTER_DECLARATOR:
	   d= (struct declarator *)expr;

	   do
	   {   if( d->nodetype == POINTER_DECLARATOR )
	       {
	           /* printing paren before pointer declarator */
		   if(first_ptr)
		   {
	               dview= d;
		       while(dview->next != NULL  &&  dview->next->nodetype == POINTER_DECLARATOR)
		       {   dview= dview->next;
		       }
		       if(dview->next != NULL  &&  dview->next->nodetype == SIMPLE_DECLARATOR)
		       {   sprintf(&declstr[strlen(declstr)],") ");
		           first_ptr=0;
		       }
	           }
	           sprintf(&declstr[strlen(declstr)],"*");
               }

	       else if( d->nodetype == SIMPLE_DECLARATOR )
	       {   sprintf(&declstr[strlen(declstr)],"%s", d->id);
	       }

	       else if( d->nodetype == ARRAY_DECLARATOR )
	       {   sprintf(&declstr[strlen(declstr)]," (%s", d->adeclarator->id);
                   sprintf(&declstr[strlen(declstr)],"[");
	           print_expr((struct ast *)d->exp,declstr);
         	   sprintf(&declstr[strlen(declstr)],"])");
	       }

	       else if( d->nodetype == FUNCTION_DECLARATOR )
	       {
	           /* print function name */
	           sprintf(&declstr[strlen(declstr)],") ");
	           sprintf(&declstr[strlen(declstr)],"%s(", d->adeclarator->id);

                   /* print parameter list */
	           print_parameter_list(d->plist,tmpstr);
	           sprintf(&declstr[strlen(declstr)],")");
	       }

	       if(d->next != NULL   &&   d->next->nodetype == ARRAY_DECLARATOR)
	       {   sprintf(&declstr[strlen(declstr)],")");
	       }

            }while( (d= d->next) != NULL);
	    break;


         case FUNCTION_DECLARATOR:
	 /* print function name */
	    d= (struct declarator *)expr;
	    sprintf(&declstr[strlen(declstr)],") %s(", d->adeclarator->id);

         /* print parameter list */
	    print_parameter_list(d->plist,declstr);
	    sprintf(&declstr[strlen(declstr)],")");
            break;


        case ARRAY_DECLARATOR:
	   if(need_array_paren)
	   {   sprintf(&declstr[strlen(declstr)],")");
	       need_array_paren=0;
           }
           d= (struct declarator *)expr;
	   sprintf(&declstr[strlen(declstr)]," %s", d->adeclarator->id);
	   sprintf(&declstr[strlen(declstr)],"[");
	   print_expr((struct ast *)d->exp,declstr);
	   sprintf(&declstr[strlen(declstr)],"]");
	   break;


        case DIRECT_ABSTRACT_DECLARATOR:
	   print_dad(d,declstr);
	   break;

        default:
	   sprintf(&declstr[strlen(declstr)],"PRINT_DECL: not sure what to do with nodetype: %d\n",expr->nodetype);
	   break;
    }
    return declstr;
}












declarator *new_simple_declarator(char *id)
{   declarator *d= emalloc(sizeof(declarator));
    d->nodetype= SIMPLE_DECLARATOR;
    d->id= strdup(id);
    return d;
}

declarator *new_label_declarator(struct ast *labelstmt)
{   declarator *d= emalloc(sizeof(declarator));
    d->nodetype= LABELED_STATEMENT;
    d->exp= (struct expr *) labelstmt;
    d->id= (char *)((struct constant *)labelstmt->l)->value;
    return d;
}


declarator *new_pointer_declarator(declarator *next)
{   declarator *d= emalloc(sizeof(declarator));
    d->nodetype= POINTER_DECLARATOR;
    d->next= next;
    return d;
}


declarator *new_array_declarator(int type, struct declarator *arrydec, struct ast *expr)
{   declarator *d= emalloc(sizeof(declarator));
    d->nodetype= ARRAY_DECLARATOR;
    d->adeclarator= arrydec;
    d->exp= (struct expr *)expr;
    return d;
}


declarator *new_direct_abstract_declarator(int type, struct ast *data, declarator *next)
{   declarator *d= emalloc(sizeof(struct declarator));

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
    else if(type == BRACKET_NO_EXPR)
    {   /* printf("TYPE BRACKET_NO_EXPR...\n"); */
    }

    return d;
}


declarator *new_function_declarator(declarator *fdecl, parameter_list *plist)
{
    declarator *d= emalloc(sizeof(declarator));
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
{   declarator_list *dl= emalloc(sizeof(struct declarator_list));
    dl->d= d;
    dl->next= next;
    return dl;
}



parameter_list *new_parameter_list(declarator *pd, parameter_list *next)
{   parameter_list *pl= emalloc(sizeof(struct parameter_list));
    pl->pd= pd;
    pl->next= next;
    return pl;
}



struct tld_list *new_tld_list(struct ast *t, struct ast *next)
{   struct tld_list *tl= emalloc(sizeof(struct tld_list));
    tl->tld= (struct tld *)t;
    tl->next= (struct tld_list *)next;
    return tl;
}


struct ast *new_decostat_list(struct ast *decostat, struct ast *next)
{   struct decostat_list *dl= emalloc(sizeof(struct decostat_list));
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



struct tld_list *reverse_tld_list(struct tld_list *tl)
{   struct tld_list *newroot= NULL;
    while(tl)
    {   struct tld_list *next= tl->next;
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





struct ast *new_decl(int typespecifier, declarator_list *dl)
{
    struct decl *d= emalloc(sizeof(struct decl));  /* this is the decl we'll populate and return */

    dl= reverse_declarator_list(dl);  /* set the declarator_list in proper order  */
    d->nodetype= DECL;
    d->tspecptr= &basic_types[typespecifier];
    d->dl=  dl;

    return  (struct ast *)d;
}



struct ast *new_tld(int datatype, struct ast *tld)
{   struct tld *t= emalloc(sizeof(struct tld));
    t->nodetype= TLD;
    t->datatype= datatype;
    if(datatype == DECL)
    {   t->d= (struct decl *)tld;
    }
    if(datatype == FUNCTION_DEFINITION)
    {   t->f= (struct ast *)tld;
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
{   declarator *pd= emalloc(sizeof(struct declarator));
    pd->tspecptr= &basic_types[typespec];
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
    {   pd->nodetype= (enum ntype) OTHER;  /* This is a parameter_decl that only specified the type */
    }
    return pd;
}





char * print_parameter_list(parameter_list *plist,char *plistr)
{
    int tcount=0;
    declarator *d;
    declarator *ad;
    do
    {   sprintf(&plistr[strlen(plistr)],"%s","(");

        switch(plist->pd->nodetype)
        {

	    case SIMPLE_DECLARATOR:
               sprintf(&plistr[strlen(plistr)],"%s ", print_type(plist->pd->tspecptr->type));
	       if(plist->pd->id != NULL)
	       {   sprintf(&plistr[strlen(plistr)],"%s", plist->pd->id);
	       }
	       break;



	    case POINTER_DECLARATOR:
               sprintf(&plistr[strlen(plistr)],"");
	       struct declarator *tmpd;
	       struct parameter_list *tmplist;

	       tmplist= plist;
	       do
	       {   sprintf(&plistr[strlen(plistr)],"\ntmplist iter %d...\n", ++tcount);
	           tmpd= tmplist->pd;
	           if(tmpd != NULL)
		   {   do
		       {   sprintf(&plistr[strlen(plistr)],"\n\n\n* tmpd type is: %d\n", tmpd->nodetype);
		           tmpd= tmpd->next;
                       }while(tmpd);
		   }
	       }while(tmplist= tmplist->next);



               sprintf(&plistr[strlen(plistr)],"%s ", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;

	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&plistr[strlen(plistr)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
	           {   sprintf(&plistr[strlen(plistr)],"%s", d->id);
		   }
		   else if( d->nodetype == DIRECT_ABSTRACT_DECLARATOR )
		   {   print_dad(d,tmpstr);
		   }
	           else if( d->nodetype == ARRAY_DECLARATOR )
		   {   sprintf(&plistr[strlen(plistr)]," (%s", d->adeclarator->id);
         	       sprintf(&plistr[strlen(plistr)],"[");
	               print_expr((struct ast *)d->exp,tmpstr);
         	       sprintf(&plistr[strlen(plistr)],"])");
		   }
               }while( (d= d->next) != NULL);
	       break;


	    case DIRECT_ABSTRACT_DECLARATOR:
	       d= plist->pd;
	       print_dad(d,tmpstr);
	       break;


	    case DECL:
	       d= plist->pd;
	       print_decl((struct ast *)d,tmpstr);


            case OTHER:  /* print type only */
               sprintf(&plistr[strlen(plistr)],"%s", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;
	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&plistr[strlen(plistr)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
		   {   sprintf(&plistr[strlen(plistr)],"%s", d->id);
		   }
               }while( (d= d->next) != NULL);
	       break;


           default:
	      d= plist->pd;
	      sprintf(&plistr[strlen(plistr)],"PRINT_PARAMETER_LIST: not sure what to do with nodetype: %d\n",d->nodetype);
	      break;

        }
        sprintf(&plistr[strlen(plistr)],")");


        if(plist->next != NULL)
        {   sprintf(&plistr[strlen(plistr)],", ");
        }
    }while((plist= plist->next) != NULL);


    return plistr;
}






char * print_dad(declarator *d,char *dadstr)
{   struct declarator *ad;


    switch(d->dadtype)
    {
        case PAREN_ENCLOSED:
           ad= d->adeclarator;
           sprintf(dadstr,"%s ", print_type(d->tspecptr->type));
           sprintf(dadstr,"(");
           print_decl((struct ast *)ad,tmpstr);
           sprintf(dadstr,")");
           break;


        case BRACKET_NO_EXPR:
	   sprintf(dadstr,"%s ", print_type(d->tspecptr->type));
           sprintf(dadstr,"[]");
	   break;


        case BRACKET_EXPR:
	   sprintf(dadstr,"%s ", print_type(d->tspecptr->type));
           sprintf(dadstr,"[");
	   print_expr((struct ast *)d->exp,dadstr);
           sprintf(dadstr,"]");
	   break;


        case DAD_LIST:
           sprintf(dadstr,"%s ", print_type(d->tspecptr->type));
	   d= reverse_declarators(d);
	   do
	   {   /* print the expression ending the dad_list */
	       if(d->exp != NULL && d->next == NULL)
	       {   sprintf(dadstr,"[");
	           print_expr((struct ast *)d->exp,dadstr);
                   sprintf(dadstr,"]");
               }
	       else if(d->exp != NULL && d->next != NULL)
	       {
	           sprintf(dadstr,"[");
	           print_expr((struct ast *)d->exp,dadstr);
                   sprintf(dadstr,"]");
               }

	       /* print dad_list members without typespecifiers */
	       else
	       {
	           switch(d->dadtype)
	           {
	               case PAREN_ENCLOSED:
                       ad= d->adeclarator;
                       sprintf(dadstr,"(");
		       print_decl((struct ast *)ad,tmpstr);
                       sprintf(dadstr,")");
		       break;


                       case BRACKET_NO_EXPR:
                          sprintf(dadstr,"[]");
                          break;


                       case BRACKET_EXPR:
                          sprintf(dadstr,"[");
		          print_expr((struct ast *)d->exp,dadstr);
                          sprintf(dadstr,"]");
		          break;


		       default:
		          sprintf(dadstr,"ERROR: problem printing dad_list...\n");
		          break;
		   }
               }
           }while( (d= d->next) != NULL);
	   break;

        default:
	   sprintf(dadstr,"unknown dad type: %d\n",d->dadtype);
    }
    return dadstr;
}



struct ast *new_expr(int type,struct ast *l, struct ast *r)
{   struct expr *expr= emalloc(sizeof(struct expr));
    expr->nodetype= type;
    expr->l= l;
    expr->r= r;

    return (struct ast *)expr;
}


struct ast *new_conditional_expr(struct ast *cond, struct ast *return1, struct ast *return2)
{   struct cond_expr *expr= emalloc(sizeof(struct cond_expr));
    expr->nodetype= CONDITIONAL_EXPR;
    expr->cond= cond;
    expr->return1= return1;
    expr->return2= return2;

    return (struct ast *)expr;
}


struct ast *new_constant(int type, void *value)
{   struct constant *k= emalloc(sizeof(struct constant));
    k->nodetype= type;
    k->value= value;
    return (struct ast *)k;
}



struct ast *new_function_def_specifier(int type, struct declarator *d)
{   struct function_defspec *fdefspec= emalloc(sizeof(struct function_defspec));
    fdefspec->nodetype= FUNCTION_DEF_SPECIFIER;
    fdefspec->typespec= type;
    fdefspec->d= d;
    return (struct ast *)fdefspec;
}



struct ast *new_function_definition(struct ast *fdefspec, struct ast *compound_stmt)
{   struct function_def *fdef= emalloc(sizeof(struct function_def));
    fdef->nodetype= FUNCTION_DEFINITION;
    fdef->fdspec= (struct function_defspec *)fdefspec;
    fdef->cstmt= compound_stmt;
    return (struct ast *)fdef;
}


struct ast *new_compound_statement(struct ast *decstmtlist)
{   struct ast *cstmt= emalloc(sizeof(struct ast));
    cstmt->nodetype= COMPOUND_STATEMENT;
    cstmt->l= decstmtlist;

    return cstmt;

}


struct ast *new_if_statement(struct ast *cond, struct ast *thendo, struct ast *elsedo)
{   struct flow *tflow= emalloc(sizeof(struct flow));
    if(elsedo == NULL)
    {   tflow->nodetype= IF_STATEMENT;
    }
    else
    {   tflow->nodetype= IF_ELSE_STATEMENT;
    }
    tflow->cond   = cond;
    tflow->thendo = thendo;
    tflow->elsedo = elsedo;

    return (struct ast *)tflow;
}



struct ast *new_while_statement(struct ast *cond, struct ast *thendo)
{   struct flow *tflow= emalloc(sizeof(struct flow));
    tflow->nodetype= WHILE_STATEMENT;
    tflow->cond= cond;
    tflow->thendo= thendo;

    return (struct ast *)tflow;
}



struct ast *new_do_statement(struct ast *cond, struct ast *thendo)
{   struct flow *tflow= emalloc(sizeof(struct flow));
    tflow->nodetype = DO_STATEMENT;
    tflow->cond     = cond;
    tflow->thendo   = thendo;

    return (struct ast *)tflow;
}



struct ast *new_for_statement(struct ast *forinit,
                              struct ast *cond,
			      struct ast *forupdate,
			      struct ast *thendo
			     )
{   struct flow *tflow= emalloc(sizeof(struct flow));
    tflow->nodetype  = FOR_STATEMENT;
    tflow->forinit   = forinit;
    tflow->cond      = cond;
    tflow->forupdate = forupdate;
    tflow->thendo    = thendo;

    return (struct ast *)tflow;
}




void * emalloc(int size)
{   void *space= malloc(size);
    if(space == NULL)
    {   printf("*** Parser ran out of memory! ***\n");
        exit(-1);
    }
    global_allocation += size;
    return space;
}



char * funcdef_to_string(struct function_def *funcdef,char fdef[])
{
    int tcount=0;
    declarator *d;
    declarator *ad;
    struct parameter_list *plist;
    plist= funcdef->fdspec->d->plist;
    struct function_defspec *fdspec= funcdef->fdspec;


    /* print type and name of function */
    d=fdspec->d;
    sprintf(&fdef[strlen(fdef)],"%s ",print_type(fdspec->typespec));  /*DEBUG: this needs to be updated */


            /* Retrieve and print function name from this declarator,
            |  considering that pointers may be present.  Also grab
	    |  the parameter_list.
            +------------------------------------------------------------*/
            if(d->nodetype == POINTER_DECLARATOR)
            {   while(d->next)
                {   d= d->next;
		    sprintf(&fdef[strlen(fdef)],"*");
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


    sprintf(&fdef[strlen(fdef)]," %s(", funcname);


    do
    {
        switch(plist->pd->nodetype)
        {

	    case SIMPLE_DECLARATOR:
               sprintf(&fdef[strlen(fdef)],"(%s ", print_type(plist->pd->tspecptr->type));
	       /*
	       if(plist->pd->id != NULL)
	       {   sprintf(&fdef[strlen(fdef)],"%s", plist->pd->id);
	       }
	       */
	       break;



	    case POINTER_DECLARATOR:
               sprintf(&fdef[strlen(fdef)],"");
	       struct declarator *tmpd;
	       struct parameter_list *tmplist;

               sprintf(&fdef[strlen(fdef)], "%s ", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;

	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&fdef[strlen(fdef)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
	           {   sprintf(&fdef[strlen(fdef)],"%s", d->id);
		   }
		   else if( d->nodetype == DIRECT_ABSTRACT_DECLARATOR )
		   {   print_dad(d,&fdef[strlen(fdef)]);
		   }
	           else if( d->nodetype == ARRAY_DECLARATOR )
		   {   /* sprintf(&fdef[strlen(fdef)]," (%s", d->adeclarator->id); */
         	       sprintf(&fdef[strlen(fdef)],"[");
	               print_expr((struct ast *)d->exp,&fdef[strlen(fdef)]);
         	       sprintf(&fdef[strlen(fdef)],"])");
		   }
               }while( (d= d->next) != NULL);
	       break;


	    case DIRECT_ABSTRACT_DECLARATOR:
	       d= plist->pd;
	       print_dad(d,&fdef[strlen(fdef)]);
	       break;


	    case DECL:
	       d= plist->pd;
	       print_decl((struct ast *)d,&fdef[strlen(fdef)]);


            case OTHER:  /* print type only */
               sprintf(&fdef[strlen(fdef)],"%s", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;
	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&fdef[strlen(fdef)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
		   {   sprintf(&fdef[strlen(fdef)],"%s", d->id);
		   }
               }while( (d= d->next) != NULL);
	       break;


           default:
	      d= plist->pd;
	      sprintf(&fdef[strlen(fdef)],"PRINT_PARAMETER_LIST: not sure what to do with nodetype: %d\n",d->nodetype);
	      break;

        }
        sprintf(&fdef[strlen(fdef)],")");


        if(plist->next != NULL)
        {   sprintf(&fdef[strlen(fdef)],", ");
        }
    }while((plist= plist->next) != NULL);
    sprintf(&fdef[strlen(fdef)],")");

    return fdef;
}







char * funcdecl_to_string(struct declarator *funcdp,char fdec[])
{
    int tcount=0;
    declarator *d;
    declarator *dporig= funcdp;
    struct parameter_list *plist;


    /* print type and name of function */
    d= funcdp;


    sprintf(&fdec[strlen(fdec)],"%s ",print_type(dporig->tspecptr->type));


            /* Retrieve and print function name from this declarator,
            |  considering that pointers may be present.  Also grab
	    |  the parameter_list.
            +------------------------------------------------------------*/
            if(d->nodetype == POINTER_DECLARATOR)
            {   while(d->next)
                {   d= d->next;
		    sprintf(&fdec[strlen(fdec)],"*");
	        }


		plist= d->plist;


	        if(!d->id)
	        {   d= d->adeclarator;
	        }
            }
	    else
	    {   d= d->adeclarator;
	        plist= dporig->plist;
	    }
            char *funcname= d->id;


    sprintf(&fdec[strlen(fdec)]," %s(", funcname);


    do
    {
        switch(plist->pd->nodetype)
        {

	    case SIMPLE_DECLARATOR:
               sprintf(&fdec[strlen(fdec)],"(%s ", print_type(plist->pd->tspecptr->type));
	       /*
	       if(plist->pd->id != NULL)
	       {   sprintf(&fdec[strlen(fdec)],"%s", plist->pd->id);
	       }
	       */
	       break;



	    case POINTER_DECLARATOR:
               sprintf(&fdec[strlen(fdec)],"");
	       struct declarator *tmpd;
	       struct parameter_list *tmplist;

               sprintf(&fdec[strlen(fdec)], "%s ", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;

	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&fdec[strlen(fdec)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
	           {   sprintf(&fdec[strlen(fdec)],"%s", d->id);
		   }
		   else if( d->nodetype == DIRECT_ABSTRACT_DECLARATOR )
		   {   print_dad(d,&fdec[strlen(fdec)]);
		   }
	           else if( d->nodetype == ARRAY_DECLARATOR )
		   {   /* sprintf(&fdec[strlen(fdec)]," (%s", d->adeclarator->id); */
         	       sprintf(&fdec[strlen(fdec)],"[");
	               print_expr((struct ast *)d->exp,&fdec[strlen(fdec)]);
         	       sprintf(&fdec[strlen(fdec)],"])");
		   }
               }while( (d= d->next) != NULL);
	       break;


	    case DIRECT_ABSTRACT_DECLARATOR:
	       d= plist->pd;
	       print_dad(d,&fdec[strlen(fdec)]);
	       break;


	    case DECL:
	       d= plist->pd;
	       print_decl((struct ast *)d,&fdec[strlen(fdec)]);


            case OTHER:  /* print type only */
               sprintf(&fdec[strlen(fdec)],"%s", print_type(plist->pd->tspecptr->type));
	       d= plist->pd;
	       do
	       {   if( d->nodetype == POINTER_DECLARATOR )
	           {   sprintf(&fdec[strlen(fdec)],"*");
		   }
		   else if( d->nodetype == SIMPLE_DECLARATOR )
		   {   sprintf(&fdec[strlen(fdec)],"%s", d->id);
		   }
               }while( (d= d->next) != NULL);
	       break;


           default:
	      d= plist->pd;
	      sprintf(&fdec[strlen(fdec)],"PRINT_PARAMETER_LIST: not sure what to do with nodetype: %d\n",d->nodetype);
	      break;

        }
        sprintf(&fdec[strlen(fdec)],")");


        if(plist->next != NULL)
        {   sprintf(&fdec[strlen(fdec)],", ");
        }
    }while((plist= plist->next) != NULL);
    sprintf(&fdec[strlen(fdec)],")");

    return fdec;
}



void clearstr(char *str)
{   int i;
    for(i=0; i<TMPSTRSZ; i++)
    {   str[i]='\0';
    }
}


char * indent(char *str)
{
    extern int indent_count;
    int i;

    for(i=0; i<indent_count; i++)
    {   sprintf(&str[strlen(str)],"%s","    ");
    }

    return str;
}




#include "symbol_table.c"
#include "generate_ir.c"



