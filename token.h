/* token.h           token definitions         ; 24 Dec 12  */

/* Copyright (c) 2012 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/* Token structure and constant definitions, assuming Bison token numbers */

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

/* 09 Feb 00; 06 Jul 12; 01 Aug 12
 */
                                 /* token data structure */
typedef struct tokn {
  int    tokentype;  /* OPERATOR, DELIMITER, RESERVED, etc */
  int    datatype;   /* INTEGER, REAL, STRINGTYPE, BOOLETYPE, or POINTER */
  struct symtbr * symtype;
  struct symtbr * symentry;
  struct tokn * operands;
  struct tokn * link;
  union { char  tokenstring[16];   /* values of different types, overlapped */
          int   which;
          int   intnum;
          double realnum; } tokenval;
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
#define IDENTIFIERTOK  3
#define STRINGTOK      4
#define NUMBERTOK      5

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

#define INTEGER    0             /* number types */
#define REAL       1
#define STRINGTYPE 2
#define BOOLETYPE  3
#define POINTER    4

#define RECORDALIGN    8        /* record size must be a multiple of this */

#define IDENTIFIER 258          /* token types for use with YACC */
#define STRING 259
#define NUMBER 260

   /* subtract OPERATOR_BIAS from the following to get operator numbers */
#define PLUS 261
#define OPERATOR_BIAS  (PLUS - 1)    /* added to Operators */
#define MINUS 262
#define TIMES 263
#define DIVIDE 264
#define ASSIGN 265
#define EQ 266
#define NE 267
#define LT 268
#define LE 269
#define GE 270
#define GT 271
#define POINT 272
#define DOT 273
#define AND 274
#define OR 275
#define NOT 276
#define DIV 277
#define MOD 278
#define IN 279

   /* subtract DELIMITER_BIAS from the following to get delimiter numbers */
#define COMMA 280
#define DELIMITER_BIAS (COMMA - 1)   /* added to Delimiters */
#define SEMICOLON 281
#define COLON 282
#define LPAREN 283
#define RPAREN 284
#define LBRACKET 285
#define RBRACKET 286
#define DOTDOT 287

   /* subtract RESERVED_BIAS from the following to get reserved word numbers */
#define ARRAY 288
#define RESERVED_BIAS  (ARRAY - 1)   /* added to reserved words */
#define BEGINBEGIN 289               /* begin */
#define CASE 290
#define CONST 291
#define DO 292
#define DOWNTO 293
#define ELSE 294
#define END 295
#define FILEFILE 296                 /* file */
#define FOR 297
#define FUNCTION 298
#define GOTO 299
#define IF 300
#define LABEL 301
#define NIL 302
#define OF 303
#define PACKED 304
#define PROCEDURE 305
#define PROGRAM 306
#define RECORD 307
#define REPEAT 308
#define SET 309
#define THEN 310
#define TO 311
#define TYPE 312
#define UNTIL 313
#define VAR 314
#define WHILE 315
#define WITH 316

#define YYTOKENTYPE 0
