/**********************************************************
 *  File:		pprint.c
 *  Project:	SPL-compiler
 *  Author:		Execution
 *  Modified:	Jun 3, 2017
 **********************************************************/

#include <ctype.h>
#include <stdio.h>
#include "token.h"
#include "lexan.h"
#include "symtab.h"
#include "parse.h"
#include "pprint.h"

#define PRINTEXPRDEBUG 0     /* Set to 1 to print each node in printexpr */

char* opprint[]  = {
	" ", ".", "+", "-", "*", "/",
	"<", "<=", "=", "<>", ">", ">=", ":=",
    "and", "or", "not", "div", "mod", "^",
    "in", "if", "goto", "progn", "label", "funcall",
    "aref", "program", "float", "fix",
};

int opsize[] = {
	1, 1, 1, 1, 1, 1,
	1, 2, 1, 2, 1, 2, 2,
	3, 2, 3, 3, 3, 1,
	2, 2, 4, 5, 5, 7,
	4, 7, 5, 3,
};

/* print a token for debugging */
void debugprinttok(TOKEN tok) {
	if (tok == NULL)
		printf(" token NULL%ld\n", (long)tok);
	else printf(
			" token %ld  typ %2d  whic %3d  dty %3d  sty %ld lnk %ld  opnds %ld\n",
			(long)tok, tok->tokenType, tok->whichval, tok->dataType,
			(long)tok->symType, (long)tok->link, (long)tok->operands
		);
}
/* find length of a string */
int strlength(char str[]) {
	int i, n;
	n = 16;
	for (i = 0; i < 16; i++)
		if ( str[i] == '\0' && n == 16 ) n = i;
	return n;
}
/* print a token in abbreviated form */
void printtok(TOKEN tok) {
	switch (tok->tokenType) {
		case TOKEN_ID:
			printf ("%s", tok->stringval);
			break;
		case TOKEN_STR:
			printf ("'%s'", tok->stringval);
			break;
		case TOKEN_NUM:
			switch (tok->dataType) {
				case TYPE_INT:
					printf ("%d", tok->intval);
					break;
				case  TYPE_REAL:
					printf ("%e", tok->realval);
					break;
			}
			break;
		case DELIMITER: case RESERVED: case OPERATOR:
			break;
	}
}
/* print a token in 'nice' debugging form */
void dbugprinttok(TOKEN tok) {
	if (tok == NULL)
		printf(" token %ld  NULL\n", (long)tok);
	else switch (tok->tokenType) {
		case TOKEN_ID:
			printf(" token %ld  ID  %12s  dtype %2d  link %ld\n",
				(long)tok, tok->stringval, tok->dataType,
				(long)tok->link);
			break;
		case TOKEN_STR:
			printf(" token %ld  STR %12s  dtype %2d  link %ld\n",
				(long)tok, tok->stringval, tok->dataType, (long)tok->link);
			break;
		case TOKEN_NUM:
			switch (tok->dataType) {
				case TYPE_INT:
					printf(" token %ld  NUM %12d  dtype %2d  link %ld\n",
						(long)tok, tok->intval, tok->dataType, (long)tok->link);
					break;
				case  TYPE_REAL:
					printf(" token %ld  NUM %12e  dtype %2d  link %ld\n",
						(long)tok, tok->realval, tok->dataType, (long)tok->link);
					break;
			}
			break;
		case OPERATOR:
			printf(" token %ld  OP  %12s  dtype %2d  link %ld  operands %ld\n",
				(long)tok, opprint[tok->whichval], tok->dataType,
				(long)tok->link, (long)tok->operands);
			break;
		case DELIMITER: case RESERVED:
			debugprinttok(tok);
			break;
	}
}
/* print an expression in prefix form */
void printexpr(TOKEN tok, int col) {
	TOKEN opnds; int nextcol, start, i;
	if (PRINTEXPRDEBUG != 0) {
		printf ("printexpr: col %d\n", col);
		dbugprinttok(tok);
	}
	if (tok->tokenType == OPERATOR) {
		printf ("(%s", opprint[tok->whichval]);
		nextcol = col + 2 + opsize[tok->whichval];
		opnds = tok->operands;
		start = 0;
		while (opnds != NULL) {
			if (start == 0) printf(" ");
			else {
				printf("\n");
				for (i = 0; i < nextcol; i++) printf(" ");
			}
			printexpr(opnds, nextcol);
			if ( opnds->tokenType == TOKEN_ID && nextcol < 60 )
				nextcol = nextcol + 1 + strlength(opnds->stringval);
			else start = 1;
			opnds = opnds->link;
		}
		printf (")");
	}
	else printtok(tok);
}
/* pretty-print an expression in prefix form */
void ppexpr(TOKEN tok) {
	if ( (long) tok <= 0 ) {
		printf("ppexpr called with bad pointer %ld\n", (long)tok);
		return;
	}
	printexpr(tok, 0);
	printf("\n");
}

TOKEN debugtoken = NULL;    /* dummy op for printing a list */

/* pretty-print a list in prefix form */
void pplist(TOKEN tok) {
	if ( debugtoken == NULL ) {
		debugtoken = talloc();
		debugtoken->whichval = 0;
	}  /* will print as blank */
	debugtoken->operands = tok;
	ppexpr(debugtoken);
}
/* print a list of tokens for debugging */
void dbugplist(TOKEN tok) {
	while (tok != NULL) {
		dbugprinttok(tok);
		tok = tok->link;
	}
}
/* print rest of token for debugging */
void dbugbprinttok(TOKEN tok) {
	if (tok != NULL)
		printf("  toktyp %6d  which  %6d  symtyp %ld  syment %ld  opnds %ld\n",
			tok->tokenType, tok->whichval, (long)tok->symType,
			(long)tok->symEntry, (long)tok->operands);
}
/* print an expression in 'nice' debugging form */
void dbugprintexpr(TOKEN tok) {
	TOKEN opnds;
	dbugprinttok(tok);
	if (tok->tokenType == OPERATOR) {
		opnds = tok->operands;
		while (opnds != NULL) {
			dbugprintexpr(opnds);
			opnds = opnds->link;
		}
    }
}


// *********


void dbugprintarg(TOKEN tok) {
  printf("  ");
  if (tok) {
    dbugprinttok(tok);
  }
  else {
    printf(" NULL\n");
  }
}

void dbugprint1arg(TOKEN a) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    printf("\n");   
  }
}

void dbugprint2args(TOKEN a, TOKEN b) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    dbugprintarg(b);
    printf("\n");
  }
}

void dbugprint3args(TOKEN a, TOKEN b, TOKEN c) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    dbugprintarg(b);
    dbugprintarg(c);
    printf("\n");
  }
}

void dbugprint4args(TOKEN a, TOKEN b, TOKEN c, TOKEN d) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    dbugprintarg(b);
    dbugprintarg(c);
    dbugprintarg(d);   
    printf("\n"); 
  }
}

void dbugprint5args(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    dbugprintarg(b);
    dbugprintarg(c);
    dbugprintarg(d);
    dbugprintarg(e);
    printf("\n");
  }
}

void dbugprint6args(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e, TOKEN f) {
  if (DB_PRINT_ARGS) {
    printf(" Arguments:\n");
    dbugprintarg(a);
    dbugprintarg(b);
    dbugprintarg(c);
    dbugprintarg(d);
    dbugprintarg(e);
    dbugprintarg(f);
    printf("\n");   
  }
}

void dbugprinttok1(TOKEN tok) {
    printf("  ");
    if (tok) {
      dbugprinttok(tok);
    }
    else {
      printf(" NULL\n");
    }
}

void dbugprint1tok(TOKEN a) {
  dbugprinttok1(a);
  printf("\n");
}

void dbugprint2toks(TOKEN a, TOKEN b) {
  dbugprinttok1(a);
  dbugprinttok1(b);
  printf("\n");
}

void dbugprint3toks(TOKEN a, TOKEN b, TOKEN c) {
  dbugprinttok1(a);
  dbugprinttok1(b);
  dbugprinttok1(c);
  printf("\n");  
}

void dbugprint4toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d) {
  dbugprinttok1(a);
  dbugprinttok1(b);
  dbugprinttok1(c);
  dbugprinttok1(d);
  printf("\n");  
}

void dbugprint5toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e) {
  dbugprinttok1(a);
  dbugprinttok1(b);
  dbugprinttok1(c);
  dbugprinttok1(d);
  dbugprinttok1(e);
  printf("\n");  
}

void dbugprint6toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e, TOKEN f) {
  dbugprinttok1(a);
  dbugprinttok1(b);
  dbugprinttok1(c);
  dbugprinttok1(d);
  dbugprinttok1(e);
  dbugprinttok1(f);
  printf("\n");
}

void dbugprintsym(SYMBOL sym) {
  printf("  ");
  if (sym) {
    dbprsymbol(sym);
  }
  else {
    printf(" NULL\n");
  }
}

void dbugprint1sym(SYMBOL a) {
  dbugprintsym(a);
  printf("\n");
}

void dbugprint2syms(SYMBOL a, SYMBOL b) {
  dbugprintsym(a);
  dbugprintsym(b);
  printf("\n");
}

void dbugprint3syms(SYMBOL a, SYMBOL b, SYMBOL c) {
  dbugprintsym(a);
  dbugprintsym(b);
  dbugprintsym(c);
  printf("\n");
}

void dbugprint4syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d) {
  dbugprintsym(a);
  dbugprintsym(b);
  dbugprintsym(c);
  dbugprintsym(d);
  printf("\n");
}

void dbugprint5syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e) {
  dbugprintsym(a);
  dbugprintsym(b);
  dbugprintsym(c);
  dbugprintsym(d);
  dbugprintsym(e);
  printf("\n");
}

void dbugprint6syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e, SYMBOL f) {
  dbugprintsym(a);
  dbugprintsym(b);
  dbugprintsym(c);
  dbugprintsym(d);
  dbugprintsym(e);
  dbugprintsym(f);
  printf("\n");
}

void dbugprintlinks(TOKEN idlist) {
  TOKEN temp = idlist;
  while (temp) {
    dbugprintarg(temp);
    temp = temp->link;
  }
  printf("\n");
}

void dbugprintoperands(TOKEN idlist) {
  dbugprintlinks(idlist);
}

