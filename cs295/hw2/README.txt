#-----------------------------------------------------------------------------
# File:		README.txt
# Date:		Thu Oct 10 23:54:39 EDT 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------


1st cut of parser... more to come.




======================================
BUILD INSTRUCTIONS
======================================
Make sure that the "make" command is in your path.
Then run:

$ make 


The default target is "all", and it will create a binary
called 'e95_scanner'.  It will also create a symlink to that
binary called "scanner".





======================================
RUN INSTRUCTIONS
======================================
If the binary is invoked without arguments, it will read/write from
STDIN/STDOUT.

$ ./parser


Otherwise, you can specify the input and output files
$ ./parser in.txt out.txt


A "-" can be used to signify STDIN or STDOUT:

$ ./scanner - -






======================================
TEST INSTRUCTIONS
======================================
$ make test
