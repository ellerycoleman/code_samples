/*============================================================================
| File:		scanner.l
| Date:		Sat Sep 14 20:54:57 GMT 2013
| Author:	ellerycoleman@fas.harvard.edu
| Abstract:	A flex program to generate a lexical analyzer for a subset
|               of the C89 programming language.
+-----------------------------------------------------------------------------
| Revision: $Id$
+===========================================================================*/



/*-----------------------------------------------------------
 |                      declarations
 +---------------------------------------------------------*/
%{
     int chars= 0;
     int words= 0;
     int lines= 0;
%}









/*-----------------------------------------------------------
 |                 patterns and actions
 +---------------------------------------------------------*/
%%
[a-zA-Z]+ { words++; chars += strlen(yytext); }
\n        { chars++; lines++; }
.         { chars++; }









%%
/*-----------------------------------------------------------
 |                    user functions                        
 +---------------------------------------------------------*/
main(int argc, char **argv)
{   yylex();
    printf("%8d %8d %8d\n", lines, words, chars);
}





