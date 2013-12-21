#=============================================================================
# File:		README.txt
# Date:		Fri Dec 20 22:16:39 GMT 2013
# Author:	Ellery Coleman <ellery.coleman@fas.harvard.edu>
# Abstract:	README file for e95 compiler.
#-----------------------------------------------------------------------------
# Revision: $Id$
#=============================================================================


The code within this directory implements the following subsystems:
  - Lexical Analyzer
  - Language Parser
  - Symbol Table Manager
  - IR Generator
  - MIPS Assembly Code Generator


Please note that there is no type checking subsystem; the compiler can only
handle typesafe programs.


This distribution contains a copy of the spim emulator for linux.
It is built as part of the compiler build process.




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

$ ./wdc   # then type your C program on the terminal
  
  -- OR --

$ cat myfile.c | ./wdc   #pipe your file into wdc



You may also pass a file to the compiler as a command-line argument:

$ ./wdc myfile.c



No matter which way you choose to run wdc the resulting IR
will be written to out.ir and the mips assembly will be
written to out.s.







======================================
TEST INSTRUCTIONS
======================================

This distribution contains a "compiler_test" directory.
All of the tests for the various language features
are contained within that directory.  Simply navigate
to any test directory and run the Makefile.
For example:



$ cd compiler_tests
$ cd 01_if_statement_test
$ make 





