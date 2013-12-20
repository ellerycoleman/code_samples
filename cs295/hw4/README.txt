#=============================================================================
# File:		README.txt
# Date:		Thu Nov 21 01:40:49 EST 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for e95 compiler.
#-----------------------------------------------------------------------------
# Revision: $Id$
#=============================================================================


The code within this directory implements the following subsystems:
  - Lexical Analyzer
  - Language Parser
  - Symbol Table Management
  - IR Generation
  - MIPS Assembly Code Generation


Please note that there is no type checking subsystem; the compiler can only
handle typesafe programs.


This distribution contains a copy of the spim emulator for linux.



======================================
BUILD INSTRUCTIONS
======================================
Make sure that the "make" command is in your path.
Then run:

$ make 


The default target is "all", and it will create a binary
called 'wdc'.  The 'wdc' binary is the compiler.





======================================
RUN INSTRUCTIONS
======================================

There are 2 ways to run wdc from STDIN:

$ ./wdc
  
  -- OR --

$ cat myfile.c | ./wdc



You can also run it with a file:

$ ./wdc myfile.c



No matter which way you choose to run wdc,
the IR will be written to wdc.ir and the mips
assembly will be written to wdc.s.





======================================
TEST INSTRUCTIONS
======================================

This dist contains a "compiler_test" directory.
All of the tests for the various language features
are contained within that directory.  Simply navigate
to any test directory and run the Makefile.
For example:



$ cd compiler_tests
$ cd 01_if_statement_test
$ make test





