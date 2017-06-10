/**********************************************************
 *	File:		symtab.h
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 6, 2017
 **********************************************************/

#ifndef SYMTAB_H
#define SYMTAB_H

#include "token.h"

/* Define kinds of symbols.  The kind field should be one of these. */
#define SYM_ARGM		0
#define SYM_BASIC		1
#define SYM_CONST		2
#define SYM_VAR			3
#define SYM_SUBRANGE	4
#define SYM_FUNCTION	5
#define SYM_ARRAY		6   
#define SYM_RECORD		7
#define SYM_TYPE		8
#define SYM_POINTER		9
#define SYM_ARGLIST		10

#define PPSYMDEBUG 0

#define MAXBLOCKS 50          /* Max number of program blocks           */

typedef struct symtbr {
	struct symtbr *link;
	char   nameString[16];
	int    kind;                /* kind of symbol -- see defines. */
	int    basicType;             /* type code for basic data types       */
	struct symtbr *dataType;    /* pointer for more complex data types  */
	int    blockLevel;
	int    size;
	int    offset;
	union  {
		char  stringConst[16];
		int   intNum;			// also store boolean value
		double realNum;
		char charConst;
	} constval;
	int    lowBound;
	int    highBound;
} SYMBOLREC, *SYMBOL;

/* symalloc makes a new symbol table record and returns a pointer to it.
   Essentially     new Symbol()   */
SYMBOL symalloc(void);

/* makesym makes a new symbol table record and puts a name into it.
   Use makesym e.g. for record field names. */
SYMBOL makesym(char name[]);

/* insertsym makes a new symbol table record, puts a name into it,
   and puts it into the symbol table.
   Use insertsym e.g. for variables and type names. */
SYMBOL insertsym(char name[]);

SYMBOL searchlev(char name[], int level);

/* search the symbol table for the given name.
   Returns a pointer to the symbol table entry if found. */
SYMBOL searchst(char name[]);

/* search the symbol table for the given name.
   Returns a pointer to the symbol table entry if found.
   If not found, a new symbol with the given name is created.
   Use this one for type names. */
SYMBOL searchins(char name[]);

/* Print one symbol table entry for debugging      */
void dbprsymbol(SYMBOL sym);

void pprintsym(SYMBOL sym, int col);

/* pretty-print a type in the symbol table */
void ppsym(SYMBOL sym);

/* Print one symbol table entry       */
void printsymbol(SYMBOL sym);

/* Print entries on one level of symbol table */
void printstlevel(int level);

/* Print all entries in the symbol table */
void printst(void);

SYMBOL insertbt(char name[], int basictp, int siz);
SYMBOL insertfn(char name[], SYMBOL resulttp, SYMBOL argtp);
void initsyms(void);    /* initializes pre-defined symbols */
SYMBOL insertfnx(char name[], SYMBOL resulttp, SYMBOL arglist);
SYMBOL insertsymat(char name[], int level);


/* alignsize returns the required alignment boundary for a type  */
int alignsize(SYMBOL sym);

#define DEBUG_SYMTAB 0
int blocknumber;           /* Number of current block being compiled */
int blockoffs[MAXBLOCKS];  /* Storage offsets for each block         */
int basicsizes[5];
int contblock[MAXBLOCKS];
int lastblock;

int user_label_exists(TOKEN label_tok);
int get_internal_label_num(int external_label_num);
void insert_label(int internal_label_num, TOKEN label_tok);

#define HASH_SIZE 26
int hashfun(char name[]);


#endif	/* SYMTAB_H */