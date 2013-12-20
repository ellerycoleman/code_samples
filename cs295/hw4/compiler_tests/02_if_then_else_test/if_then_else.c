/*============================================================================
| File:		if_then_else.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Test of if/then/else statement.
+-----------------------------------------------------------------------------
| Revision: $Id: if_test.c 453 2013-12-20 21:25:39Z ellery $
+===========================================================================*/


void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{   int x;

    /* Prompt user for integer */
    printstring("\n\n\nEnter any integer: ");
    readint(x);




    /* Determine whether integer is even or odd; print message */

    printstring("\n\n");
    printint(x);
   
    if((x % 2) == 1)
    {   printstring(" is an odd number.\n\n\n");
    }
    else
    {   printstring(" is an even number.\n\n\n");
    }
}




