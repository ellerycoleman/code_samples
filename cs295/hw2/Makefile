#-----------------------------------------------------------------------------
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#-----------------------------------------------------------------------------



all:    clean bison_parser.o flex_scanner.o e95_parser.c parser_support.h parser_support.c
	gcc parser_support.c e95_parser.c flex_scanner.o bison_parser.o -o e95_parser -lfl
	ln -s e95_parser parser


scanner:  clean bison_parser.tab.h flex_scanner.o e95_tokens.c e95_scanner.c bison_parser.o
	gcc e95_scanner.c flex_scanner.o e95_tokens.c -o e95_scanner bison_parser.o -lfl
	ln -s e95_scanner scanner


flex_scanner.o:   flex_scanner.c
	gcc -c flex_scanner.c


flex_scanner.c:       flex_scanner.l
	flex -o flex_scanner.c flex_scanner.l


bison_parser.o:	bison_parser.tab.h bison_parser.tab.c
	gcc -c bison_parser.tab.c -o bison_parser.o


bison_parser.tab.h:	bison_parser.y
	bison -d bison_parser.y


bison_parser.tab.c:	bison_parser.y
	bison -d bison_parser.y



clean:
	rm -f *.o
	rm -f flex_scanner.c
	rm -f a.out
	rm -f e95_scanner scanner
	rm -f bison_parser.tab.*
	rm -f parser


test:	all
	./test.sh
