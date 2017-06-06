/**********************************************************
 * File:    	mainparser.c
 * Project: 	SPL-compiler
 * Author:		Execution
 * Modified:	Jun 6, 2017
 **********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexan.h"
#include "symtab.h"
#include "parse.h"
#include "pprint.h"

extern TOKEN parseresult;

int main() {

	int res;
	initsyms();
	res = yyparse();
	//printst();
	printf("yyparse result = %8d\n", res);
	if (DEBUG & DB_PARSERES) dbugprinttok(parseresult);
	ppexpr(parseresult);           /* Pretty-print the result tree */
	//gencode(parseresult, blockoffs[blocknumber], labelnumber);
	return 0;
}
