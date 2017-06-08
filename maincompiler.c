/**********************************************************
 * File:    	maincompiler.c
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
#include "codegen.h"
#include "genasm.h"

extern TOKEN parseresult;
extern int labelnumber;

int main() {

	int res;
	initsyms();
	res = yyparse();
	//printst();
	printf("yyparse result = %8d\n", res);
	ppexpr(parseresult); 
	gencode(parseresult, blockoffs[blocknumber], labelnumber);
	return 0;
}
