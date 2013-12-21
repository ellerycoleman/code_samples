/*============================================================================
| File:		while_test.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Test of the WHILE statement.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{   int x;
    x=10;

    /* Prompt user for integer */
    printstring("\n\n\nCounting down from 10 in a while loop: \n");



    /* Count down in while loop */
    while(x >= 0)
    {   printstring("* ");
        printint(x);
	printstring("\n");
	x--;
    }
    printstring("\n\n\n");

}




