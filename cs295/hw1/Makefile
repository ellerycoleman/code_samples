#-----------------------------------------------------------------------------
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for hw1.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------



all:    clean scanner.o scanner_main.c e95_tokens.c
	gcc scanner_main.c e95_tokens.c scanner.o -o scanner -lfl


scanner.o:   scanner.c
	gcc -c scanner.c -lfl


scanner.c:       scanner.l
	flex -o scanner.c scanner.l

clean:
	rm -f *.o
	rm -f scanner.c
	rm -f a.out
	rm -f scanner
