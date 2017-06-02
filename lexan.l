%{   

#include <ctype.h>
#include "token.h"
#include "lexan.h"

int maketoken(int type, int which);
void read_comments();
int install_id();
int install_fnum();
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

delim    [ \t\n]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
integer  {digit}+
fnumber  {digit}+(\.{digit}+)?(([E|e])[+\-]?{digit}+)?
comment1 ("(*")
comment2 [{]+([!-|~]|{delim})*[}]
string   [']([!-&(-~]|{delim}|(['][']))*[']

/* LEX: RULES  */

/* States what functions to run when the following expressions are read by the scanner */

%%

{ws}      { /* no action and no return */ }
{comment1} {read_comments();}
{comment2} { /* no action and no return */ }
and       {return(maketoken(OPERATOR, AND));}
or        {return(maketoken(OPERATOR, OR));}
not       {return(maketoken(OPERATOR, NOT));}
div       {return(maketoken(OPERATOR, DIV));}
mod       {return(maketoken(OPERATOR, MOD));}
in        {return(maketoken(OPERATOR, IN));}
array     {return(maketoken(RESERVED, ARRAY));}
case      {return(maketoken(RESERVED, CASE));}
const     {return(maketoken(RESERVED, CONST));}
do        {return(maketoken(RESERVED, DO));}
downto    {return(maketoken(RESERVED, DOWNTO));}
file      {return(maketoken(RESERVED, FILEFILE));}
for       {return(maketoken(RESERVED, FOR));}
function  {return(maketoken(RESERVED, FUNCTION));}
goto      {return(maketoken(RESERVED, GOTO));}
label     {return(maketoken(RESERVED, LABEL));}
nil       {return(maketoken(RESERVED, NIL));}
of        {return(maketoken(RESERVED, OF));}
packed    {return(maketoken(RESERVED, PACKED));}
procedure {return(maketoken(RESERVED, PROCEDURE));}
program   {return(maketoken(RESERVED, PROGRAM));}
record    {return(maketoken(RESERVED, RECORD));}
repeat    {return(maketoken(RESERVED, REPEAT));}
set       {return(maketoken(RESERVED, SET));}
to        {return(maketoken(RESERVED, TO));}
type      {return(maketoken(RESERVED, TYPE));}
until     {return(maketoken(RESERVED, UNTIL));}
var       {return(maketoken(RESERVED, VAR));}
while     {return(maketoken(RESERVED, WHILE));}
with      {return(maketoken(RESERVED, WITH));}
if        {return(maketoken(RESERVED, IF));}
then      {return(maketoken(RESERVED, THEN));}
else      {return(maketoken(RESERVED, ELSE));}
begin     {return(maketoken(RESERVED, BEGINBEGIN));}
end       {return(maketoken(RESERVED, END));}
{id}      {return(install_id());}
{string}  {return(install_string()); }
{integer} {return(install_integer()); }
{fnumber} {return(install_fnum());}
"+"       {return(maketoken(OPERATOR, PLUS));}
"-"       {return(maketoken(OPERATOR, MINUS));}
"*"       {return(maketoken(OPERATOR, TIMES));}
"/"       {return(maketoken(OPERATOR, DIVIDE));}
"<"       {return(maketoken(OPERATOR, LT));}
"<="      {return(maketoken(OPERATOR, LE));}
"="       {return(maketoken(OPERATOR, EQ));}
"<>"      {return(maketoken(OPERATOR, NE));}
">"       {return(maketoken(OPERATOR, GT));}
">="      {return(maketoken(OPERATOR, GE));}
":="      {return(maketoken(OPERATOR, ASSIGN));}
"^"       {return(maketoken(OPERATOR, POINT));}
"."       {return(maketoken(OPERATOR, DOT));}
";"       {return(maketoken(DELIMITER, SEMICOLON));}
","       {return(maketoken(DELIMITER, COMMA));}
":"       {return(maketoken(DELIMITER, COLON));}
"("       {return(maketoken(DELIMITER, LPAREN));}
")"       {return(maketoken(DELIMITER, RPAREN));}
"["       {return(maketoken(DELIMITER, LBRACKET));}
"]"       {return(maketoken(DELIMITER, RBRACKET));}
".."      {return(maketoken(DELIMITER, DOTDOT));}

%%


/* LEX: C CODE */

/* C functions used to create the tokens for the parser */

/* Note that lex always returns two values:
   1. "What is it?": a small integer such as NUMBER that indicates the
      kind of item that was found; this is the return() value.
   2. The value itself.  This is yylval, which points to a TOKEN.   */


/* Make simple tokens. */
int maketoken(int type, int which)
  {
  yylval = (TOKEN) talloc();  /* Make new token, set yylval to point to it */
  yylval->tokentype = type;
  if (type == RESERVED)
     yylval->whichval = which - RESERVED_BIAS;
     else if (type == OPERATOR)
             yylval->whichval = which - OPERATOR_BIAS;
             else if (type == DELIMITER)
                     yylval->whichval = which - DELIMITER_BIAS;
                     else yylval->whichval = which;
  return(which);
  }

/* Do nothing until the end comment characters, "*)" or "*}", are read */
void read_comments()
 { while(1)
   {  while (input() != '*');
      switch (input())
      {  case ')': return;
         case '}': return;
         case '*': unput('*');  /* places one asterik back if we read two in a row */
      }
    }
 }

int install_id()
 { int i, c, n;
  yylval = (TOKEN) talloc();  /* Make new token, set yylval to point to it */
  yylval->tokentype = IDENTIFIERTOK;
  n = yyleng;            /* number of characters */
  if ( n > 15 ) n = 15;
  for (i = 0; i < n ; i++)
   { c = yytext[i];
          /* if (islower(c)) c = toupper(c); */ /* to raise to upper case */
     yylval->stringval[i] = c;
    };
  yylval->stringval[n] = '\0';

  return(IDENTIFIER);
 }

int install_string() {
  yylval = (TOKEN) talloc();   /* Make new token, set yylval to point to it */
  yylval->tokentype = STRINGTOK;
 
	++yytext;
	yyleng -= 2;
	int i = 0;
	int j = i;
	while(yytext[j] != '\0' && i < 15){
		if(yytext[j] == '\'')
		{
			if(yytext[j+1] == '\'')
				++j;
			else
				break;
		}
		yylval->stringval[i] = yytext[j];
		++i;
		++j;
	}
	yylval->stringval[i] = '\0';

	return(STRING);
	}

int install_integer() {  /* Note -- you may use sscanf for this assignment. */
  int num;
  yylval = (TOKEN) talloc();   /* Make new token, set yylval to point to it */
  yylval->tokentype = NUMBERTOK;
  yylval->datatype = INTEGER;

  sscanf(yytext, "%i", &num);
  yylval->intval = num;
  return(NUMBER);
 }

int install_fnum() {  /* Note -- you may use sscanf for this assignment. */
  float fnum;
  yylval = (TOKEN) talloc();   /* Make new token, set yylval to point to it */
  yylval->tokentype = NUMBERTOK;
  yylval->datatype = REAL;

  sscanf(yytext, "%f", &fnum);
  yylval->realval = fnum;
  return(NUMBER);
 }

int yywrap() {  return(1);  }       /* lex may need this. */
