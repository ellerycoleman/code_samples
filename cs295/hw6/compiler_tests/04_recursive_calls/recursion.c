/*============================================================================
| File:		recursion.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Test of recursive function calls.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



void printint(int);
void printstring(char *);
void readint(int);


void printhello(void);



int x;

void main(void)
{   int a;  
    a=5;
    x=9;
    printhello();   
    printstring("\n\n\n");
}




/*-----------------------------------------------
 | User defined functions
 *---------------------------------------------*/
void printhello(void)
{   
    printstring("\n\n\nMESG: printhello() invoked, x is the ");
    if(x % 2 == 0)
    {   printstring("even number ");
    }
    else
    {   printstring("odd number ");
    }
    printint(x);
    printstring("\n");


    x= x-1;
    if(x > 0)
    {   printhello();    /* recursive call */
    }
}




