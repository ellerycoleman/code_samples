/*============================================================================
| File:		mips_generator.h
| Date:		Tue Nov 12 21:03:13 GMT 2013
| Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
| Abstract:	Header file for MIPS generation subsystem.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/


/*-------------------------------
| Data Structures
+------------------------------*/




/*-------------------------------
| Function Prototypes
+------------------------------*/
void generate_mips(void);
void declare_global_vars(void);




/*-------------------------------
| Global Variables
+------------------------------*/
FILE *mipsout;
char mipsfname[100];



