/**********************************************************
 *  File:     symtab.c
 *  Project:  SPL-compiler
 *  Author:   Execution
 *  Modified: Jun 3, 2017
 **********************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "token.h"

TOKENNODE user_labels = NULL;
TOKENNODE curr_label = NULL;


/* BASEOFFSET is the offset for the first variable */
#define BASEOFFSET 0

extern int lastblock;
extern int    blocknumber;       	/* Number of current block being compiled */
extern int    contblock[MAXBLOCKS];  /* Containing block for each block (the outer block of this block)        */
int    blockoffs[MAXBLOCKS] = {0};  		/* Storage offsets for each block         */
SYMBOL symtab[MAXBLOCKS][HASH_SIZE];     /* Symbol chain for each block            */
SYMBOL symend[MAXBLOCKS];     /* End of symbol chain for each block     */

/* Sizes of basic types  INTEGER  REAL  	CHAR  	BOOL 	 STRING   */
int basicsizes[5] =      { 4,       8,       1,         4,        16 };

char* symprint[10]  = {"ARGM", "BASIC", "CONST", "VAR", "SYM_SUBRANGE",
                       "FUNCTION", "ARRAY", "RECORD", "TYPE", "ARGLIST"};
int symsize[10] = { 1, 5, 5, 3, 8, 8, 5, 6, 4, 7 };

/* allocate a new symbol record */
SYMBOL symalloc() { 
	return((SYMBOL) calloc(1,sizeof(SYMBOLREC)));
}

/* Make a symbol table entry with the given name */
SYMBOL makesym(char name[]) {
	SYMBOL sym; int i;
    sym = symalloc();
    for ( i = 0; i < 16; i++) sym->nameString[i] = name[i];
    sym->link = NULL;
    return sym;
}

/* Insert a name in the symbol table at current value of blocknumber */
/* Returns pointer to the new symbol table entry, which is empty     */
/* except for the name.                                              */
SYMBOL insertsym(char name[]) {
	return insertsymat(name, blocknumber);
}
SYMBOL insertsymat(char name[], int level) {
	SYMBOL sym;
	sym = makesym(name);
	int pos = hashfun(name);
	while (symtab[level][pos] != NULL) {
		pos = (pos + 1) % HASH_SIZE;
		if (pos == hashfun(name)) {
			printf("Error: symbol table overflow.\n");
			exit(-1);
		}
	}
	symtab[level][pos] = sym;
	sym->blockLevel = level;

	// we will use the offset of var in gencode.c
	if (sym->kind == SYM_VAR) {
		sym->offset = blockoffs[level];
		blockoffs[level] += basicsizes[sym->basicType];
	} 

	if (DEBUG_SYMTAB) printf("insertsym %8s %ld at level %d, pos %d\n",
		name, (long) sym, level, pos);
	return sym;
}

int hashfun(char name[]) {
	if (name[0] == '_') return 26;
	return tolower(name[0])-'a';
}


/* Search one level of the symbol table for the given name.         */
/* Result is a pointer to the symbol table entry or NULL            */
SYMBOL searchlev(char name[], int level){
	int pos = hashfun(name);
	SYMBOL sym; 

	while ((sym = symtab[level][pos]) != NULL) {
		if (strcmp(name, sym->nameString) == 0) return sym;
		pos = (pos + 1) % HASH_SIZE;
		if (pos == hashfun(name)) return NULL;
	}
	return NULL;
}

/* Search all levels of the symbol table for the given name.        */
/* Result is a pointer to the symbol table entry or NULL            */
SYMBOL searchst(char name[]){
	SYMBOL sym = NULL;
	int level = blocknumber;
	while ( sym == NULL && level >= 0 ) {
		sym = searchlev(name, level);
		if (level > 0) level = contblock[level]; /* try containing block */
		else level = -1;                      /* until all are tried  */
	}
	if (DEBUG_SYMTAB) printf("searchst %8s %ld at level %d\n",
		name, (long) sym, level);
	return sym;
}

/* Search for symbol, insert if not there. */
SYMBOL searchins(char name[]) {
	SYMBOL res;
	res = searchst(name);
	if ( res != NULL ) return(res);
	res = insertsym(name);
	return(res);
}

/* Get the alignment boundary for a type  */
int alignsize(SYMBOL sym) {
	switch (sym->kind) {
		case SYM_BASIC:
		case SYM_SUBRANGE:
			return sym->size;
			break;
		case SYM_ARRAY:
		case SYM_RECORD:
			return 16;
			break;
		default:
			return 8;
			break;
	}
}

/* Print one symbol table entry for debugging      */
void dbprsymbol(SYMBOL sym) {
	if( sym != NULL )
		printf(" %ld  %10s knd %1d %1d  typ %ld lvl %1d  siz %5d  off %5d lnk %ld\n",
			(long)sym, sym->nameString, sym->kind, sym->basicType, (long)sym->dataType,
			sym->blockLevel, sym->size, sym->offset, (long)sym->link);
}
/* print type expression in prefix form */
void pprintsym(SYMBOL sym, int col) {
	SYMBOL opnds; int nextcol, start, done, i;
	if (sym == NULL) {
		printf ("pprintsym: called with sym = NULL\n");
		return;
	}
	if (PPSYMDEBUG != 0) {
		printf ("pprintsym: col %d\n", col);
		dbprsymbol(sym);
	}
	switch (sym->kind) {
		case SYM_BASIC:
			printf("%s", sym->nameString);
			nextcol = col + 1 + strlen(sym->nameString);
			break;
		case SYM_SUBRANGE:
			printf("%3d ..%4d", sym->lowBound, sym->highBound);
			nextcol = col + 10;
			break;

	case SYM_FUNCTION:
	case SYM_ARRAY:
	case SYM_RECORD:
		printf ("(%s", symprint[sym->kind]);
		nextcol = col + 2 + symsize[sym->kind];
		if ( sym->kind == SYM_ARRAY ) {
			printf(" %3d ..%4d", sym->lowBound, sym->highBound);
			nextcol = nextcol + 11;
		}
		opnds = sym->dataType;
		start = 0;
		done = 0;
		while ( opnds != NULL && done == 0 ) {
			if (start == 0) printf(" ");
			else {
				printf("\n");
				for (i = 0; i < nextcol; i++) printf(" ");
			}
			if ( sym->kind == SYM_RECORD ) {
				printf("(%s ", opnds->nameString);
				pprintsym(opnds, nextcol + 2 + strlen(opnds->nameString));
				printf(")");
			}
			else pprintsym(opnds, nextcol);
			start = 1;
			if ( sym->kind == SYM_ARRAY ) done = 1;
			opnds = opnds->link;
		}
		printf(")");
		break;
	default:
		if ( sym->dataType != NULL) pprintsym(sym->dataType, col);
		else printf("NULL");
		break;
	}
}
/* print a type expression in prefix form */
void ppsym(SYMBOL sym) {
	pprintsym(sym, 0);
	printf("\n");
}

/* Print one symbol table entry       */
void printsymbol(SYMBOL sym) {
	if (sym == NULL) {
		printf ("printsymbol: called with sym = NULL\n");
		return;
	}
	switch (sym->kind) {
		case SYM_FUNCTION:
		case SYM_ARRAY:
		case SYM_RECORD:
			printf(" %ld  %10s  knd %1d %1d  typ %ld  lvl %2d  siz %5d  off %5d\n",
				(long)sym, sym->nameString, sym->kind, sym->basicType, (long)sym->dataType,
				sym->blockLevel, sym->size, sym->offset);
			ppsym(sym);
			break;
		case SYM_VAR:
			if (sym->dataType->kind == SYM_BASIC)
				printf(" %ld  %10s  VAR    %1d typ %7s  lvl %2d  siz %5d  off %5d\n",
					(long)sym, sym->nameString, sym->basicType, sym->dataType->nameString,
					sym->blockLevel, sym->size, sym->offset);
			else printf(" %ld  %10s  VAR    %1d typ %ld  lvl %2d  siz %5d  off %5d\n",
					(long)sym, sym->nameString, sym->basicType, (long)sym->dataType,
 					sym->blockLevel, sym->size, sym->offset);
			if (sym->dataType->kind != SYM_BASIC ) ppsym(sym->dataType);
			break;
		case SYM_TYPE:
			printf(" %ld  %10s  TYPE   typ %ld  lvl %2d  siz %5d  off %5d\n",
				(long)sym, sym->nameString, (long)sym->dataType,
				sym->blockLevel, sym->size, sym->offset);
			if (sym->dataType->kind != SYM_BASIC ) ppsym(sym->dataType);
			break;
		case SYM_BASIC:
			printf(" %ld  %10s  BASIC  basicType %3d          siz %5d\n",
				(long)sym, sym->nameString, sym->basicType, sym->size);
			break;
		case SYM_SUBRANGE:
			printf(" %ld  %10s  SUBRA  typ %7d  val %5d .. %5d\n",
				(long)sym, sym->nameString, sym->basicType, sym->lowBound, sym->highBound);
			break;
		case SYM_CONST:
			switch (sym->basicType) {
				case TYPE_INT:
					printf(" %ld  %10s  CONST  typ INTEGER  val  %d\n",
						(long)sym, sym->nameString, sym->constval.intNum);
					break;
				case TYPE_REAL:
					printf(" %ld  %10s  CONST  typ    REAL  val  %12e\n",
						(long)sym, sym->nameString, sym->constval.realNum);
					break;
				case TYPE_STR:
					printf(" %ld  %10s  CONST  typ  STRING  val  %12s\n",
						(long)sym, sym->nameString, sym->constval.stringConst);
					break;
				case TYPE_CHAR:
					printf(" %ld  %10s  CONST  typ  STRING  val  %c\n",
						(long)sym, sym->nameString, sym->constval.charConst);
					break;
			}
		break;
	};
}

/* Print entries on one level of symbol table */
void printstlevel(int level) {
	printf("Symbol table level %d\n", level);
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		printf("%2d: ", i);
		if (symtab[level][i] != NULL) printsymbol(symtab[level][i]);
		else printf("NULL\n");
	}
}

/* Print all entries in the symbol table */
void printst() {
	int level;
	for (level = 0; level <= lastblock; level++)
		printstlevel(level);
}

/* Insert a basic type into the symbol table */
SYMBOL insertbt(char name[], int basictp, int siz) {
	SYMBOL sym = insertsym(name);
	sym->kind = SYM_BASIC;
	sym->basicType = basictp;
	sym->size = siz;
	return sym;
}

/* Insert a one-argument function in the symbol table. */
/* Linked to the function symbol are result type followed by arg types.  */
SYMBOL insertfn(char name[], SYMBOL resulttp, SYMBOL argtp) {
	SYMBOL sym, res, arg;
	sym = insertsym(name);
	sym->kind = SYM_FUNCTION;
	res = symalloc();
	res->kind = SYM_ARGM;
	res->dataType = resulttp;
	if (resulttp != NULL) res->basicType = resulttp->basicType;
	arg = symalloc();
	arg->kind = SYM_ARGM;
	arg->dataType = argtp;
	if (argtp != NULL) arg->basicType = argtp->basicType;
	arg->link = NULL;
	res->link = arg;
	sym->dataType = res;
	return sym;
}

// insert function with argument list
SYMBOL insertfnx(char name[], SYMBOL resulttp, SYMBOL arglist) {
	SYMBOL sym, res, arg;
	sym = insertsymat(name, contblock[blocknumber]);
	sym->kind = SYM_FUNCTION;
	res = symalloc();
	res->kind = SYM_ARGM;
	res->dataType = resulttp;
	if (resulttp != NULL) res->basicType = resulttp->basicType;

	res->link = arglist;
	sym->dataType = res;
	return sym;
}

/* Call this to initialize symbols provided by the compiler */
void initsyms() {
	SYMBOL sym, realsym, intsym, charsym, boolsym;
	blocknumber = 0;               /* Put compiler symbols in block 0 */
	blockoffs[1] = BASEOFFSET;     /* offset of first variable */
	realsym = insertbt("real", TYPE_REAL, 8);
	intsym  = insertbt("integer", TYPE_INT, 4);
	charsym = insertbt("char", TYPE_CHAR, 1);
	boolsym = insertbt("boolean", TYPE_BOOL, 4);

	sym = insertfn("abs", realsym, realsym);
	sym = insertfn("sqr", realsym, realsym);
	sym = insertfn("sqrt", realsym, realsym);
	sym = insertfn("ord", intsym, intsym);
	sym = insertfn("chr", charsym, intsym);
	sym = insertfn("pred", charsym, charsym);
	sym = insertfn("succ", charsym, charsym);
	sym = insertfn("odd", boolsym, intsym);

	sym = insertfn("write", NULL, charsym);
	sym = insertfn("writeln", NULL, charsym);
	sym = insertfn("read", NULL, NULL);
	sym = insertfn("readln", NULL, NULL);

	//sym = insertfn("exp", realsym, realsym);
	//sym = insertfn("trexp", realsym, realsym);
	//sym = insertfn("sin", realsym, realsym);
	//sym = insertfn("cos", realsym, realsym);
	//sym = insertfn("trsin", realsym, realsym);
	//sym = insertfn("round", realsym, realsym);
	//sym = insertfn("iround", intsym, realsym); /* C math lib defines round */
	//sym = insertfn("new", intsym, intsym);
	//sym = insertfn("trnew", intsym, intsym);
	//sym = insertfn("writef", NULL, realsym);
	//sym = insertfn("writelnf", NULL, realsym);
	//sym = insertfn("writei", NULL, intsym);
	//sym = insertfn("writelni", NULL, intsym);
	//sym = insertfn("eof", boolsym, NULL);
	blocknumber = 1;             /* Start the user program in block 1 */
	lastblock = 1;
	contblock[1] = 0;
}



int user_label_exists(TOKEN label_tok) {
	if (label_tok->intval < 0) {
		printf("Warning: searching for user label with negative value (%d)\n", label_tok->intval);
	}

	int exists = get_internal_label_num(label_tok->intval);
	if (exists == -1) {
		return 0;
	}
	return 1;
}

// do NOT return ->token for reuse, otherwise the label can only be goto'd once
int get_internal_label_num(int external_label_num) {
	if (external_label_num < 0) {
		printf("Error: cannot find negative label number %d\n", external_label_num);
		return -1;
	}
	else {
		TOKENNODE temp = user_labels;
		while (temp) {
			if (temp->token->intval == external_label_num) {
				return (temp->internal_label_num);
			}
			temp = temp->next;
		}
		return -1;
	}
}
void insert_label(int internal_label_num, TOKEN label_tok) {
	if (label_tok->intval < 0) {
		// ??? should be allowed
	}
	else if (internal_label_num < 0) {
		// ???
	}
	else if (user_label_exists(label_tok)) {
		return;
	}	// check to see if label num already exists?
	else {
//		dbugprinttok(label_tok);
		if (!user_labels) {
			user_labels = malloc(sizeof(struct toknode));
			user_labels->internal_label_num = internal_label_num;
			user_labels->token = label_tok;
			user_labels->next = NULL;
			curr_label = user_labels;
		}
		else {
			TOKENNODE curr = malloc(sizeof(struct toknode));
			curr->internal_label_num = internal_label_num;
			curr->token = label_tok;
			curr->next = NULL;
			curr_label->next = curr;
			curr_label = curr;
		}
	}
}
