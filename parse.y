%{
/**********************************************************
 *	File:		parse.y
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 4, 2017
 **********************************************************/

/* Yacc reports 1 shift/reduce conflict, due to the ELSE part of
   the IF statement, but Yacc's default resolves it in the right way.*/

#include <stdio.h>
#include <ctype.h>
#include "token.h"
#include "lexan.h"
#include "symtab.h"
#include "parse.h"


/* define the type of the Yacc stack element to be TOKEN */

#define YYSTYPE TOKEN

TOKEN parseresult;
extern int lineCnt;
extern int labelnumber;

%}

/* Order of tokens corresponds to tokendefs.c; do not change */

%token ID CONST_STR CONST_INT CONST_REAL CONST_CHAR   /* token types */

%token LP RP LB RB DOTDOT COMMA COLON SEMI    /* Delimiters */

%token DOT PLUS MINUS MUL DIV_R LT LE EQ NE GT GE ASSIGN
%token AND OR NOT DIV MOD /* Operator */

%token ARRAY BEGIN_T CASE CONST DO DOWNTO ELSE END FOR FUNCTION GOTO
%token IF IN OF PACKED PROCEDURE PROGRAM READ RECORD REPEAT SET THEN
%token TO TYPE UNTIL VAR WHILE WITH
%token SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE	/* reserved */

%%

program			: program_head routine DOT			{ parseresult = makeProgram($1, $2); }
				;
program_head	: PROGRAM ID SEMI					{ $$ = $2; }
				;
routine			: routine_head routine_body			{ $$ = cons($1, $2); }
				;
sub_routine		: routine_head routine_body			{ $$ = cons($1, $2); }
				;

routine_head	: label_part const_part type_part var_part routine_part
													{ $$ = $5; }	// definitions are not in code tree
				;
label_part		: // empty
				;
// const part
const_part		: CONST const_expr_list				{ $$ = NULL; }
				| // empty
				;
const_expr_list	: const_expr_list ID EQ const_value SEMI
													{ instConst($2, $4); }
				| ID EQ const_value SEMI			{ instConst($1, $3); }
				;
const_value		: CONST_INT							{ $$ = $1; }
				| CONST_REAL						{ $$ = $1; }
				| CONST_CHAR						{ $$ = $1; }
				| CONST_STR							{ $$ = $1; }
				| SYS_CON							{ $$ = $1; }
				;
// type part
type_part		: TYPE type_decl_list				{ $$ = NULL; }		// no nodes
				| // empty
				;
type_decl_list	: type_decl_list type_definition	{ $$ = NULL; }
				| type_definition					{ $$ = NULL; }
				;
type_definition	: ID EQ type_decl SEMI				{ instType($1, $3); }
				;
type_decl		: simple_type_decl					{ $$ = $1; }
				| array_type_decl					{ $$ = $1; }
				| record_type_decl					{ $$ = $1; }
				;
simple_type_decl: SYS_TYPE							{ $$ = findType($1); }
				| ID								{ $$ = findType($1); }
				| LP id_list RP						{ $$ = instEnum($2); }
				| const_value DOTDOT const_value	{ $$ = NULL; }
				| ID DOTDOT ID						{ $$ = NULL; }
				;
array_type_decl	: ARRAY LB simple_type_decl RB OF type_decl
													{ $$ = NULL; }	// instarray
				;
record_type_decl: RECORD field_decl_list END		{ $$ = NULL; }	// intrec
				;
field_decl_list	: field_decl_list field_decl		{ $$ = cons($1, $2); }
				| field_decl						{ $$ = $1; }
				;
field_decl		: id_list COLON type_decl SEMI		{ $$ = NULL; }	// instfields
				;
id_list			: id_list COMMA ID					{ $$ = cons($1, $3); }
				| ID								{ $$ = $1; }
				;
// var part
var_part		: VAR var_decl_list					{ $$ = NULL; }
				| // empty
				;
var_decl_list	: var_decl_list var_decl			{ $$ = NULL; }
				| var_decl							{ $$ = NULL; }
				;
var_decl		: id_list COLON type_decl SEMI		{ instVars($1, $3); }	// install vars
				;
// routine part
routine_part	: routine_part function_decl		{ $$ = NULL; }
				| routine_part procedure_decl		{ $$ = NULL; }
				| function_decl						{ $$ = NULL; }
				| procedure_decl					{ $$ = NULL; }
				| // empty
				;
function_decl	: function_head SEMI sub_routine SEMI
													{ $$ = NULL; }
				;
function_head	: FUNCTION ID parameters COLON simple_type_decl
													{ $$ = NULL; }
				;
procedure_decl	: procedure_head SEMI sub_routine SEMI
													{ $$ = NULL; }
				;
procedure_head	: PROCEDURE ID parameters			{ $$ = NULL; }
				;
parameters		: LP para_decl_list RP				{ $$ = NULL; }
				| // empty
				;
para_decl_list	: para_decl_list SEMI para_type_list
													{ $$ = NULL; }
				| para_type_list					{ $$ = NULL; }
				;
para_type_list	: var_para_list COLON simple_type_decl
													{ $$ = NULL; }
				| val_para_list COLON simple_type_decl
													{ $$ = NULL; }
				;
var_para_list	: VAR id_list						{ $$ = NULL; }
				;
val_para_list	: id_list							{ $$ = NULL; }
				;

// routine body
routine_body	: compound_stmt						{ $$ = $1; }
				;
compound_stmt	: BEGIN_T stmt_list END				{ $$ = $2; }
				;
stmt_list		: stmt_list stmt SEMI				{ $$ = cons($1, $2); }
				| // empty
				;
stmt			: CONST_INT COLON non_label_stmt	{}// $$ = dolabel($1, $2, $3); }	// dolabel
				| non_label_stmt					{ $$ = $1; }
				;
non_label_stmt	: assign_stmt						{ $$ = $1; }
				| proc_stmt							{ $$ = $1; }
				| compound_stmt						{ $$ = $1; }
				| if_stmt							{ $$ = $1; }
				| repeat_stmt						{ $$ = $1; }
				| while_stmt						{ $$ = $1; }
				| for_stmt							{ $$ = $1; }
				| case_stmt							{ $$ = $1; }
				| goto_stmt							{ $$ = $1; }
				;
assign_stmt		: ID ASSIGN expression				{ $$ = binop($2, $1, $3); }
				| ID LB expression RB ASSIGN expression
													{}// $$ = binop($5, makefuncall($2, $1, $3), $6); }
				| ID DOT ID ASSIGN expression		{}// $$ = binop($4, makefuncall($2, $1, $3), $5); }
				;
proc_stmt		: ID								{ $$ = NULL; }
				| ID LP args_list RP				{ $$ = NULL; }
				| SYS_PROC							{ $$ = NULL; }
				| SYS_PROC LP expression_list RP	{ $$ = NULL; }
				| READ LP factor RP					{ $$ = NULL; }
				;
if_stmt			: IF expression THEN stmt else_clause		
													{ $$ = NULL; }
				;
else_clause		: ELSE stmt							{ $$ = NULL; }
				|									{ $$ = NULL; }
				;
repeat_stmt		: REPEAT stmt_list UNTIL expression	{ $$ = NULL; }
				;
while_stmt		: WHILE expression DO stmt			{ $$ = NULL; }
				;
for_stmt		: FOR ID ASSIGN expression direction expression DO stmt
													{ $$ = NULL; }
				;
direction		: TO								{ $$ = NULL; }
				| DOWNTO							{ $$ = NULL; }
				;
case_stmt		: CASE expression OF case_expr_list END
													{ $$ = NULL; }
				;
case_expr_list	: case_expr_list case_expr			{ $$ = NULL; }
				| case_expr							{ $$ = NULL; }
				;
case_expr		: const_value COLON stmt SEMI		{ $$ = NULL; }
				| ID COLON stmt SEMI				{ $$ = NULL; }
				;
goto_stmt		: GOTO CONST_INT					{ $$ = NULL; }
				;
expression_list	: expression_list COMMA expression	{ $$ = NULL; }
				|									{ $$ = NULL; }
				;
expression		: expression GE expr				{ $$ = NULL; }
				| expression GT expr				{ $$ = NULL; }
				| expression LE expr				{ $$ = NULL; }
				| expression LT expr				{ $$ = NULL; }
				| expression EQ expr				{ $$ = NULL; }
				| expression NE expr				{ $$ = NULL; }
				| expr								{ $$ = $1; }
				;
expr			: expr PLUS term					{ $$ = NULL; }
				| expr MINUS term					{ $$ = NULL; }
				| expr OR term						{ $$ = NULL; }
				| term								{ $$ = $1; }
				;
term			: term MUL factor					{ $$ = NULL; }
				| term DIV factor					{ $$ = NULL; }
				| term MOD factor					{ $$ = NULL; }
				| term AND factor					{ $$ = NULL; }
				| factor							{ $$ = $1; }
				;
factor			: ID								{ $$ = findId($1); }
				| ID LP args_list RP				{ $$ = NULL; }
				| SYS_FUNCT							{ $$ = NULL; }
				| SYS_FUNCT LP args_list RP			{ $$ = NULL; }
				| const_value						{ $$ = $1; }
				| LP expression RP					{ $$ = NULL; }
				| NOT factor						{ $$ = NULL; }
				| MINUS factor						{ $$ = NULL; }
				| ID LB expression RB				{ $$ = NULL; }
				| ID DOT ID							{ $$ = NULL; }
				;
args_list		: args_list COMMA expression		{ $$ = NULL; }
				| expression						{ $$ = NULL; }

%%


int yyerror(s) char *s; {
	fprintf(stderr, "Parse Error at line %d: %s\n", lineCnt, s);
	return 0;
}
