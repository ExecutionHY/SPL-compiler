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
void senmaticError(char* s);

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

TOKEN makePnb(TOKEN tok, TOKEN statements) {
	
	if (statements->whichval == OP_PROGN && ELIM_NESTED_PROGN) {
		if (DEBUG & DB_MAKEPNB) {
			printf("(%d) line: %d\n", debug_call_num++, lineCnt);
			printf("In makePnb(), from %s\n", last_method);
			dbugprint2args(tok, statements);
			printf(" Finished makePnb().\n");
			dbugprint1tok(statements);
		}
		return statements;
	}
	if (DEBUG & DB_MAKEPNB) {
		printf("(%d)\n", debug_call_num++);
		printf("In makePnb(), from %s; transferring to makeprogn()...\n", last_method);
	}
	return (makeProgn(tok, statements));
}

TOKEN makeProgn(TOKEN tok, TOKEN statements) {
	
	if (DEBUG & DB_MAKEPROGN) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeProgn(), from %s\n", last_method);
		dbugprint2args(tok, statements);
		last_method = "makeProgn()";
	}
	
	tok->tokenType = OPERATOR;    // 0
	tok->whichval = OP_PROGN;     // 22
	tok->operands = statements;
	
	if (DEBUG & DB_MAKEPROGN) {
		printf(" Finished makeProgn().\n");
		dbugprint2toks(tok, statements);
	}
	
	return tok;
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
		char s[64];
		sprintf(s, "type \"%s\" not found in symbol table", tok->stringval);
		senmaticError(s);
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
		printf(" Installing at block %d...\n", blocknumber);
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

TOKEN instDotdot(TOKEN lowtok, TOKEN dottok, TOKEN hightok) {
	if (DEBUG & DB_INSTDOTDOT) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instDotdot(), from %s\n", last_method);
		dbugprint3args(lowtok, dottok, hightok);
		last_method = "instDotdot()";
	}
	int lowb, highb;

	// ID DOTDOT ID
	if (lowtok->tokenType == TOKEN_ID && hightok->tokenType == TOKEN_ID) {
		SYMBOL sym1 = searchst(lowtok->stringval);
		SYMBOL sym2 = searchst(hightok->stringval);
	
		if (sym1->kind == SYM_CONST && sym2->kind == SYM_CONST &&
			sym1->basicType == TYPE_INT && sym2->basicType == TYPE_INT) {
			lowb = sym1->constval.intNum;
			highb = sym2->constval.intNum;
		}
		else senmaticError("wrong type for DOTDOT");
	}
	// INT DOT INT
	else if (lowtok->tokenType == TOKEN_NUM && hightok->tokenType == TOKEN_NUM &&
		lowtok->dataType == TYPE_INT && hightok->dataType == TYPE_INT) {
		lowb = lowtok->intval;
		highb = hightok->intval;
	}
	else senmaticError("wrong type for DOTDOT");


	TOKEN out = makeSubrange(dottok, lowtok->intval, hightok->intval);
	
	if (DEBUG & DB_INSTDOTDOT) {
		printf(" Finished instDotdot().\n");
		dbugprint1tok(out);
		last_method = "instDotdot()";
	}
	
	return out;
}

TOKEN instArray(TOKEN bounds, TOKEN typetok) {
	if (DEBUG & DB_INSTARRAY) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instArray(), from %s\n", last_method);
		dbugprint2args(bounds, typetok);
		//    dbprsymbol(typetok->symType);
		last_method = "instArray()";
	}
	
	// bounds->symType->[low/highBound]
	
	TOKEN curr_bound = bounds;
	SYMBOL prev_sym = NULL;
	
	SYMBOL typesym = searchst(typetok->stringval);
	int low, high;
	
	if (!typesym) {
		char s[64];
		sprintf(s, "array \"%s\" not found in symbol table", typetok->stringval);
		senmaticError(s);
	}
	
	while (curr_bound) {
		SYMBOL arrsym = symalloc();
		arrsym->kind = SYM_ARRAY;
		
		if (typesym) {
			arrsym->dataType = typesym;
		}
		else {
			//       arrsym->basicType = typetok->dataType;
		}
		
		low = curr_bound->symType->lowBound;
		high = curr_bound->symType->highBound;
		
		if (!prev_sym) {
			arrsym->size = (high - low + 1) * typesym->size;
		}
		else {
			arrsym->dataType = typetok->symType;
			arrsym->size = (high - low + 1) * prev_sym->size;
		}
		
		typetok->symType = arrsym;
		prev_sym = arrsym;
		
		arrsym->lowBound = low;
		arrsym->highBound = high;
		
		curr_bound = curr_bound->link;
	}
	
	if (DEBUG & DB_INSTARRAY) {
		printf(" Finished instArray().\n");
		dbugprint1tok(typetok);
	}
	
	return typetok;
}

TOKEN instRec(TOKEN rectok, TOKEN argstok) {
	if (DEBUG & DB_INSTREC) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instRec(), from %s\n", last_method);
		dbugprint2args(rectok, argstok);
		last_method = "instRec()";
	}
	
	int total_size, offset;
	
	SYMBOL recsym = symalloc();
	recsym->kind = SYM_RECORD;
	rectok->symType = recsym;
	recsym->dataType = argstok->symType;
	offset = wordaddress(argstok->symType->size, 8);   // TODO: test this w/ something size 8
	total_size = offset;
	
	if (DEBUG & DB_INSTREC) {
		printf(" Installing...\n");
		dbugprint1sym(recsym->dataType);
	}
	
	TOKEN curr = argstok;
	TOKEN next = argstok->link;
	while (next) {
		/* Link the symTypes of each TOKEN (the record fields) together
		 and calculate the total size of the record */
		
		curr->symType->link = next->symType;
		offset = wordaddress(next->symType->size, 8);
		next->symType->offset = total_size;
		total_size += offset;
		
		if (DEBUG & DB_INSTREC) {
			printf("  ");
			dbprsymbol(next->symType);
		}
		
		curr = next;
		next = next->link;
	}
	
	recsym->size = total_size;
	
	if (DEBUG & DB_INSTREC) {
		printf(" Finished instRec().\n");
		dbugprint1tok(rectok);
	}
	
	return rectok;
}

TOKEN instFields(TOKEN idlist, TOKEN typetok) {
	if (DEBUG & DB_INSTFIELDS) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instFields(), from %s\n", last_method);
		printf(" typetok: ");
		ppexpr(typetok);
		dbugprinttok(typetok);
		printf("\n Installing...\n");
		last_method = "instFields()";
	}

	int next = 0;
	int offset = 0;

	SYMBOL recsym, typesym;
	typesym = searchst(typetok->stringval);

	TOKEN temp = idlist;

	/* Set the symtype of each record field (each TOKEN in idlist).
	   These will be linked together in instrec(). */
	while (temp) {
		recsym = makesym(temp->stringval);
		recsym->dataType = typesym;

		// offset = next; next = next + n;
		offset = next;
		next = next + typesym->size;

		recsym->offset = offset;
		recsym->size = typesym->size;
		recsym->dataType = typesym;

		if (typesym->kind == SYM_BASIC) {
			recsym->basicType = typesym->basicType;
		}

		temp->symType = recsym;

		if (DEBUG & DB_INSTFIELDS) {
			dbprsymbol(typesym);
			dbugprintsym(recsym);
		}

		temp = temp->link;
	}

	if (DEBUG & DB_INSTFIELDS) {
//		printf(" Finished instFields().\n");
		printf("\n");
	}

	return idlist;
}

TOKEN doLabel(TOKEN labeltok, TOKEN tok, TOKEN statement) {
	
	if (DEBUG & DB_DOLABEL) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In doLabel(), from %s\n", last_method);
		dbugprint3args(labeltok, tok, statement);
		last_method = "doLabel()";
	}
	
	int internal_label_num = get_internal_label_num(labeltok->intval);
	if (internal_label_num == -1) {
		printf(" Error: label %d not found in label table\n", labeltok->intval);
		return NULL;
	}
	
	TOKEN do_progn_tok = makeOp(OP_PROGN);
	TOKEN label_tok = makeOp(OP_LABEL);
	// TOKEN do_progn_tok = makeprogn(makeop(OP_PROGN), makeop(OP_LABEL));  // second arg will never be progn, so skip makepnb()
	TOKEN label_num_tok = makeIntc(internal_label_num);
	
	if (!do_progn_tok || !label_tok || !label_num_tok) {
		printf(" Failed to alloc label TOKEN(s), doLabel().\n");
		return NULL;
	}
	
	do_progn_tok->operands = label_tok;
	label_tok->operands = label_num_tok;
	label_tok->link = statement;
	
	if (DEBUG & DB_DOLABEL) {
		printf(" Finished doLabel().\n");
		dbugprint1tok(do_progn_tok);
		last_method = "doLabel()";
	}
	
	return do_progn_tok;
}


TOKEN arrayRef(TOKEN arr, TOKEN tok, TOKEN subs, TOKEN tokb) {
	if (DEBUG & DB_ARRAYREF) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In arrayRef(), from %s\n", last_method);
		dbugprint4args(arr, tok, subs, tokb);
		last_method = "arrayRef()";
	}
	
	TOKEN array_ref = NULL;
	SYMBOL arr_varsym, typesym, temp;
	SYMBOL arrsyms[10];
	
	arr_varsym = searchst(arr->stringval);
	if (!arr_varsym) {
		char s[64];
		sprintf(s, "array \"%s\" not found in symbol table", arr->stringval);
		senmaticError(s);
	}
	
	temp = arr_varsym->dataType;
	
	int counter = 0;
	int num_arr_dims = 0;   // not being used for anything
	
	// Store the SYM_ARRAYs into an array
	while (temp && temp->kind != SYM_TYPE) {
		arrsyms[counter] = temp;
		num_arr_dims++;
		counter++;
		temp = temp->dataType;
	}
	
	// The type of the array
	// arr_varsym->dataType
	
	// only one const integer
	if (subs->link == NULL && subs->tokenType == TOKEN_NUM && subs->dataType == TYPE_INT) {
		int offset = (subs->intval-arr_varsym->dataType->lowBound) * arr_varsym->dataType->dataType->size;
		array_ref = makeAref(arr, makeIntc(offset), NULL);
		array_ref->dataType = arr_varsym->basicType;
		return array_ref;
	}
	// else if tokenType is IDENTIFIER?
	

	counter = 0;
	TOKEN curr_sub = subs;
	
	while (curr_sub) {
		
		if (counter == 0) {     // first iteration; need to make aref
			SYMBOL curr_sym = arrsyms[counter];
			int curr_size = curr_sym->size / (curr_sym->highBound - curr_sym->lowBound + 1);
			
			// TOKEN mul_op = maketimes(makeintc(curr_size), curr_sub, NULL);
			
			TOKEN mul_op = makeOp(OP_MUL);
			TOKEN pos_typesym_size = makeIntc(curr_size);
			TOKEN neg_typesym_size = makeIntc(curr_size * -curr_sym->lowBound);
			
			mul_op->operands = pos_typesym_size;
			pos_typesym_size->link = curr_sub;
			
			neg_typesym_size->link = mul_op;
			
			TOKEN plus_op = makePlus(neg_typesym_size, mul_op, NULL);
			
			array_ref = makeAref(arr, plus_op, NULL);
			array_ref->dataType = arr_varsym->basicType;
			
		}
		else {
			
			if (curr_sub->tokenType == TOKEN_NUM) {      // smash constants (optimization)
				TOKEN add_to = array_ref->operands->link->operands;
				add_to = addInt(add_to, makeIntc(curr_sub->whichval * typesym->size), NULL);
			}
			else {
				// Smash constants and add to tree
				
				SYMBOL curr_sym = arrsyms[counter];
				int curr_size = curr_sym->size / (curr_sym->highBound - curr_sym->lowBound + 1);
				
				TOKEN mul_op = makeOp(OP_MUL);
				TOKEN pos_typesym_size = makeIntc(curr_size);
				TOKEN neg_typesym_size = makeIntc(curr_size * -1);
				
				mul_op->operands = pos_typesym_size;
				pos_typesym_size->link = curr_sub;
				
				TOKEN added = array_ref->operands->link->operands;
				added = addInt(added, neg_typesym_size, NULL);
				
				TOKEN add_to = array_ref->operands->link->operands->link;
				TOKEN plus_op = makePlus(add_to, mul_op, NULL);
				add_to = plus_op;
				
			}
		}
		
		// Destroy the current link
		TOKEN unlink = curr_sub;
		curr_sub = curr_sub->link;
		unlink->link = NULL;
		counter++;
	}
	
	if (DEBUG & DB_ARRAYREF) {
		printf(" Finished arrayRef().\n");
		dbugprint1tok(array_ref);
		last_method = "arrayRef()";
	}
	
	return array_ref;
}


TOKEN makeAref(TOKEN var, TOKEN off, TOKEN tok) {
	if (DEBUG & DB_MAKEAREF) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeAref(), from %s\n", last_method);
		dbugprint3args(var, off, tok);
		last_method = "makeAref()";
	}
	
	TOKEN aref = makeOp(OP_AREF);
	aref->operands = var;
	var->link = off;
	
	if (DEBUG & DB_MAKEAREF) {
		printf(" Finished makeAref().\n");
		dbugprint1tok(aref);
		last_method = "makeAref()";
	}
	
	return aref;
}

TOKEN makePlus(TOKEN lhs, TOKEN rhs, TOKEN tok) {
	
	if (DEBUG & DB_MAKEPLUS) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makePlus(), from %s\n", last_method);
		dbugprint3args(lhs, rhs, tok);
		last_method = "makePlus()";
	}
	
	TOKEN out = makeOp(OP_PLUS);
	if (lhs && rhs) {
		out->operands = lhs;
		lhs->link = rhs;
		rhs->link = NULL;
	}
	
	if (DEBUG & DB_MAKEPLUS) {
		printf(" Finished makePlus().\n");
		dbugprint1tok(out);
		last_method = "makePlus()";
	}
	
	return out;
}

TOKEN addInt(TOKEN exp, TOKEN off, TOKEN tok) {
	if (!exp || !off) {
		return NULL;
	}
	
	int a = exp->intval;
	int b = off->intval;
	
	if (DEBUG & DB_ADDINT) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In addInt(), from %s\n Adding %d to %d\n", last_method, b, a);
		dbugprint3args(exp, off, tok);
		last_method = "addInt()";
	}
	
	if ((b > 0) && (a > INT_MAX - b)) { // if ((INT_MAX / exp->intval) > off->intval)
		printf(" Error: integer overflow detected, addInt()\n");
		printf(" Cannot add %d to %d\n", b, a);
	}
	else {
		exp->intval = a + b;
	}
	
	if (DEBUG & DB_ADDINT) {
		printf(" New value of exp: %d\n", exp->intval);
		dbugprint1tok(exp);
	}
	
	return exp;
}


TOKEN makeFuncall(TOKEN tok, TOKEN fn, TOKEN args) {
	
	if (DEBUG & DB_MAKEFUNCALL) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeFuncall(), from %s\n", last_method);
		dbugprint3args(tok, fn, args);
		last_method = "makeFuncall()";
	}
	
	TOKEN funcall_tok;
			
	funcall_tok = makeOp(OP_FUNCALL);      // 24
	if (!funcall_tok) {
		printf(" Failed to allocate TOKEN, makeFuncall().\n");   // according to the Prof, print message and coredump
		return NULL;
	}
	
	SYMBOL this_fxn = searchst(fn->stringval);
	if (!this_fxn) {
		char s[64];
		sprintf(s, "failed to find function with name \"%s\" in symbol table.", fn->stringval);
		senmaticError(s);
	}
	
	funcall_tok->dataType = this_fxn->dataType->basicType;
	
	// TODO!!
	// Check for type compatibility between the write functions and
	// the arguments. Correct if necessary.
	if (strcmp(fn->stringval, "write") == 0 || strcmp(fn->stringval, "writeln") == 0) {
		// do not edit funcall
		//fn = write_fxn_args_type_check(fn, args);
		if (!fn) {
			return NULL;
		}
	}
	
	funcall_tok->operands = fn;
	fn->link = args;

	
	if (DEBUG & DB_MAKEFUNCALL) {
		printf(" Finished makeFuncall().\n");
		dbugprint3toks(funcall_tok, funcall_tok->operands, funcall_tok->operands->link);
		//    ppexpr(funcall_tok);
		last_method = "makeFuncall()";
	}
	
	return funcall_tok;
}

// TODO: assumes fn is always a write() or writeln() function
// TODO: does not do cross-conversions (e.g., send in writelnf() to convert to writelni())
// TODO: identifier dataTypes?
TOKEN write_fxn_args_type_check(TOKEN fn, TOKEN args) {
	
	if (args->dataType == TYPE_STR) {
		return fn;
	}
	
	TOKEN out = NULL;
	
	SYMBOL fn_sym = searchst(fn->stringval);
	if (!fn_sym) {
		printf(" Error: function \"%s\" is not defined.\n", fn->stringval);
		return out;
	}
	
	int fn_arg_type = fn_sym->dataType->link->basicType;
	int args_type = args->dataType;
	
	if (args_type == TYPE_STR) {
		out = fn;
	}
	else {
		
		int replace_index = 5;
		if (strcmp(fn->stringval, "writeln") == 0) {
			replace_index = 7;
		}
		
		if (strcmp(fn->stringval, "write") == 0) {
			
			if (args_type == TYPE_INT) {
				fn->stringval[replace_index] = 'i';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			else if (args_type == TYPE_REAL) {
				fn->stringval[replace_index] = 'f';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			
		}
		else if (strcmp(fn->stringval, "writeln") == 0) {
			
			if (args_type == TYPE_INT) {
				fn->stringval[replace_index] = 'i';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			else if (args_type == TYPE_REAL) {
				fn->stringval[replace_index] = 'f';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			
		}
	}
	
	return out;
}

TOKEN unaryop(TOKEN op, TOKEN lhs) {
	
	if (DEBUG & DB_UNARYOP) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In unaryop(), from %s\n", last_method);
		dbugprint2args(op, lhs);
		last_method = "unaryop()";
	}
	
	op->operands = lhs;
	lhs->link = NULL;
	
	if (DEBUG & DB_UNARYOP) {
		printf(" Finished unaryop().\n");
		dbugprint2toks(op, lhs);
	}
	
	return op;
}

/* Helper method for reducedot(). Searches for the offset
 of the given field in var. Returns a TOKEN_NUM whose
 whichval field is the offset if found, otherwise returns
 with a whichval of -1. */
TOKEN get_offset(TOKEN var, TOKEN field) {
	
	TOKEN offset = makeIntc(-1);
	
	TOKEN root_name = get_last_operand(var);
	TOKEN last_offset = get_last_link(var->operands);
	
	SYMBOL found = NULL;
	SYMBOL varsym = searchst(root_name->stringval);
	
	int var_is_arefop = 0;
	if (var->whichval == OP_AREF) {
		var_is_arefop = 1;
	}
	
	if (!varsym) {
		printf(" Error: could not find symbol \"%s\" in symbol table, get_offset():1.\n", root_name->stringval);
		return NULL;
	}
	
	SYMBOL temp = varsym;
	while (temp) {
		if (temp->dataType && temp->dataType->kind == SYM_BASIC) {
			break;
		}
		temp = temp->dataType;
	}
	
	if (!temp) {
		printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():2.\n", root_name->stringval);
		return NULL;
	}
	
	while (temp) {
		
		if ((strcmp(temp->nameString, field->stringval) == 0)) {
			found = temp;
			if (var_is_arefop) {
				
				// TOKEN var is an aref TOKEN; ignore 
				if (last_offset->tokenType == OPERATOR) {
					offset->whichval = -1;
				}
				else {
					offset->whichval = last_offset->whichval + found->offset;
				}
				
				// Set offset's link field to a REAL TOKEN_NUM.
				// Is necessary to ensure correct behaviour of
				// binop() during type checking/coercion.
				 
				// The link MUST be nulled out after use, otherwise
				// the console output will be incorrect.
				if (found->dataType->basicType == TYPE_REAL) {
					offset->link = makeRealTok(0);
					offset->link->realval = -DBL_MAX;
				}
			}
			else {
				
				offset->whichval = found->offset;
				if (found->dataType->basicType == TYPE_REAL) {
					offset->link = makeRealTok(0);
					offset->link->realval = -DBL_MAX;
				}
			}
			
			return offset;
		}
		else if (var_is_arefop && (temp->offset == last_offset->whichval)) {
			found = temp;
			break;
		}
		
		temp = temp->link;
	}
	
	/* NOT an error condition if !found here. */
	
	if (!var_is_arefop && found) {
		offset->whichval = found->offset;
		if (found->dataType->basicType == TYPE_REAL) {
			offset->link = makeRealTok(0);
			offset->link->realval = -DBL_MAX;
		}
		return offset;
	}
	else if (var_is_arefop && found) {
		/* TOKEN field represents a record field; search for
		 the record in the symbol table and then search for
		 the field within that record. */
		
		SYMBOL temp1 = searchst(found->dataType->nameString);
		found = NULL;
		if (!temp1) {
			printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():3.\n", root_name->stringval);
			return NULL;
		}
		
		while (temp1) {
			if (temp1->dataType && temp1->dataType->kind == SYM_BASIC) {
				break;
			}
			temp1 = temp1->dataType;
		}
		
		if (!temp1) {
			printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():4.\n", root_name->stringval);
			return NULL;
		}
		
		while (temp1 && !found) {
			if (strcmp(temp1->nameString, field->stringval) == 0) {
				found = temp1;
			}
			else {
				temp1 = temp1->link;
			}
		}
		
		/* NOT an error condition if !found here. */
		if (found) {
			offset->whichval = last_offset->whichval + found->offset;
			if (found->dataType->basicType == TYPE_REAL) {
				offset->link = makeRealTok(0);
				offset->link->realval = -DBL_MAX;
			}
			
			return offset;
		}
	}
	
	return offset;
}

TOKEN get_last_link(TOKEN tok) {
	if (!tok) {
		return NULL;
	}
	
	TOKEN curr = tok;
	TOKEN curr_link = curr->link;
	while (curr_link) {
		curr = curr_link;
		curr_link = curr_link->link;
	}
	
	return curr;
}
TOKEN get_last_operand(TOKEN tok) {
	if (!tok) {
		return NULL;
	}
	
	TOKEN curr = tok;
	TOKEN curr_operand = curr->operands;
	while (curr_operand) {
		curr = curr_operand;
		curr_operand = curr_operand->operands;
	}
	
	return curr;
}
TOKEN makeRealTok(float num) {
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makeRealTok().\n");
		return NULL;
	}
	
	out->tokenType = TOKEN_NUM;
	out->dataType = TYPE_REAL;
	out->realval = num;
	
	return out;
}

TOKEN reduceDot(TOKEN var, TOKEN dot, TOKEN field) {
	if (DEBUG & DB_REDUCEDOT) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In reduceDot(), from %s\n", last_method);
		dbugprint3args(var, dot, field);
		last_method = "reduceDot()";
	}
	
	TOKEN aref;
	TOKEN offset = get_offset(var, field);
	
	if (var->whichval == OP_AREF) {
		// Avoid adding multiple copies to the tree.
		
		if (offset->whichval >= 0) {
			var->operands->link = offset;
		}
		aref = var;
	}
	else {
		aref = makeAref(var, offset, NULL);
	}
	
	// Change aref's dataType to REAL to avoid incorrect
	// fixes/floats in binop(). NULL out the dummy link.
	if (offset->link && offset->link->dataType == TYPE_REAL &&
	offset->link->realval == -DBL_MAX) {
		
		aref->dataType = TYPE_REAL;
		offset->link = NULL;
	}
	
	if (DEBUG & DB_REDUCEDOT) {
		printf(" Finished reduceDot().\n");
		dbugprint1tok(aref);
		last_method = "reduceDot()";
		// ppexpr(aref);
		// printf("\n");
	}
	
	return aref;
}

TOKEN makeIf(TOKEN tok, TOKEN exp, TOKEN thenpart, TOKEN elsepart) {
	
	if (DEBUG & DB_MAKEIF) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeIf(), from %s\n", last_method);
		dbugprint4args(tok, exp, thenpart, elsepart);
		last_method = "makeIf()";
	}
	
	tok->tokenType = OPERATOR; /* Make it look like an operator. */
	tok->whichval = OP_IF;
	
	if (elsepart != NULL) {
		elsepart->link = NULL;
	}
	
	thenpart->link = elsepart;
	exp->link = thenpart;
	tok->operands = exp;
	
	if (DEBUG & DB_MAKEIF) {
		printf(" Finished makeIf().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

TOKEN makeRepeat(TOKEN tok, TOKEN statements, TOKEN tokb, TOKEN expr) {
	
	if (DEBUG & DB_MAKEREPEAT) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeRepeat(), from %s\n", last_method);
		dbugprint4args(tok, statements, tokb, expr);
		last_method = "makeRepeat()";
	}
	
	TOKEN label_tok = makeLabel();
	TOKEN goto_tok = makeGoto(label_tok->operands->intval);
	TOKEN rpt_progn_tok = makePnb(talloc(), label_tok);   // operand label_tok to rpt_progn_tok
	TOKEN do_progn_tok = makeOp(OP_PROGN);
	TOKEN ifop_tok = makeIf(makeOp(OP_IF), expr, do_progn_tok, NULL);
	
	if (!label_tok || !goto_tok || !rpt_progn_tok ||
	!do_progn_tok ||!ifop_tok) {
		printf(" Failed to alloc TOKEN(s), makeRepeat().\n");
		return NULL;
	}
	
	/* Link TOKENs together */
	label_tok->link = statements;
	do_progn_tok->link = goto_tok;
	
	/* Handle elimination of nested progns */
	get_last_link(statements)->link = ifop_tok;
	
	if (DEBUG & DB_MAKEREPEAT) {
		printf(" Finished makeRepeat().\n");
		dbugprint1tok(rpt_progn_tok);
		last_method = "makeRepeat()";
	}
	
	return rpt_progn_tok;
}

TOKEN makeLabel() {
	
	// DO NOT CALL FOR USER LABELS
	
	if (DEBUG & DB_MAKELABEL) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeLabel(), from %s\n\n", last_method);
		last_method = "makeLabel()";
	}
	
	TOKEN label_tok = makeOp(OP_LABEL);
	TOKEN label_num_tok = makeIntc(labelnumber++);  // increment it after creation
	
	if (!label_tok || !label_num_tok) {
		printf(" Failed to alloc TOKEN(s), makeLabel().\n");
		return NULL;
	}
	
	label_tok->operands = label_num_tok;   // operand together
	
	if (DEBUG & DB_MAKELABEL) {
		printf(" Finished makeLabel().\n");
		dbugprint1tok(label_tok);
		last_method = "makeLabel()";
	}
	
	return label_tok;
}

TOKEN makeGoto(int label) {
	
	if (DEBUG & DB_MAKEGOTO) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeGoto(), from %s\n", last_method);
		if (DB_PRINT_ARGS) {
			printf(" Arguments:\n  %d\n\n", label);
		}
		last_method = "makeGoto()";
	}
	
	if (label < 0) {
		printf(" Warning: label number is negative (%d), makeGoto().\n", label);
	}
	
	TOKEN goto_tok = makeOp(OP_GOTO);
	TOKEN goto_num_tok = makeIntc(label);
	if (!goto_tok || !goto_num_tok) {
		printf(" Failed to alloc TOKEN, makeGoto().\n");
		return NULL;
	}
	
	goto_tok->operands = goto_num_tok;  // operand together
	
	if (DEBUG & DB_MAKEGOTO) {
		printf(" Finished makeGoto().\n");
		dbugprint1tok(goto_tok);
		last_method = "makeGoto()";
	}
	
	return goto_tok;
}

TOKEN makeWhile(TOKEN tok, TOKEN expr, TOKEN tokb, TOKEN statement) {
	
	if (DEBUG & DB_MAKEWHILE) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeWhile(), from %s\n", last_method);
		dbugprint4args(tok, expr, tokb, statement);
		last_method = "makeWhile()";
	}
	
	TOKEN label_tok = makeLabel();
	TOKEN goto_tok = makeGoto(label_tok->operands->intval);
	TOKEN while_progn_tok = makePnb(talloc(), label_tok); // operand label_tok to while_progn_tok
	TOKEN do_progn_tok = makePnb(talloc(), statement);    // operand statement to do_progn_tok
	TOKEN ifop_tok = makeIf(makeOp(OP_IF), expr, do_progn_tok, NULL);
	
	if (!label_tok || !goto_tok || !while_progn_tok ||
	!do_progn_tok ||!ifop_tok) {
		printf(" Failed to alloc TOKEN(s), makeWhile().\n");
		return NULL;
	}
	
	label_tok->link = ifop_tok;
	
	/* Handle elimination of nested progns */
	if (do_progn_tok->whichval != OP_PROGN) {
		do_progn_tok->operands = statement;
		statement->link = goto_tok;
	}
	else {   // do_progn_tok == statement
		get_last_link(do_progn_tok->operands)->link = goto_tok;
	}
	
	if (DEBUG & DB_MAKEWHILE) {
		printf(" Finished makeWhile().\n");
		dbugprint1tok(while_progn_tok);
		last_method = "makeWhile()";
	}
	
	return while_progn_tok;
}

/* A helper method used to create the TOKENs required to increment
 the counter var in a for-loop.
 
 Returned TOKENs are of the form "(:= i (+ i 1))".  */
TOKEN makeLoopIncr(TOKEN var, int incr_amt) {
	// if in makefor(), send in asg->operands
	
	TOKEN assignop = makeOp(OP_ASSIGN);
	TOKEN var_cpy = copyTok(var);
	TOKEN plusop = makePlus(copyTok(var), makeIntc(incr_amt), NULL);  // OP_PLUS operand "var" link amt
	
	assignop->operands = var_cpy;
	var_cpy->link = plusop;
	
	return assignop;
}

TOKEN makeFor(TOKEN tok, TOKEN asg, TOKEN dir,
	TOKEN endexpr, TOKEN tokc, TOKEN statement) {
	
	if (DEBUG & DB_MAKEFOR) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeFor(), from %s\n", last_method);
		dbugprint5args(tok, asg, endexpr, tokc, statement);
		last_method = "makeFor()";
	}

	int sign = 1;
	if (strcmp("to", dir->stringval) == 0) sign = 1;
	else if (strcmp("downto", dir->stringval) == 0) sign = -1;
	
	TOKEN for_asg_progn_tok = makePnb(talloc(), asg);
	TOKEN label_tok = makeLabel();
	
	TOKEN stop_op_tok = makeOp(OP_LE);
	TOKEN do_progn_tok = makePnb(talloc(), statement);
	TOKEN ifop_tok = makeIf(makeOp(OP_IF), stop_op_tok, do_progn_tok, NULL);
	
	TOKEN loop_stop_tok = copyTok(asg->operands); // the counter var, eg "i" in trivb.pas
	TOKEN stmt_incr_tok = makeLoopIncr(asg->operands, sign);
	TOKEN goto_tok;
	
	if (!for_asg_progn_tok || !label_tok || !ifop_tok || !stop_op_tok ||
	!do_progn_tok || !loop_stop_tok || !stmt_incr_tok) {
		printf(" Failed to alloc TOKEN(s), makeFor().\n");
		return NULL;
	}
	
	goto_tok = makeGoto(label_tok->operands->intval);  // TODO: not null-checked
	
	if (sign == -1) {
		stop_op_tok->whichval = OP_GE;   // "downto"
	}
	
	/* Link all the tokens together. */
	asg->link = label_tok;
	label_tok->link = ifop_tok;
	stop_op_tok->operands = loop_stop_tok;
	loop_stop_tok->link = endexpr;
	
	/* Handle elimination of nested progns */
	if (do_progn_tok->whichval != OP_PROGN) {
		do_progn_tok->operands = statement;
		statement->link = stmt_incr_tok;
	}
	else {   // do_progn_tok == statement
		get_last_link(do_progn_tok->operands)->link = stmt_incr_tok;
	}
	stmt_incr_tok->link = goto_tok;
	
	if (DEBUG & DB_MAKEFOR) {
		printf(" Finished makeFor().\n");
		dbugprint1tok(for_asg_progn_tok);
		last_method = "makeFor()";
	}
	
	return for_asg_progn_tok;
}

TOKEN doGoto(TOKEN tok, TOKEN labeltok) {
	
	// THIS METHOD SHOULD ONLY BE CALLED FOR A USER LABEL
	
	if (DEBUG & DB_DOGOTO) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In doGoto(), from %s\n", last_method);
		dbugprint2args(tok, labeltok);
		last_method = "doGoto()";
	}
	
	if (labeltok->intval < 0) {
		printf(" Warning: label number is negative, doGoto().\n");
	}
	
	int internal_label_num = get_internal_label_num(labeltok->intval);
	if (internal_label_num == -1) {
		printf(" Error: could not find internal label number corresponding to user label number %d\n", labeltok->intval);
		return NULL;
	}
	
	if (DEBUG & DB_DOGOTO) {
		printf(" Finished doGoto().\n");
		printf("  Found internal label number %d corresponding to user label number %d\n", internal_label_num, labeltok->intval);
		printf("  Transferring to makegoto()...\n\n");
	}
	
	return (makeGoto(internal_label_num));
}

TOKEN makeFunDcl(TOKEN head, TOKEN body) {
	if (DEBUG & DB_MAKEFUNDCL) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In makeFunDcl(), from %s\n", last_method);
		dbugprint2args(head, body);
		last_method = "makeFunDcl()";
	}
	TOKEN  fundcl_tok = makeOp(OP_FUNDCL);
	if (!fundcl_tok) {
		printf(" Failed to alloc TOKEN(s), makeLabel().\n");
		return NULL;
	}

	fundcl_tok->operands = head;
	fundcl_tok->link = body;

	lastblock = blocknumber;	// this is the last block
	blockoffs[blocknumber] = 0;
	blocknumber++;				// may be another function block
	contblock[blocknumber] = contblock[lastblock];

	if (DEBUG & DB_MAKEFUNDCL) {
		printf(" Finished makeFunDcl().\n");
		dbugprint1tok(fundcl_tok);
	}

	return fundcl_tok;
}

TOKEN instFun(TOKEN head) {
	if (DEBUG & DB_INSTFUN) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instFun(), from %s\n", last_method);
		last_method = "instFun()";
	}

	TOKEN fun_name = head->link;

	// if function
	if (strcmp(head->stringval, "function") == 0) {
		TOKEN funtype_tok = fun_name->link;
		TOKEN arg_tok = funtype_tok->link;
		SYMBOL funtype_sym = searchst(funtype_tok->stringval);
		if (!funtype_sym) {
			senmaticError("sorry we only support SYS_TYPE in functions & procedures");
		}

		SYMBOL arglist = symalloc();
		SYMBOL temp = arglist;
		while (arg_tok) {
			SYMBOL arg_sym = symalloc();
			if (arg_tok->tokenType == RESERVED && arg_tok->whichval == SYS_TYPE - RESERVED_BIAS) {
				if (strcmp(arg_tok->stringval, "integer")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_INT;
				}
				else if (strcmp(arg_tok->stringval, "real")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_REAL;
				}
				else if (strcmp(arg_tok->stringval, "string")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_STR;
				}
				else if (strcmp(arg_tok->stringval, "char")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_CHAR;
				}
				else if (strcmp(arg_tok->stringval, "boolean")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_BOOL;
				}
			}
			else {
				senmaticError("sorry we only support SYS_TYPE in functions & procedures");
			}
			temp->dataType = arg_sym;
			temp = arg_sym;
			arg_tok = arg_tok->link;
		}

		insertfnx(fun_name->stringval, funtype_sym, arglist);
	}
	// if procedure
	else {
		TOKEN arg_tok = fun_name->link;

		SYMBOL arglist = symalloc();
		SYMBOL temp = arglist;
		while (arg_tok) {
			SYMBOL arg_sym = symalloc();
			if (arg_tok->tokenType == SYS_TYPE) {
				if (strcmp(arg_tok->stringval, "integer")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_INT;
				}
				else if (strcmp(arg_tok->stringval, "real")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_REAL;
				}
				else if (strcmp(arg_tok->stringval, "string")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_STR;
				}
				else if (strcmp(arg_tok->stringval, "char")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_CHAR;
				}
				else if (strcmp(arg_tok->stringval, "boolean")) {
					arg_sym->kind = SYM_ARGLIST;
					arg_sym->basicType = TYPE_BOOL;
				}
			}
			else {
				senmaticError("sorry we only support SYS_TYPE in functions & procedures");
			}
			temp->dataType = arg_sym;
			temp = arg_sym;
			arg_tok = arg_tok->link;
		}

		insertfnx(fun_name->stringval, NULL, arglist);
	}

	if (DEBUG & DB_MAKEFUNDCL) {
		printf(" Finished instFun() at block %d.\n", contblock[blocknumber]);
	}

	return head;
}

void endVarPart() {
	int thisblock = blocknumber;
	blocknumber++;
	contblock[blocknumber] = thisblock;

	if (DEBUG) {
		printf(" endVarPart(), blocknumber = %d.\n", blocknumber);
	}
}

TOKEN endDecl(TOKEN decl) {
	blocknumber = contblock[blocknumber];
	if (DEBUG) {
		printf(" endDecl(), blocknumber = %d, cont = %d.\n", blocknumber, contblock[blocknumber]);
		dbugprint1tok(decl);
	}
	return decl;
}
