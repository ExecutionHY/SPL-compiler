/**********************************************************
 *	File:		codegen.c
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 8, 2017
 **********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "symtab.h"
#include "genasm.h"
#include "codegen.h"
#include "pprint.h"

void genc(TOKEN code);

/* Set DEBUGGEN to 1 for debug printouts of code generation */
#define DEBUGGEN 0

int nextlabel;    /* Next available label number */
int stkframesize;   /* total stack frame size */

/* Top-level entry for code generator.
   pcode    = pointer to code:  (program foo (output) (progn ...))
   varsize  = size of local storage in bytes
   maxlabel = maximum label number used so far

Add this line to the end of your main program:
    gencode(parseresult, blockoffs[blocknumber], labelnumber);
The generated code is printed out; use a text editor to extract it for
your .s file.
         */

void gencode(TOKEN pcode, int varsize, int maxlabel){
	TOKEN name, code;
	name = pcode->operands;
	code = name->link;
	nextlabel = maxlabel + 1;
	stkframesize = asmentry(name->stringval,varsize);
	genc(code);
	asmexit(name->stringval);
}

/* Trivial version: always returns RBASE + 0 */
/* Get a register.   */
/* Need a type parameter or two versions for INTEGER or REAL */
int getreg(int kind) {
	/*     ***** fix this *****   */
	return RBASE;
}

/* Trivial version */
/* Generate code for arithmetic expression, return a register number */
int genarith(TOKEN code) {
	int num, reg;
	if (DEBUGGEN) {
		printf("genarith\n");
		dbugprinttok(code);
	}
	switch ( code->tokenType ) {
		case TOKEN_NUM:
			switch (code->dataType) {
				case TYPE_INT:
					num = code->intval;
					reg = getreg(WORD);
					if ( num >= MINIMMEDIATE && num <= MAXIMMEDIATE )
					asmimmed(MOVL, num, reg);
					break;
				case TYPE_REAL:
	/*     ***** fix this *****   */
				break;
			}
			break;
		case TOKEN_ID:
	/*     ***** fix this *****   */
			break;
		case OPERATOR:
	/*     ***** fix this *****   */
			break;
	}
	return reg;
}


/* Generate code for a Statement from an intermediate-code form */
void genc(TOKEN code){
	TOKEN tok, lhs, rhs;
	int reg, offs;
	SYMBOL sym;
	if (DEBUGGEN) {
		printf("genc\n");
		dbugprinttok(code);
	}
	if ( code->tokenType != OPERATOR ) {
		printf("Bad code token");
		dbugprinttok(code);
	}
	switch ( code->whichval ) {
		case OP_PROGN:
			tok = code->operands;
			while ( tok != NULL ) {
				genc(tok);
				tok = tok->link;
			}
			break;
		case OP_ASSIGN:                   /* Trivial version: handles I := e */
			lhs = code->operands;
			rhs = lhs->link;
			reg = genarith(rhs);              /* generate rhs into a register */
			sym = searchst(lhs->stringval);    /* assumes lhs is a simple var  */

			offs = sym->offset - stkframesize; /* net offset of the var   */
			/* store value into lhs  */
			switch (code->dataType) {
				case TYPE_INT:
					asmst(MOVL, reg, offs, lhs->stringval);
					break;
	/* ...  */
			}
			break;
	}
}
