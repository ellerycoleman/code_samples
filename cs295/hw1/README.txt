#-----------------------------------------------------------------------------
# File:		README.txt
# Date:		Thu Oct  6 19:44:43 GMT 2011
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for hw1.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------




The scanner for hw1 is built from the following 5 files:

1. Makefile        - build file for the scanner.
2. e95_scanner.c   - contains the main function for the scanner.
3. e95_tokens.c    - defines all recognized tokens
4. e95_tokens.h    - header file for scanner
5. flex_scanner.l  - lex rules for the scanner



BUILD INSTRUCTIONS
===================
Make sure that the "make" command is in your path.
Then run:

$ make 


The default target is "all", and it will create a binary
called 'e95_scanner'.  It will also create a symlink to that
binary called "scanner".





RUN INSTRUCTIONS
=================
If the binary is invoked without arguments, it will read/write from
STDIN/STDOUT.

$ ./scanner


Otherwise, you can specify the input and output files
$ ./scanner in.txt out.txt


A "-" can be used to signify STDIN or STDOUT:

$ ./scanner - -



