#!/usr/local/bin/bash
#-----------------------------------------------------------------------------
# File:		test.sh
# Date:		Thu Oct 10 23:54:39 EDT 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	A shell script to exercise and test the hw2 parser.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------


#
# Write a C program and store it in testprog.c
#----------------------------------------------

cat <<DATA > testprog.c 

/*---------------------------------------------------*/
/* This is the progam input, see parser output below */
/*---------------------------------------------------*/


int main(void)
{   
    goto embedded_block_2;
    int testing;
    {   int case1;

        {   int case1_3;
        }

        {   int case1_4;
        }
    }


embedded_block_2:
    testing++;
    int funcvar;

    {   int case2;

        {   int case3;
        }

foo:
        {   int case4;
        }
    }



    {    int case2_2;
         
         {   int case3_2;
         }

         {   int case4_2;
         }
    }
}


/*---------------------------------------------------*/
/*             END test program code                 */
/*---------------------------------------------------*/


DATA




# Now invoke the parser and give it the testprog.c
# file that was created above.
#----------------------------------------------------
cat testprog.c
echo "Preparing to parse the above input program..."
echo "Running parser with GDB..."
echo;echo;

cat testprog.c | gdb -x gdb.cmd ./parser | tee  testprog_output.c

