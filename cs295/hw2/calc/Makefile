#-----------------------------------------------------------------------------
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for calculator
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------



all:    clean flex_scanner.o calc_tokens.c
	gcc calc_scanner.c calc_tokens.c flex_scanner.o -o calc_scanner -lfl
	ln -s calc_scanner scanner


flex_scanner.o:   flex_scanner.c
	gcc -c flex_scanner.c


flex_scanner.c:       flex_scanner.l
	flex -o flex_scanner.c flex_scanner.l

clean:
	rm -f *.o
	rm -f flex_scanner.c
	rm -f a.out
	rm -f calc_scanner scanner
	rm -f bison_parser.tab.*

test:	all
	./test.sh
