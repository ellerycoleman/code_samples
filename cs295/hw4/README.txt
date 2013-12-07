#-----------------------------------------------------------------------------
# File:		README.txt
# Date:		Thu Nov 21 01:40:49 EST 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for hw3.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------


This version of the compiler implements a symbol table management system.
It uses hashing with linear probing to store the symbols in the symbol table.

There is a 'struct symtabl' which has an id (name), sid (number), and an
array of 'struct declarator *'s which serves as the actual symbol table.

All the code and configuration for the symbol table management system is
stored in symbol_table.c and symbol_table.h.  The code basically crawls the
parse tree and inserts all decls into the symbol table.  It also stores labels
in the symbol table, and complains if identifiers or labels are used without
being defined.




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
