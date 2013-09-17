#-----------------------------------------------------------------------------
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for hw1.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------



all:    clean flex_scanner.o e95_tokens.c
	gcc e95_scanner.c e95_tokens.c flex_scanner.o -o e95_scanner -lfl


flex_scanner.o:   flex_scanner.c
	gcc -c flex_scanner.c -lfl


flex_scanner.c:       flex_scanner.l
	flex -o flex_scanner.c flex_scanner.l

clean:
	rm -f *.o
	rm -f flex_scanner.c
	rm -f a.out
	rm -f e95_scanner
