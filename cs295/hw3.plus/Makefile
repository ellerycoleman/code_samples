#=============================================================================
# File:		Makefile
# Date:		Sun Sep 15 00:25:06 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for hw3; builds parser with symbol table.
#-----------------------------------------------------------------------------
# Revision: $Id$
#=============================================================================



all:    clean bison_parser.o flex_scanner.o e95_parser.c e95_parser.h symbol_table.c symbol_table.h
	gcc -g flex_scanner.o bison_parser.o e95_parser.c -o e95_parser -lfl #symbol table code included in e95_parser.*
	ln -s e95_parser parser
	ln -s e95_parser a.out


scanner:  clean bison_parser.tab.h flex_scanner.o e95_tokens.c e95_scanner.c bison_parser.o e95_parser.h e95_parser.c 
	gcc -g e95_scanner.c flex_scanner.o e95_tokens.c -o e95_scanner bison_parser.o e95_parser.c -lfl
	ln -s e95_scanner scanner


flex_scanner.o:   flex_scanner.c
	gcc -g -c flex_scanner.c


flex_scanner.c:       flex_scanner.l
	flex -o flex_scanner.c flex_scanner.l


bison_parser.o:	bison_parser.tab.h bison_parser.tab.c
	gcc -g -c bison_parser.tab.c -o bison_parser.o


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
	rm -f e95_parser parser
	rm -f testprog*
	sleep 2;


test:	all
	chmod 755 test.sh
	./test.sh
