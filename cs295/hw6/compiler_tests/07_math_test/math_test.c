/*============================================================================
| File:		math_test.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Test of basic arithmetic operators.
+-----------------------------------------------------------------------------
| Revision: $Id: if_test.c 453 2013-12-20 21:25:39Z ellery $
+===========================================================================*/

void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{
    int a;
    a= 2 + 9 / 3 - 4 * 2;
    printstring("Value of a: ");
    printint(a);
    printstring("\n\n");

}


