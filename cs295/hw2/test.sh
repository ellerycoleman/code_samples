#!/usr/local/bin/bash


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



    /* return statement */
    return;
    return 3;



    /* decls inside of function blocks */
    int i;
    int *p;


}

/*---------------------------------------------------*/
/*             END test program code                 */
/*---------------------------------------------------*/



DATA


cat testprog.c
echo "Preparing to parse the above input program..."
cat testprog.c | gdb -x gdb.cmd ./parser
