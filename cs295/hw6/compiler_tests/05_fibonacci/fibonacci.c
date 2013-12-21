/*============================================================================
| File:		fibonacci.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	A program to generate and display the first N numbers of the
|               fibonacci sequence.
|               Adapted from the following URL:
| http://www.cquestions.com/2008/01/write-c-program-to-find-fibonacci.html
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{   int k,r;
    int i,j,f;
    
    i=0;
    j=1;


    /* Prompt user for requested fibonacci range */
    printstring("\n\n\n\nNumber of fibonacci numbers would you like to compute: ");
    readint(r);
    printstring("\n");
    printint(i);
    printstring(" ");
    printint(j);
    printstring(" ");

    for(k=2; k<r; k=k+1)
    {   f= i+j;
        i=j;
	j=f;
	printint(j);
	printstring(" ");
    }

    printstring("\n\n\n\n");
}



