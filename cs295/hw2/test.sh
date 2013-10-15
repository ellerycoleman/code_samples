#!/usr/local/bin/bash
#-----------------------------------------------------------------------------
# File:		test.sh
# Date:		Thu Oct 10 23:54:39 EDT 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	A shell script to exercise and test the hw2 parser.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------


#
# Write a C program and store it in testprog.c
#----------------------------------------------

cat <<DATA > testprog.c 

/*---------------------------------------------------*/
/* This is the progam input, see parser output below */
/*---------------------------------------------------*/
short q,r,s,t;
int  i, j;
char  a, b;
long int  l;
unsigned int  u;
unsigned char  uc;
unsigned short us;
unsigned long  ul;
void veee,vee,ve,v;


short ***q,*r,*s;
int **i, *j;
char *a, *b;


int/* this is a test of the comments */a;
int add(int i, int j);
char sub(int **p, int ***r);
int f1(char, int, void, long);
int main(void)
{   ;
    ;
    ;
    3;
  'A';
  "ellery";
  '\101';

  goto label1;
  continue;
  break;
  return;
  return 8;
  1,2,3,4,5;
}

int main(void)
{   1,2,3,4,5;
    1+2-3+4-5;
    1*2/3%4*5;

    /* testing assignments */
    a=  5;
    a+= 5;
    a-= 5;
    a*= 5;
    a/= 5;
    a%= 5;
    a<<= 5;
    a>>= 5;
    a&= 5;
    a|= 5;
    a^= 5;


    /* testing direct abstract declarators and array declarators */
    int (*q)(void);
    int a[5];
    char word[15];


    /* unary ops */
    -5;
    +5;
    --a;
    a--;
    ++a;
    a++;
    *i;
    &i;
    ~i;
    !i;



    /* return statement */
    return;
    return 3;



    /* decls inside of function blocks */
    int i;
    int *p;


    /* testing labeled statements */
MYSTMT:    {  
               a= 10;
	       b= 5;
	       c= a+b;
	       return 0;
           }

    /* parenthesized expr  */
    (i= 2, j= 3);


    /* cast expr  */
    (int *)p;
}

/*---------------------------------------------------*/
/*             END test program code                 */
/*---------------------------------------------------*/


DATA




# Now invoke the parser and give it the testprog.c
# file that was created above.
#----------------------------------------------------
cat testprog.c
echo "Preparing to parse the above input program..."
echo "Running the parser with GDB..."
cat testprog.c | gdb -x gdb.cmd ./parser



