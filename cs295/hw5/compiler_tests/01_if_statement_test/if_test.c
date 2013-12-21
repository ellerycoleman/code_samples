/*============================================================================
| File:		if_test.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Test of if statement.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{
    int n;
    int x;

    x=5;


    printstring("\n\n\n\nEnter a number that is greater than, less than, or equal to 5: ");
    readint(n);


    if( n > x)
    {
        printstring("'");
	printint(n);
        printstring("' is greater than "); 
	printint(x);
	printstring(".\n");
    }


    if( n < x)
    {
        printstring("'");
	printint(n);
        printstring("' is less than "); 
	printint(x);
	printstring(".\n");
    }


    if( n == x)
    {
        printstring("'");
	printint(n);
        printstring("' is equal to "); 
	printint(x);
	printstring(".\n");
    }
    printstring("\n\n");

}


