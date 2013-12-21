#=============================================================================
# File:		Makefile
# Date:		Fri Dec 20 18:42:33 GMT 2013
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Makefile for IF_STATEMENT testing.
#-----------------------------------------------------------------------------
# Revision: $Id$
#=============================================================================

TESTFILE  = while_test.c
STATEMENT = WHILE


all:    clean $(TESTFILE) wdc
	@cat $(TESTFILE) | ../../wdc 
	@printf "\n\n\n\n\n\n\n"
	@printf "=====================================================\n";
	@printf "       $(STATEMENT) STATEMENT TEST\n";
	@printf "=====================================================\n\n";
	@printf "The code in $(TESTFILE) has been compiled. See stdin.ir and stdin.mips\n"
	@printf "Invoking spim now...\n\n"

	@ln -s ../../spim_bin .
	./spim_bin/usr/bin/spim -f out.s
	

wdc:
	(cd ../..; make)


clean:
	rm -f *.s
	rm -f *.ir
	if [ -x ./spim_bin ]; then    unlink ./spim_bin; fi
	@printf "\n\nDirectory Cleaned.\n"
	@printf "\n\n\n\n\n"




test:	all


