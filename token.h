/**********************************************************
 *	File:		token.l
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 2, 2017
 **********************************************************/

/* Copyright (c) 2012 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/* 
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.

; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License
; along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* Token structure and constant definitions, assuming Bison token numbers */

/* token data structure */
typedef struct tokn {
	int    tokentype;  /* OPERATOR, DELIMITER, RESERVED, etc */
	int    datatype;   /* INTEGER, REAL, STRINGTYPE, BOOLETYPE, or POINTER */
	struct symtbr * symtype;
	struct symtbr * symentry;
	struct tokn * operands;
	struct tokn * link;
	union {
		char  tokenstring[257];   /* values of different types, overlapped */
		int   which;
		int   intnum;
		double realnum;
	} tokenval;
} *TOKEN;

/* The following alternative kinds of values share storage in the token
   record.  Only one of the following can be present in a given token.  */
#define whichval  tokenval.which
#define intval    tokenval.intnum
#define realval   tokenval.realnum
#define stringval tokenval.tokenstring

#define OPERATOR       0         /* token types */
#define DELIMITER      1
#define RESERVED       2
#define TOKEN_ID       3
#define TOKEN_STR      4
#define TOKEN_INT      5
#define TOKEN_REAL     6

#define PLUSOP         1         /* operator numbers */
#define MINUSOP        2
#define TIMESOP        3
#define DIVIDEOP       4
#define ASSIGNOP       5
#define EQOP           6
#define NEOP           7
#define LTOP           8
#define LEOP           9
#define GEOP          10
#define GTOP          11
#define POINTEROP     12
#define DOTOP         13
#define ANDOP         14
#define OROP          15
#define NOTOP         16
#define DIVOP         17
#define MODOP         18
#define INOP          19
#define IFOP          20
#define GOTOOP        21
#define PROGNOP       22
#define LABELOP       23
#define FUNCALLOP     24
#define AREFOP        25
#define PROGRAMOP     26
#define FLOATOP       27
#define FIXOP         28

#define TYPE_INT    0             /* number types */
#define TYPE_REAL   1
#define TYPE_STR    2
#define TYPE_BOOL   3

#define ID			258          /* token types for use with YACC */
#define CONST_STR	259
#define CONST_INT	260
#define CONST_REAL	261

/* subtract DELIMITER_BIAS from the following to get reserved word numbers */
#define LP 		262
#define DELIMITER_BIAS (LP - 1)
#define RP		263
#define LB		264
#define RB		265
#define DOTDOT	266
#define COMMA	267
#define COLON	268
#define SEMI	269

/* subtract OPERATOR_BIAS from the following to get operator numbers */
#define DOT		270
#define OPERATOR_BIAS (DOT - 1)
#define PLUS	271
#define MINUS	272
#define MUL		273
#define DIV_R	274				/* division for real type */
#define LT		275
#define LE		276
#define EQ		277
#define NE		278
#define GT		279
#define GE		280
#define ASSIGN	281
#define AND		282
#define OR		283
#define NOT		284
#define DIV		285
#define MOD		286

/* subtract RESERVED_BIAS from the following to get reserved word numbers */
#define ARRAY		287
#define RESERVED_BIAS (ARRAY - 1)
#define BEGIN_T		288
#define CASE		289
#define CONST		290
#define DO			291
#define DOWNTO		292
#define ELSE		293
#define END			294
#define FOR			295
#define FUNCTION	296
#define GOTO		297
#define IF			298
#define IN			299
#define OF			300
#define PACKED		301
#define PROCEDURE	302
#define PROGRAM		303
#define RECORD		304
#define REPEAT		305
#define SET			306
#define THEN		307
#define TO			308
#define TYPE		309
#define UNTIL		310
#define VAR			311
#define WHILE		312
#define WITH		313
#define SYS_CON		314
#define SYS_FUNCT	315
#define SYS_PROC	316
#define SYS_TYPE	317


#define YYTOKENTYPE 0
