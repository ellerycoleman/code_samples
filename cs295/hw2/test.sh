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
}


/*---------------------------------------------------*/
/*             END test program code                */
/*---------------------------------------------------*/



DATA


cat testprog.c
echo "Preparing to parse the above input program..."
cat testprog.c | gdb -x gdb.cmd ./parser
