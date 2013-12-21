/*============================================================================
| File:		prime_number.c
| Date:		Tue Dec 17 22:38:46 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	A program to generate and display the first N prime numbers.
|               Adapted from the following URL:
| http://www.programmingsimplified.com/c/source-code/c-program-for-prime-number
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/

void printint(int);
void printstring(char *);
void readint(int);



void main(void)
{
    int n, i, count, c;
    i=3;


    printstring("\n\n\n\nNumber of primes you would like to compute: ");
    readint(n);


    if( n >= 1)
    {   printstring("First ");
        printint(n);
	printstring(" prime numbers are: \n");
	printstring("2\n");
    }

    for(count=2; count <= n;)
    {   
    
        for(c=2; c <= i - 1; c++)
        {   if( i%c == 0)
	    {   break;
	    }
	}
	if( c == i)
	{   printint(i);
	    printstring("\n");
	    count++;
        }
	i++;
    }
}


