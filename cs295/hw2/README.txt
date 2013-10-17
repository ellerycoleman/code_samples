#-----------------------------------------------------------------------------
# File:		README.txt
# Date:		Thu Oct 10 23:54:39 EDT 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------


The e95_parser program for hw2 is built from the following 10 files:

1. Makefile          -> build control file for the hw2 project
2. bison_parser.y    -> working grammar for the e95 C language in bison format
3. e95_parser.c      -> main routine for the parser
4. e95_scanner.c     -> main routine for the scanner, used only if you choose
                        to build the standalone scanner with "make scanner"
5. e95_tokens.h      -> header file for tokens, used for standalone scanner
6. e95_tokens.c      -> provides routine to initialize token map for the lexer
7. flex_scanner.l    -> implements a lexer for the e95 C language via flex
8. parser_support.h  -> header file for user-defined parser functions
9. parser_support.c  -> implementation of user-defined parser functions
10. test.sh          -> a shell-based test program for the parser




======================================
BUILD INSTRUCTIONS
======================================
Verify that "make" is in your command path.
Then run:

$ make 


The default target is "all", and it will create a binary
called 'e95_parser'.  It will also create a symlink to that
binary called "parser".





======================================
RUN INSTRUCTIONS
======================================
If the binary is invoked without arguments, it will read/write from
STDIN/STDOUT.

$ ./parser


Otherwise, you can specify the input and output files
$ ./parser in.txt out.txt


A "-" can be used to signify STDIN or STDOUT:

$ ./parser - -






======================================
TEST INSTRUCTIONS
======================================
$ make test



To test a different series of statments, simply edit the
"here" document located inside of test.sh. There here doc
starts at line 17.

Please note that the 'test' target in the Makefile runs
the parser via GDB.  To change this, you can comment out
the GDB invocation at the bottom of the file and use the
non-GDB version of the test instead.



