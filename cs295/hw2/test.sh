#!/usr/local/bin/bash


cat <<DATA | ./parser
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

DATA


