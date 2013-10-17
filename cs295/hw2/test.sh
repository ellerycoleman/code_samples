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


    /* testing array declarators */
    int a[5];
    char word[15];

    /* testing direct abstract declarators */
    int f1(int (*));
    int f2(char []);
    int f3(char [5]);
    int f4(char [][5]);
    int f5(char (*)[5]);
    int f6(char [][5]);
    int f7(char [10][5]);
    int f8(char [][10][5]);
    int f9(char [][10][10][5]);
    int f10(char[][12][11][10][5]);
    int f11(char *[]);
   
    /* testing subscript_expr */
    name[2+3];
    (2+3)*4;
    i++[2];



    /* testing conditional_expr, logical, bitwise, and relational expr */
    ( a || b ) ? 11 : 22;
    ( a && B ) || ( c && d ) || ( e && f );
    ( a && b ) ? 22 : 33;
    ( a|b ) && (c|d) && (e|f) && (g|h);
    ( a |  b ) ? 33 : 44;
    (a^b) | (c^d) | (d^e);
    ( a ^  b ) ? 44 : 55;
    (a&b) ^ (c&d) ^ (d&e);
    (a == (b != c));
    a >  1;
    b >= 2;
    c <  3;
    d <= 4;

    a << 1;
    b >> 2;


    /* testing  function calls */
    display_current_time();
    calc_volume(5, 8<<1,*4);



    /* testing conditional statements and loops */
    if(a > b)
    {   printf("%d", A);
    }


    if(c > d)
    {   printf("%d", c);
    }
    else if( e > f)
    {   printf("%d", e);
    }
    else
    {   printf("%d", q);
    }


    while(i <= 10)
    {   printf("%d\n", ++i);
        while( j >= 20)
	{   printf("%d\n", j--);
	}
    }


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
echo "Running parser with GDB..."
echo;echo;

cat testprog.c | gdb -x gdb.cmd ./parser | tee  testprog_output.c

