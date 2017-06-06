/**********************************************************
 *	File:		token.h
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 3, 2017
 **********************************************************/

#ifndef TOKEN_H
#define TOKEN_H

/* token data structure */
typedef struct tokn {
	int		tokenType;  /* OPERATOR, DELIMITER, RESERVED, etc */
	int		which;		/* which token */
	int		dataType;   /* TYPE, REAL, STRINGTYPE, BOOLETYPE */
	struct	symtbr * symType;
	struct	symtbr * symEntry;
	struct	tokn * operands;
	struct	tokn * link;
	union {
		char tokenString[16];   /* values of different types, overlapped */
		int intNum;
		char ch;
		double realNum;
	} tokenval;
} *TOKEN;

/* The following alternative kinds of values share storage in the token
   record.  Only one of the following can be present in a given token.  */
#define whichval  which
#define intval    tokenval.intNum
#define realval   tokenval.realNum
#define stringval tokenval.tokenString
#define charval tokenval.ch

#define OPERATOR       0         /* token types */
#define DELIMITER      1
#define RESERVED       2
#define TOKEN_ID       3
#define TOKEN_STR      4
#define TOKEN_NUM      5
#define TOKEN_CHAR     6

/* basic operators */
/* these 17 definitions will not directly used,
	but will reflect on pprint.c[opprint] */
#define OP_DOT 		1
#define OP_PLUS 	2
#define OP_MINUS 	3
#define OP_MUL 		4
#define OP_DIV_R 	5
#define OP_LT 		6
#define OP_LE 		7
#define OP_EQ 		8
#define OP_NE 		9
#define OP_GT 		10
#define OP_GE 		11
#define OP_ASSIGN 	12
#define OP_AND 		13
#define OP_OR 		14
#define OP_NOT 		15
#define OP_DIV 		16
#define OP_MOD 		17

/* other operators - from reserved words */
#define OP_POINTER		18
#define OP_IN			19
#define OP_IF			20
#define OP_GOTO			21
#define OP_PROGN		22
#define OP_LABEL		23
#define OP_FUNCALL		24
#define OP_AREF			25
#define OP_PROGRAM		26
#define OP_FLOAT		27
#define OP_FIX			28

#define TYPE_INT    0             /* number types */
#define TYPE_REAL   1
#define TYPE_CHAR    2
#define TYPE_BOOL   3
#define TYPE_STR	4

/* Tokens.  */
#define ID 258
#define CONST_STR 259
#define CONST_INT 260
#define CONST_REAL 261
#define CONST_CHAR 262
#define LP 263
#define RP 264
#define LB 265
#define RB 266
#define DOTDOT 267
#define COMMA 268
#define COLON 269
#define SEMI 270

#define DOT 271
#define PLUS 272
#define MINUS 273
#define MUL 274
#define DIV_R 275
#define LT 276
#define LE 277
#define EQ 278
#define NE 279
#define GT 280
#define GE 281
#define ASSIGN 282
#define AND 283
#define OR 284
#define NOT 285
#define DIV 286
#define MOD 287

#define ARRAY 288
#define BEGIN_T 289
#define CASE 290
#define CONST 291
#define DO 292
#define DOWNTO 293
#define ELSE 294
#define END 295
#define FOR 296
#define FUNCTION 297
#define GOTO 298
#define IF 299
#define IN 300
#define OF 301
#define PACKED 302
#define PROCEDURE 303
#define PROGRAM 304
#define READ 305
#define RECORD 306
#define REPEAT 307
#define SET 308
#define THEN 309
#define TO 310
#define TYPE 311
#define UNTIL 312
#define VAR 313
#define WHILE 314
#define WITH 315
#define SYS_CON 316
#define SYS_FUNCT 317
#define SYS_PROC 318
#define SYS_TYPE 319

#define DELIMITER_BIAS (LP - 1)
#define OPERATOR_BIAS (DOT - 1)
#define RESERVED_BIAS (ARRAY - 1)

#ifndef YYTOKENTYPE
#define YYTOKENTYPE 0
#endif

/* A struct to be used as a linked list for tokens.
   Used in this program to keep track of all tokens
   ever created in order to free them as well as for
   user-created labels (see symtab.c). */
typedef struct toknode {
	struct tokn * token;
	int internal_label_num;
	struct toknode *next;
} *TOKENNODE;

#endif	/* TOKEN_H */

