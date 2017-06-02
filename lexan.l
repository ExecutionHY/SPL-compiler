/**********************************************************
 *	File:		lexan.l
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 2, 2017
 **********************************************************/

%{
#include <ctype.h>
#include <stdio.h>
#include "token.h"
#include "lexan.h"

void read_comments();
int maketoken(int type, int which);	// make a simple new token
int install_id();	// make a token and install id
int install_real();
int install_int();
int install_str();
int yywrap();

/* If the lex yytext buffer turns out to be too small to hold comments,
   add the following lines:
 #undef YYLMAX
 #define YYLMAX 256
 */

TOKEN yylval;     /* This is the type of the lexical analyzer result */

%}

/* LEX: DECLARATIONS */

/* regular definitions */

delim		[ \t\n]
ws			{delim}+
letter		[A-Za-z]
digit		[0-9]
id			{letter}({letter}|{digit})*
integer		{digit}+
real		{digit}+(\.{digit}+)?(([E|e])[+\-]?{digit}+)?
comment1	("{")
comment2	(\/\/.*\n)
string		[\"]

/* LEX: RULES  */

/* States what functions to run when the following expressions are read by the scanner */

%%

{ws}		{ /* no action and no return */ }
{comment1}	{ read_comments(); }
{comment2}	{ /* no action and no return */ }

"("			{ return maketoken(DELIMITER, LP); }
")"			{ return maketoken(DELIMITER, RP); }
"["			{ return maketoken(DELIMITER, LB); }
"]"			{ return maketoken(DELIMITER, RB); }
".."		{ return maketoken(DELIMITER, DOTDOT); }
","			{ return maketoken(DELIMITER, COMMA); }
":"			{ return maketoken(DELIMITER, COLON); }
";"			{ return maketoken(DELIMITER, SEMI); }

"."			{ return maketoken(OPERATOR, DOT); }
"+"			{ return maketoken(OPERATOR, PLUS); }
"-"			{ return maketoken(OPERATOR, MINUS); }
"*"			{ return maketoken(OPERATOR, MUL); }
"/"			{ return maketoken(OPERATOR, DIV_R); }
"<"			{ return maketoken(OPERATOR, LT); }
"<="		{ return maketoken(OPERATOR, LE); }
"="			{ return maketoken(OPERATOR, EQ); }
"<>"		{ return maketoken(OPERATOR, NE); }
">"			{ return maketoken(OPERATOR, GT); }
">="		{ return maketoken(OPERATOR, GE); }
":="		{ return maketoken(OPERATOR, ASSIGN); }
"and"		{ return maketoken(OPERATOR, AND); }
"or"		{ return maketoken(OPERATOR, OR); }
"not"		{ return maketoken(OPERATOR, NOT); }
"div"		{ return maketoken(OPERATOR, DIV); }
"mod"		{ return maketoken(OPERATOR, MOD); }

"array"		{ return maketoken(RESERVED, ARRAY); }
"begin"		{ return maketoken(RESERVED, BEGIN_T); }
"case"		{ return maketoken(RESERVED, CASE); }
"const"		{ return maketoken(RESERVED, CONST); }
"do"		{ return maketoken(RESERVED, DO); }
"downto"	{ return maketoken(RESERVED, DOWNTO); }
"else"		{ return maketoken(RESERVED, ELSE); }
"end"		{ return maketoken(RESERVED, END); }
"for"		{ return maketoken(RESERVED, FOR); }
"function"	{ return maketoken(RESERVED, FUNCTION); }
"goto"		{ return maketoken(RESERVED, GOTO); }
"if"		{ return maketoken(RESERVED, IF); }
"in"		{ return maketoken(RESERVED, IN); }
"of"		{ return maketoken(RESERVED, OF); }
"packed"	{ return maketoken(RESERVED, PACKED); }
"procedure"	{ return maketoken(RESERVED, PROCEDURE); }
"program"	{ return maketoken(RESERVED, PROGRAM); }
"record"	{ return maketoken(RESERVED, RECORD); }
"repeat"	{ return maketoken(RESERVED, REPEAT); }
"set"		{ return maketoken(RESERVED, SET); }
"then"		{ return maketoken(RESERVED, THEN); }
"to"		{ return maketoken(RESERVED, TO); }
"type"		{ return maketoken(RESERVED, TYPE); }
"until"		{ return maketoken(RESERVED, UNTIL); }
"var"		{ return maketoken(RESERVED, VAR); }
"while"		{ return maketoken(RESERVED, WHILE); }
"with"		{ return maketoken(RESERVED, WITH); }

"true"|"false"|"maxint" { return maketoken(RESERVED, SYS_CON); }
"abs"|"chr"|"odd"|"ord"|"pred"|"sqr"|"sqrt"|"succ" { return maketoken(RESERVED, SYS_FUNCT); }
"write"|"writeln" { return maketoken(RESERVED, SYS_PROC); }
"integer"|"boolean"|"char"|"real"|"string" { return maketoken(RESERVED, SYS_TYPE); }

{id}		{ return install_id(); }
{string}	{ return install_str(); }
{integer}	{ return install_int(); }
{real}		{ return install_real(); }

%%


/* LEX: C CODE */

/* C functions used to create the tokens for the parser */

/* Note that lex always returns two values:
   1. "What is it?": a small integer such as NUMBER that indicates the
      kind of item that was found; this is the return ) value.
   2. The value itself.  This is yylval, which points to a TOKEN.   */

/* Do nothing until the end comment characters, "}", are read */
void read_comments() {
	while (input() != '}');
}

int maketoken(int type, int which) {
	yylval = (TOKEN) talloc();	/* Make new token, set yylval to point to it */
	yylval->tokentype = type;
	
	if (type == RESERVED) {
		yylval->whichval = which - RESERVED_BIAS;
	}
	else if (type == OPERATOR) {
		yylval->whichval = which - OPERATOR_BIAS;
	}
	else if (type == DELIMITER) {
		yylval->whichval = which - DELIMITER_BIAS;
	}
	else {
		yylval->whichval = which;
	}
	
	return which;
}

int install_id() {
	int i, c, n;
	yylval = (TOKEN) talloc();		/* Make new token, set yylval to point to it */
	yylval->tokentype = TOKEN_ID;
	n = yyleng;			/* Number of characters */
	if (n > 15) {
		n = 15;
	}
	
	for (i = 0; i < n; i++) {
		c = yytext[i];
		yylval->stringval[i] = c;
	}
	yylval->stringval[n] = '\0';
	
	return ID;
}

int install_str() {
	/* string max length = 255 */
	yylval = (TOKEN) talloc();		/* Make new token, set yylval to point to it */
	yylval->tokentype = TOKEN_STR;
	yylval->datatype = TYPE_STR;
	
	int len = 0;
	char c;
	while ((c = input()) != '\"') {
		len++;
		if (len <= 255) yylval->stringval[len] = c;
	}
	if (len > 255) len = 255;
	yylval->stringval[0] = len;
	yylval->stringval[len+1] = '\0';
	
	return CONST_STR;
}

int install_real() {
	yylval = (TOKEN) talloc();
	yylval->tokentype = TOKEN_REAL;
	yylval->datatype = TYPE_REAL;
	
	float num;
	sscanf(yytext, "%f", &num);
	yylval->realval = num;
	return CONST_REAL;
}

int install_int() {
	yylval = (TOKEN) talloc();
	yylval->tokentype = TOKEN_INT;
	yylval->datatype = TYPE_INT;
	
	int num;
	sscanf(yytext, "%i", &num);
	yylval->intval = num;
	return CONST_INT;
}


int yywrap() {  return 1;  }       /* lex may need this. */
