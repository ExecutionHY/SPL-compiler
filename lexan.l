/**********************************************************
 *	File:		lexan.l
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 3, 2017
 **********************************************************/

%{
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexan.h"

void read_comments();
int maketoken(int type, int which);	// make a simple new token
int install_id();	// make a token and install id
int install_real();
int install_int();
int install_str();
int yywrap();
void lexError();

/* If the lex yytext buffer turns out to be too small to hold comments,
   add the following lines:
 #undef YYLMAX
 #define YYLMAX 256
 */

TOKEN yylval;     /* This is the type of the lexical analyzer result */
int lineCnt = 1;
%}

/* LEX: DECLARATIONS */

/* regular definitions */

delim		[ \t]
ws			{delim}+
letter		[A-Za-z]
digit		[0-9]
id			{letter}(_|{letter}|{digit})*
integer		{digit}+
real		{digit}+(\.{digit}+)?(([E|e])[+\-]?{digit}+)?
comment1	("{")
comment2	(\/\/.*\n)
string		[\']
A [Aa]
B [Bb]
C [Cc]
D [Dd]
E [Ee]
F [Ff]
G [Gg]
H [Hh]
I [Ii]
J [Jj]
K [Kk]
L [Ll]
M [Mm]
N [Nn]
O [Oo]
P [Pp]
Q [Qq]
R [Rr]
S [Ss]
T [Tt]
U [Uu]
V [Vv]
W [Ww]
X [Xx]
Y [Yy]
Z [Zz]

/* LEX: RULES  */

/* States what functions to run when the following expressions are read by the scanner */

%%

{ws}		{ /* no action and no return */ }
{comment1}	{ read_comments(); }
{comment2}	{ lineCnt++; }
"\n"		{ lineCnt++; }

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
{A}{N}{D}			{ return maketoken(OPERATOR, AND); }
{O}{R}				{ return maketoken(OPERATOR, OR); }
{N}{O}{T}			{ return maketoken(OPERATOR, NOT); }
{D}{I}{V}			{ return maketoken(OPERATOR, DIV); }
{M}{O}{D}|"%"		{ return maketoken(OPERATOR, MOD); }

{A}{R}{R}{A}{Y}				{ return maketoken(RESERVED, ARRAY); }
{B}{E}{G}{I}{N}				{ return maketoken(RESERVED, BEGIN_T); }
{C}{A}{S}{E}				{ return maketoken(RESERVED, CASE); }
{C}{O}{N}{S}{T}				{ return maketoken(RESERVED, CONST); }
{D}{O}						{ return maketoken(RESERVED, DO); }
{D}{O}{W}{N}{T}{O}			{ return maketoken(RESERVED, DOWNTO); }
{E}{L}{S}{E}				{ return maketoken(RESERVED, ELSE); }
{E}{N}{D}					{ return maketoken(RESERVED, END); }
{F}{O}{R}					{ return maketoken(RESERVED, FOR); }
{F}{U}{N}{C}{T}{I}{O}{N}	{ return maketoken(RESERVED, FUNCTION); }
{G}{O}{T}{O}				{ return maketoken(RESERVED, GOTO); }
{I}{F}						{ return maketoken(RESERVED, IF); }
{I}{N}						{ return maketoken(RESERVED, IN); }
{O}{F}						{ return maketoken(RESERVED, OF); }
{P}{A}{C}{K}{E}{D}			{ return maketoken(RESERVED, PACKED); }
{P}{R}{O}{C}{E}{D}{U}{R}{E}	{ return maketoken(RESERVED, PROCEDURE); }
{P}{R}{O}{G}{R}{A}{M}		{ return maketoken(RESERVED, PROGRAM); }
{R}{E}{A}{D}				{ return maketoken(RESERVED, READ); }
{R}{E}{C}{O}{R}{D}			{ return maketoken(RESERVED, RECORD); }
{R}{E}{P}{E}{A}{T}			{ return maketoken(RESERVED, REPEAT); }
{S}{E}{T}					{ return maketoken(RESERVED, SET); }
{T}{H}{E}{N}				{ return maketoken(RESERVED, THEN); }
{T}{O}						{ return maketoken(RESERVED, TO); }
{T}{Y}{P}{E}				{ return maketoken(RESERVED, TYPE); }
{U}{N}{T}{I}{L}				{ return maketoken(RESERVED, UNTIL); }
{V}{A}{R}					{ return maketoken(RESERVED, VAR); }
{W}{H}{I}{L}{E}				{ return maketoken(RESERVED, WHILE); }
{W}{I}{T}{H}				{ return maketoken(RESERVED, WITH); }

true|false|maxint { return maketoken(RESERVED, SYS_CON); }
abs|chr|odd|ord|pred|sqr|sqrt|succ { return maketoken(RESERVED, SYS_FUNCT); }
write|writeln|read { return maketoken(RESERVED, SYS_PROC); }
integer|boolean|char|real|string { return maketoken(RESERVED, SYS_TYPE); }

{id}		{ return install_id(); }
{string}	{ return install_str(); }
{integer}	{ return install_int(); }
{real}		{ return install_real(); }

.			{ lexError(); }

%%


/* LEX: C CODE */

/* C functions used to create the tokens for the parser */

/* Note that lex always returns two values:
   1. What is it?: a small integer such as NUMBER that indicates the
      kind of item that was found; this is the return ) value.
   2. The value itself.  This is yylval, which points to a TOKEN.   */

/* Do nothing until the end comment characters, }, are read */
void read_comments() {
	char c;
	while ((c = input()) != '}') {
		if (c == '\n') lineCnt++;
	}
}

int maketoken(int type, int which) {
	yylval = (TOKEN) talloc();	/* Make new token, set yylval to point to it */
	yylval->tokenType = type;
	
	if (type == RESERVED) {
		yylval->whichval = which - RESERVED_BIAS;
		/* store those strings */
		if (which == SYS_CON || which == SYS_FUNCT || which == SYS_PROC || which == SYS_TYPE) {
			int i;
			for (i = 0; i < yyleng; i++) {
				yylval->stringval[i] = tolower(yytext[i]);
			}
			yylval->stringval[yyleng] = '\0';
		}
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
	yylval->tokenType = TOKEN_ID;
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
	/* string max length = 15 */
	yylval = (TOKEN) talloc();		/* Make new token, set yylval to point to it */
	
	int len = 0;
	char c;
	while ((c = input()) != '\'') {
		if (len < 15) yylval->stringval[len] = c;
		len++;
	}
	if (len > 15) len = 15;
	
	if (len == 1) {
		yylval->charval = yylval->stringval[0];
		yylval->tokenType = TOKEN_CHAR;
		yylval->dataType = TYPE_CHAR;
		return CONST_CHAR;
	}
	else {
		yylval->stringval[len] = '\0';
		yylval->tokenType = TOKEN_STR;
		yylval->dataType = TYPE_STR;
		return CONST_STR;
	}
	
}

int install_real() {
	yylval = (TOKEN) talloc();
	yylval->tokenType = TOKEN_NUM;
	yylval->dataType = TYPE_REAL;
	
	float num;
	sscanf(yytext, "%f", &num);
	yylval->realval = num;
	return CONST_REAL;
}

int install_int() {
	yylval = (TOKEN) talloc();
	yylval->tokenType = TOKEN_NUM;
	yylval->dataType = TYPE_INT;
	
	int num;
	sscanf(yytext, "%i", &num);
	yylval->intval = num;
	return CONST_INT;
}
void lexError() {
	printf("Lexical Analysis Error at line %d\n", lineCnt);
}

int yywrap() {  return 1;  }       /* lex may need this. */
