#=============================================================================
# File:		Makefile
# Date:		Fri Dec 20 18:42:33 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for basic arithmetic operators.
#-----------------------------------------------------------------------------
# Revision: $Id: Makefile 465 2013-12-21 02:58:17Z ellery $
#=============================================================================

TESTFILE  = math_test.c
STATEMENT = Basic Arithmetic Operator


all:    clean $(TESTFILE) wdc
	@cat $(TESTFILE) | ../../wdc 
	@printf "\n\n\n\n\n\n\n"
	@printf "=====================================================\n";
	@printf "       $(STATEMENT) Test\n";
	@printf "=====================================================\n\n";
	@printf "The code in $(TESTFILE) has been compiled. See out.ir and out.s\n"
	@printf "Invoking spim now...\n\n"

	@ln -s ../../spim_bin .
	./spim_bin/usr/bin/spim -f out.s
	

wdc:
	(cd ../..; make)


clean:
	rm -f *.s
	rm -f *.ir
	if [ -x ./spim_bin ]; then    unlink ./spim_bin; fi
	@printf "\n\nDirectory Cleaned.\n\n"




test:	all


