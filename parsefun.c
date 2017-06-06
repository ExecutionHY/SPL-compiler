/**********************************************************
 * File:    	parsefun.c
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

extern int lineCnt;

/* See parse.h for all debug constants */

int labelnumber = 0;  // sequential counter for internal label numbers
int debug_call_num = 0;	// sequential counter for finding a specific place during program execution
char *last_method = "yyparse()"; 	// the method that called the currently-executing method, if applicable

TOKEN makeProgram(TOKEN program_name, TOKEN routine) {
	
	if (DEBUG & DB_MAKEPROGRAM) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeProgram(),\n");
		dbugprint2toks(program_name, routine);
		last_method = "makeProgram()";
	}
	
	TOKEN program = makeOp(OP_PROGRAM);    //26
	if (!program) {
		printf("Error: failed to alloc TOKEN in makeProgram().\n");
		return NULL;
	}
	
	// Create the head of tree [parseresult]
	program->operands = program_name;
	program_name->link = routine;
	
	if (DEBUG & DB_MAKEPROGRAM) {
		printf("Finished makeProgram().\n");
		dbugprinttok(program);
	}
	return program;
	
}

TOKEN makeOp(int opnum) {
	
	if (DEBUG & DB_MAKEOP) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeOp(), from %s\n", last_method);
		if (DB_PRINT_ARGS) {
			printf(" Arguments:\n  %d\n\n", opnum);
		}
		last_method = "makeOp()";
	}
	
	if (opnum < 0) {
		printf(" Warning: opnum is negative (%d), makeOp().\n", opnum);
	}
	
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makeOp().\n");
		return NULL;
	}
	
	out->tokenType = OPERATOR;
	out->whichval = opnum;
	
	if (DEBUG & DB_MAKEOP) {
		printf(" Finished makeOp().\n");
		dbugprint1tok(out);
	}
	
	return out;
}

TOKEN cons(TOKEN list, TOKEN item) {
	if (list == NULL) return item;
	TOKEN t = list;
	while (t->link != NULL) {
		t = t->link;
	}
	t->link = item;
	if (DEBUG & DB_CONS) {
		printf("cons\n");
		dbugprinttok(list);
		dbugprinttok(item);
	};
	return list;
}

TOKEN findType(TOKEN tok) {
	
	if (DEBUG & DB_FINDTYPE) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In findType(), from %s\n", last_method);
		dbugprint1arg(tok);
		last_method = "findType()";
	}
	
	SYMBOL sym, typ;
	// sym = searchins(tok->stringval); // triggers segfault
	sym = searchst(tok->stringval);
	
	if (!sym) {
		printf(" Error: type \"%s\" not found in symbol table, findType().\n", tok->stringval);
		return NULL;
	}
	
	typ = sym->dataType;
	int kind = sym->kind;
	
	// Arg represents symbol of basic dataType (INTEGER, REAL, STRINGTYPE, BOOLETYPE, POINTER)
	if (kind == SYM_BASIC) {
		tok->dataType = sym->basicType;
		tok->symType = sym;
	}
	else {
		tok->symType = typ;
	}
	
	if (DEBUG & DB_FINDTYPE) {
		printf(" Finished findType().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

void instConst(TOKEN idtok, TOKEN consttok) {
	
	if (DEBUG & DB_INSTCONST) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instConstant(), from %s\n", last_method);
		dbugprint2args(idtok, consttok);
		last_method = "instConst()";
	}
	
	SYMBOL sym;
	
	sym = insertsym(idtok->stringval);
	sym->kind = SYM_CONST;
	sym->basicType = consttok->dataType;
	
	// SYS_CON
	if (consttok->tokenType == RESERVED &&
		consttok->whichval == SYS_CON - RESERVED_BIAS) {
		if (strcmp(consttok->stringval, "true") == 0) {
			sym->basicType = TYPE_BOOL;
			sym->constval.intNum = 1;
			sym->size = basicsizes[TYPE_BOOL];
		}
		else if (strcmp(consttok->stringval, "false") == 0) {
			sym->basicType = TYPE_BOOL;
			sym->constval.intNum = 0;
			sym->size = basicsizes[TYPE_BOOL];
		}
		if (strcmp(consttok->stringval, "maxint") == 0) {
			sym->basicType = TYPE_INT;
			sym->constval.intNum = INT_MAX;
			sym->size = basicsizes[TYPE_INT];
		}
	}
	else if (sym->basicType == TYPE_INT) {      // INTEGER
		sym->constval.intNum = consttok->intval;
		sym->size = basicsizes[TYPE_INT];   // 4
	}
	else if (sym->basicType == TYPE_REAL) {   // REAL
		sym->constval.realNum = consttok->realval;
		sym->size = basicsizes[TYPE_REAL];  // 8
	}
	else if (sym->basicType == TYPE_STR) {
		strncpy(sym->constval.stringConst, consttok->stringval, 16);
		sym->size = basicsizes[TYPE_STR];
	}
	else if (sym->basicType == TYPE_CHAR) {
		sym->constval.charConst = consttok->charval;
		sym->size = basicsizes[TYPE_CHAR];
	}
	
	if (DEBUG & DB_INSTCONST) {
		printf(" Finished instConstant().\n");
		dbugprint2toks(idtok, consttok);
	}
	
}

TOKEN binop(TOKEN op, TOKEN lhs, TOKEN rhs) {
	
	if (DEBUG & DB_BINOP) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In binop(), from %s\n", last_method);
		dbugprint3args(op, lhs, rhs);
		last_method = "binop()";
	}
	
	int lhs_dataType = lhs->dataType;
	int rhs_dataType = rhs->dataType;
	int op_type = op->whichval;
	
	/* Type checking/coercion needed. */
	
	if (lhs_dataType != rhs_dataType) {
		op = binop_type_coerce(op, lhs, rhs);
	}
	else {
		op->dataType = lhs->dataType;
		op->operands = lhs;     // link operands to operator
		lhs->link = rhs;     // link second operand to first
		rhs->link = NULL;    // terminate operand list
	}
	
	if (DEBUG & DB_BINOP) {
		printf(" Finished binop().\n");
		dbugprint1tok(op);
		last_method = "binop()";
		//    ppexpr(op);
	}
	
	return op;
}

TOKEN binop_type_coerce(TOKEN op, TOKEN lhs, TOKEN rhs) {
	
	TOKEN cast_tok;

	int lhs_dataType = lhs->dataType;
	int rhs_dataType = rhs->dataType;
	int op_type = op->whichval;
	
	if (lhs_dataType == TYPE_INT && rhs_dataType == TYPE_REAL) {
		// LHS is integer, RHS is real.
		// Fix if OP_ASSIGN, else float the integer.
		
		op->dataType = TYPE_REAL;
		
		if (op_type == OP_ASSIGN) {
			cast_tok = makefix(rhs);
			op->operands = lhs;
			lhs->link = cast_tok;
		}
		else {
			cast_tok = makefloat(lhs);
			op->operands = cast_tok;
			cast_tok->link = rhs;
		}
	}
	else if (lhs_dataType == TYPE_REAL && rhs_dataType == TYPE_INT) {
		// LHS is real, RHS is integer.
		// Float the integer value.
		// DO NOT fix; lhs still takes precedence.
		
		cast_tok = makefloat(rhs);
		
		op->dataType = TYPE_REAL;
		op->operands = lhs;
		lhs->link = cast_tok;
		cast_tok->link = NULL;
		rhs->link = NULL;
	}
	else {
		op->dataType = lhs->dataType;
		op->operands = lhs;     // link operands to operator
		lhs->link = rhs;     // link second operand to first
		rhs->link = NULL;    // terminate operand list
	}
	
	return op;
}
TOKEN makefix(TOKEN tok) {
	
	if (DEBUG & DB_MAKEFIX) {
		printf("(%d)\n", debug_call_num++);
		printf("In makefix(), from %s\n", last_method);
		last_method = "makefix()";
		dbugprint1arg(tok);
		last_method = "makefix()";
	}
	
	TOKEN out = makeOp(OP_FIX);   // create OP_FIX TOKEN
	if (!out) {
		printf(" Failed to alloc TOKEN, makefix().\n");
		return NULL;
	}
	
	out->operands = tok;
	out->link = NULL;
	
	if (DEBUG & DB_MAKEFIX) {
		printf(" Finished makefix().\n");
		dbugprint1tok(out);
		last_method = "makefix()";
	}
	
	return out;
}

TOKEN makefloat(TOKEN tok) {
  
  if (DEBUG & DB_MAKEFLOAT) {
    printf("(%d)\n", debug_call_num++);
    printf("In makefloat(), from %s\n", last_method);
    dbugprint1arg(tok);
    last_method = "makefloat()";
  }
  
  TOKEN out;
  
  if (tok->tokenType == TOKEN_NUM) {
    // e.g., floating 34 prints "3.400000e+01" to console
    out = tok;
    out->dataType = TYPE_REAL;
    out->realval = out->intval;
    out->intval = INT_MIN;
  }
  else {
    // e.g., floating 34 prints "(float 34)" to console
    out = makeOp(OP_FLOAT);
    if (!out) {
      printf(" Failed to alloc TOKEN, makefloat().\n");
      return NULL;
    }
    
    out->operands = tok;
    out->link = NULL;
  }
  
  if (DEBUG & DB_MAKEFLOAT) {
    printf(" Finished makefloat().\n");
    dbugprint1tok(out);
    last_method = "makefloat()";
  }
  
  return out;
}

void instVars(TOKEN idlist, TOKEN typetok) {
	
	if (DEBUG & DB_INSTVARS) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instVars(), from %s\n", last_method);
		printf(" typetok: ");
		ppexpr(typetok);
		dbugprint1tok(typetok);
		printf(" Installing...\n");
		last_method = "instVars()";
	}
	
	SYMBOL sym, typesym;
	int align;
	
	typesym = typetok->symType;
	align = alignsize(typesym);
	
	while (idlist != NULL) {
		sym = insertsym(idlist->stringval);
		sym->kind = SYM_VAR;
		sym->offset = wordaddress(blockoffs[blocknumber], align);
		sym->size = typesym->size;
		blockoffs[blocknumber] = sym->offset + sym->size;
		sym->dataType = typesym;
		sym->basicType = typesym->basicType;
		
		if (typesym->dataType != NULL && typesym->dataType->kind == SYM_ARRAY) {
			SYMBOL arr_type = typesym->dataType;
			while (arr_type && arr_type->kind == SYM_ARRAY) {
				arr_type = arr_type->dataType;
			}
			if (arr_type->kind == SYM_BASIC) {
				sym->basicType = arr_type->basicType;
			}
		}
		
		if (DEBUG & DB_INSTVARS) {
			printf(" ");
			dbugprinttok(idlist);
		}
		
		idlist = idlist->link;
	}
	
	if (DEBUG & DB_INSTVARS) {
		printf("\n");
	}
}

int wordaddress(int n, int wordsize) {
	return ((n + wordsize - 1) / wordsize) * wordsize;
}

TOKEN findId(TOKEN tok) {
	
	if (DEBUG & DB_FINDID) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In findId(), from %s\n", last_method);
		dbugprint1arg(tok);
		last_method = "findId()";
	}
	
	SYMBOL sym, typ;
	sym = searchst(tok->stringval);
	
	if (sym->kind == SYM_CONST) {
		tok->tokenType = TOKEN_NUM;
		
		if (sym->basicType == 0) {
			tok->dataType = TYPE_INT;
			tok->intval = sym->constval.intNum;
		}
		else if (sym->basicType == 1) {
			tok->dataType = TYPE_REAL;
			tok->realval = sym->constval.realNum;
		}
	}
	else {
		tok->symEntry = sym;
		typ = sym->dataType;
		tok->symType = typ;
		
		if (typ->kind == SYM_BASIC || typ->kind == SYM_POINTER) {
			tok->dataType = typ->basicType;
		}
	}
	
	if (DEBUG & DB_FINDID) {
		printf(" Finished findId().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

void instType(TOKEN typename, TOKEN typetok) {
	if (DEBUG & DB_INSTTYPE) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instType(), from %s\n", last_method);
		dbugprint2args(typename, typetok);
		printf(" Installing...\n");
		last_method = "instType()";
	}
	
	SYMBOL sym, typesym;
	
	typesym = typetok->symType;
	
	sym = searchins(typename->stringval);  // insert if not found
	sym->kind = SYM_TYPE;
	sym->size = typesym->size;
	sym->dataType = typesym;
	sym->basicType = typesym->basicType;
	
	if (DEBUG & DB_INSTTYPE) {
		printf(" ");
		dbugprinttok(typename);
		printf(" ");
		dbprsymbol(sym);
		printf(" ");
		dbprsymbol(typesym);
		printf("\n");
	}
	
}

TOKEN instEnum(TOKEN idlist) {
	if (DEBUG & DB_INSTENUM) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instEnum(), from %s\n", last_method);
		dbugprint1arg(idlist);
		last_method = "instEnum()";
	}
	
	int total_size = 0;
	TOKEN temp = idlist;
	while (temp) {
		instConst(temp, makeIntc(total_size));
		total_size++;
		temp = temp->link;
	}
	
	TOKEN subrange_tok = makeSubrange(idlist, 0, (total_size - 1));
	
	if (DEBUG & DB_INSTENUM) {
		printf(" Finished instEnum().\n");
		dbugprint1tok(subrange_tok);
		last_method = "instEnum()";
	}
	
	return subrange_tok;
}

TOKEN makeIntc(int num) {
	
	if (DEBUG & DB_MAKEINTC) {
		printf("(%d) line %d\n", debug_call_num++, lineCnt);
		printf("In makeIntc(), from %s\n", last_method);
		printf(" Arguments:\n  %d\n\n", num);
		last_method = "makeIntc()";
	}
	
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makeIntc().\n");
		return NULL;
	}
	
	out->tokenType = TOKEN_NUM;
	out->dataType = TYPE_INT;
	out->intval = num;
	
	if (DEBUG & DB_MAKEINTC) {
		printf(" Finished makeIntc().\n");
		dbugprint1tok(out);
	}
	
	return out;
}


TOKEN makeSubrange(TOKEN tok, int low, int high) {
	if (DEBUG & DB_MAKESUBRANGE) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeSubrange(), from %s\n", last_method);
		dbugprint1arg(tok);
		printf("  low bound: %d\n  high bound: %d\n\n", low, high);
		last_method = "makeSubrange()";
	}
	if (low > high) {
		printf(" Error: low bound cannot be greater than high bound\n\n");
		return NULL;
	}
	
	TOKEN out = copyTok(tok);
	
	SYMBOL subrange_entry = symalloc();
	subrange_entry->kind = SYM_SUBRANGE;
	subrange_entry->basicType = TYPE_INT;
	subrange_entry->lowBound = low;
	subrange_entry->highBound = high;
	subrange_entry->size = basicsizes[TYPE_INT]; // each entry is a constant (int)
	
	out->symType = subrange_entry;
	
	if (DEBUG & DB_ADDOFFS) {
		printf(" Finished makeSubrange().\n");
		dbugprint1tok(out);
		last_method = "makeSubrange()";
	}
	
	return out;
}

TOKEN copyTok(TOKEN origtok) {
	
	if (DEBUG & DB_COPYTOK) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In copyTok(), from %s\n", last_method);
		dbugprint1arg(origtok);
		last_method = "copyTok()";
	}
	
	if (!origtok) {
		return NULL;
	}
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, copyTok().\n");
		return NULL;
	}
	
	out->tokenType = origtok->tokenType;
	out->dataType = origtok->dataType;
	
	/* Shallow copies only (as expected). */
	out->symType = origtok->symType;
	out->symEntry = origtok->symEntry;
	out->operands = origtok->operands;
	out->link = origtok->link;
	
	if (origtok->stringval[0] != '\0') {
		strncpy(out->stringval, origtok->stringval, 16);
	}
	else if (origtok->whichval != -1) {
		out->whichval = origtok->whichval;
	}
	else if (origtok->realval != -DBL_MIN) {
		out->realval = origtok->realval;
	}
	else {
		out->intval = origtok->intval;
	}
	
	if (DEBUG & DB_COPYTOK) {
		printf(" Finished copyTok().\n");
		printf("  Original:\n    ");
		dbugprinttok(origtok);
		printf("  Copy:\n    ");
		dbugprinttok(out);
		printf("\n");
	}
	
	return out;
}