/**********************************************************
 *  File:		pprint.h
 *  Project:	SPL-compiler
 *  Author:		Execution
 *  Modified:	Jun 3, 2017
 **********************************************************/

#ifndef PPRINT_H
#define PPRINT_H

void debugprinttok(TOKEN tok);
void printtok(TOKEN tok);
void dbugprinttok(TOKEN tok);
void printexpr(TOKEN tok, int col);
void ppexpr(TOKEN tok);
void pplist(TOKEN tok);
void dbugplist(TOKEN tok);
void dbugbprinttok(TOKEN tok);
void dbugprintexpr(TOKEN tok);
void dbugprintarg(TOKEN tok);
void dbugprint1arg(TOKEN a);
void dbugprint2args(TOKEN a, TOKEN b);
void dbugprint3args(TOKEN a, TOKEN b, TOKEN c);
void dbugprint4args(TOKEN a, TOKEN b, TOKEN c, TOKEN d);
void dbugprint5args(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e);
void dbugprint6args(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e, TOKEN f);
void dbugprinttok1(TOKEN tok);
void dbugprint1tok(TOKEN a);
void dbugprint2toks(TOKEN a, TOKEN b);
void dbugprint3toks(TOKEN a, TOKEN b, TOKEN c);
void dbugprint4toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d);
void dbugprint5toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e);
void dbugprint6toks(TOKEN a, TOKEN b, TOKEN c, TOKEN d, TOKEN e, TOKEN f);
void dbugprintsym(SYMBOL sym);
void dbugprint1sym(SYMBOL a);
void dbugprint2syms(SYMBOL a, SYMBOL b);
void dbugprint3syms(SYMBOL a, SYMBOL b, SYMBOL c);
void dbugprint4syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d);
void dbugprint5syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e);
void dbugprint6syms(SYMBOL a, SYMBOL b, SYMBOL c, SYMBOL d, SYMBOL e, SYMBOL f);
void dbugprintlinks(TOKEN idlist);
void dbugprintoperands(TOKEN idlist);

#endif