#-----------------------------------------------------------------------------
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for calculator
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------



all:    clean flex_scanner.o bison_parser.o calc_tokens.c calc_scanner.c calc_tokens.h
	gcc flex_scanner.o bison_parser.o -o calc_parser -lfl
	ln -s calc_parser parser


flex_scanner.o:   flex_scanner.c
	gcc -c flex_scanner.c


flex_scanner.c:       flex_scanner.l bison_parser.tab.h
	flex -o flex_scanner.c flex_scanner.l


bison_parser.tab.h:	bison_parser.y
	bison -d bison_parser.y


bison_parser.o:   bison_parser.tab.c
	gcc -c bison_parser.tab.c -o bison_parser.o

bison_parser.tab.c:	bison_parser.y
	bison -d bison_parser.y


clean:
	rm -f *.o
	rm -f flex_scanner.c
	rm -f a.out
	rm -f calc_scanner scanner
	rm -f bison_parser.tab.*
	rm -f parser


test:	all
	./test.sh
