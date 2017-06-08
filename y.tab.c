/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     CONST_STR = 259,
     CONST_INT = 260,
     CONST_REAL = 261,
     CONST_CHAR = 262,
     LP = 263,
     RP = 264,
     LB = 265,
     RB = 266,
     DOTDOT = 267,
     COMMA = 268,
     COLON = 269,
     SEMI = 270,
     DOT = 271,
     PLUS = 272,
     MINUS = 273,
     MUL = 274,
     DIV_R = 275,
     LT = 276,
     LE = 277,
     EQ = 278,
     NE = 279,
     GT = 280,
     GE = 281,
     ASSIGN = 282,
     AND = 283,
     OR = 284,
     NOT = 285,
     DIV = 286,
     MOD = 287,
     ARRAY = 288,
     BEGIN_T = 289,
     CASE = 290,
     CONST = 291,
     DO = 292,
     DOWNTO = 293,
     ELSE = 294,
     END = 295,
     FOR = 296,
     FUNCTION = 297,
     GOTO = 298,
     IF = 299,
     IN = 300,
     OF = 301,
     PACKED = 302,
     PROCEDURE = 303,
     PROGRAM = 304,
     READ = 305,
     RECORD = 306,
     REPEAT = 307,
     SET = 308,
     THEN = 309,
     TO = 310,
     TYPE = 311,
     UNTIL = 312,
     VAR = 313,
     WHILE = 314,
     WITH = 315,
     SYS_CON = 316,
     SYS_FUNCT = 317,
     SYS_PROC = 318,
     SYS_TYPE = 319
   };
#endif
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




/* Copy the first part of user declarations.  */
#line 1 "parse.y"

/**********************************************************
 *	File:		parse.y
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 4, 2017
 **********************************************************/

/* Yacc reports 1 shift/reduce conflict, due to the ELSE part of
   the IF statement, but Yacc's default resolves it in the right way.*/

#include <stdio.h>
#include <stdlib.h>
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

void senmaticError(char* s);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 266 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   328

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  123
/* YYNRULES -- Number of states.  */
#define YYNSTATES  255

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   319

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,    11,    14,    17,    23,    24,    27,
      28,    34,    39,    41,    43,    45,    47,    49,    52,    53,
      56,    58,    63,    65,    67,    69,    71,    73,    77,    81,
      85,    92,    96,    99,   101,   106,   110,   112,   115,   116,
     119,   121,   126,   129,   132,   134,   136,   137,   142,   148,
     153,   157,   161,   162,   166,   168,   172,   176,   179,   181,
     183,   187,   191,   192,   196,   198,   200,   202,   204,   206,
     208,   210,   212,   214,   216,   220,   227,   233,   235,   240,
     245,   250,   256,   259,   260,   265,   270,   279,   281,   283,
     289,   292,   294,   299,   304,   307,   311,   313,   317,   321,
     325,   329,   333,   337,   339,   343,   347,   351,   353,   357,
     361,   365,   369,   371,   373,   378,   383,   385,   389,   392,
     395,   400,   404,   408
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      66,     0,    -1,    67,    68,    16,    -1,    49,     3,    15,
      -1,    70,    98,    -1,    70,    98,    -1,    71,    72,    75,
      85,    88,    -1,    -1,    36,    73,    -1,    -1,    73,     3,
      23,    74,    15,    -1,     3,    23,    74,    15,    -1,     5,
      -1,     6,    -1,     7,    -1,     4,    -1,    61,    -1,    56,
      76,    -1,    -1,    76,    77,    -1,    77,    -1,     3,    23,
      78,    15,    -1,    79,    -1,    80,    -1,    81,    -1,    64,
      -1,     3,    -1,     8,    84,     9,    -1,    74,    12,    74,
      -1,     3,    12,     3,    -1,    33,    10,    79,    11,    46,
      78,    -1,    51,    82,    40,    -1,    82,    83,    -1,    83,
      -1,    84,    14,    78,    15,    -1,    84,    13,     3,    -1,
       3,    -1,    58,    86,    -1,    -1,    86,    87,    -1,    87,
      -1,    84,    14,    78,    15,    -1,    88,    89,    -1,    88,
      91,    -1,    89,    -1,    91,    -1,    -1,    90,    15,    69,
      15,    -1,    42,     3,    93,    14,    79,    -1,    92,    15,
      69,    15,    -1,    48,     3,    93,    -1,     8,    94,     9,
      -1,    -1,    94,    15,    95,    -1,    95,    -1,    96,    14,
      79,    -1,    97,    14,    79,    -1,    58,    84,    -1,    84,
      -1,    99,    -1,    34,   100,    40,    -1,   100,   101,    15,
      -1,    -1,     5,    14,   102,    -1,   102,    -1,   103,    -1,
     104,    -1,    99,    -1,   105,    -1,   107,    -1,   108,    -1,
     109,    -1,   111,    -1,   114,    -1,     3,    27,   116,    -1,
       3,    10,   116,    11,    27,   116,    -1,     3,    16,     3,
      27,   116,    -1,     3,    -1,     3,     8,   120,     9,    -1,
      63,     8,   115,     9,    -1,    50,     8,   119,     9,    -1,
      44,   116,    54,   101,   106,    -1,    39,   101,    -1,    -1,
      52,   100,    57,   116,    -1,    59,   116,    37,   101,    -1,
      41,     3,    27,   116,   110,   116,    37,   101,    -1,    55,
      -1,    38,    -1,    35,   116,    46,   112,    40,    -1,   112,
     113,    -1,   113,    -1,    74,    14,   101,    15,    -1,     3,
      14,   101,    15,    -1,    43,     5,    -1,   115,    13,   116,
      -1,   116,    -1,   116,    26,   117,    -1,   116,    25,   117,
      -1,   116,    22,   117,    -1,   116,    21,   117,    -1,   116,
      23,   117,    -1,   116,    24,   117,    -1,   117,    -1,   117,
      17,   118,    -1,   117,    18,   118,    -1,   117,    29,   118,
      -1,   118,    -1,   118,    19,   119,    -1,   118,    31,   119,
      -1,   118,    32,   119,    -1,   118,    28,   119,    -1,   119,
      -1,     3,    -1,     3,     8,   120,     9,    -1,    62,     8,
     120,     9,    -1,    74,    -1,     8,   116,     9,    -1,    30,
     119,    -1,    18,   119,    -1,     3,    10,   116,    11,    -1,
       3,    16,     3,    -1,   120,    13,   116,    -1,   116,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    49,    49,    51,    53,    55,    58,    61,    64,    65,
      67,    69,    71,    72,    73,    74,    75,    78,    79,    81,
      82,    84,    86,    87,    88,    90,    91,    92,    93,    94,
      96,    99,   101,   102,   104,   106,   107,   110,   111,   113,
     114,   116,   119,   120,   121,   122,   123,   125,   128,   131,
     134,   136,   137,   139,   141,   143,   145,   148,   150,   154,
     156,   158,   159,   161,   162,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   174,   175,   177,   179,   180,   182,
     183,   185,   188,   189,   191,   193,   195,   198,   199,   201,
     204,   205,   207,   208,   210,   212,   213,   215,   216,   217,
     218,   219,   220,   221,   223,   224,   225,   226,   228,   229,
     230,   231,   232,   234,   235,   237,   238,   239,   240,   241,
     242,   243,   245,   246
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "CONST_STR", "CONST_INT",
  "CONST_REAL", "CONST_CHAR", "LP", "RP", "LB", "RB", "DOTDOT", "COMMA",
  "COLON", "SEMI", "DOT", "PLUS", "MINUS", "MUL", "DIV_R", "LT", "LE",
  "EQ", "NE", "GT", "GE", "ASSIGN", "AND", "OR", "NOT", "DIV", "MOD",
  "ARRAY", "BEGIN_T", "CASE", "CONST", "DO", "DOWNTO", "ELSE", "END",
  "FOR", "FUNCTION", "GOTO", "IF", "IN", "OF", "PACKED", "PROCEDURE",
  "PROGRAM", "READ", "RECORD", "REPEAT", "SET", "THEN", "TO", "TYPE",
  "UNTIL", "VAR", "WHILE", "WITH", "SYS_CON", "SYS_FUNCT", "SYS_PROC",
  "SYS_TYPE", "$accept", "program", "program_head", "routine",
  "sub_routine", "routine_head", "label_part", "const_part",
  "const_expr_list", "const_value", "type_part", "type_decl_list",
  "type_definition", "type_decl", "simple_type_decl", "array_type_decl",
  "record_type_decl", "field_decl_list", "field_decl", "id_list",
  "var_part", "var_decl_list", "var_decl", "routine_part", "function_decl",
  "function_head", "procedure_decl", "procedure_head", "parameters",
  "para_decl_list", "para_type_list", "var_para_list", "val_para_list",
  "routine_body", "compound_stmt", "stmt_list", "stmt", "non_label_stmt",
  "assign_stmt", "proc_stmt", "if_stmt", "else_clause", "repeat_stmt",
  "while_stmt", "for_stmt", "direction", "case_stmt", "case_expr_list",
  "case_expr", "goto_stmt", "expression_list", "expression", "expr",
  "term", "factor", "args_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    68,    69,    70,    71,    72,    72,
      73,    73,    74,    74,    74,    74,    74,    75,    75,    76,
      76,    77,    78,    78,    78,    79,    79,    79,    79,    79,
      80,    81,    82,    82,    83,    84,    84,    85,    85,    86,
      86,    87,    88,    88,    88,    88,    88,    89,    90,    91,
      92,    93,    93,    94,    94,    95,    95,    96,    97,    98,
      99,   100,   100,   101,   101,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   103,   103,   103,   104,   104,   104,
     104,   105,   106,   106,   107,   108,   109,   110,   110,   111,
     112,   112,   113,   113,   114,   115,   115,   116,   116,   116,
     116,   116,   116,   116,   117,   117,   117,   117,   118,   118,
     118,   118,   118,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     3,     2,     2,     5,     0,     2,     0,
       5,     4,     1,     1,     1,     1,     1,     2,     0,     2,
       1,     4,     1,     1,     1,     1,     1,     3,     3,     3,
       6,     3,     2,     1,     4,     3,     1,     2,     0,     2,
       1,     4,     2,     2,     1,     1,     0,     4,     5,     4,
       3,     3,     0,     3,     1,     3,     3,     2,     1,     1,
       3,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     6,     5,     1,     4,     4,
       4,     5,     2,     0,     4,     4,     8,     1,     1,     5,
       2,     1,     4,     4,     2,     3,     1,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     3,     1,     3,     3,
       3,     3,     1,     1,     4,     4,     1,     3,     2,     2,
       4,     3,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     7,     0,     1,     0,     0,     9,     3,
       2,    62,     4,    59,     0,    18,     0,     0,     8,     0,
      38,    77,     0,     0,    60,     0,     0,     0,     0,    62,
       0,     0,    67,     0,    64,    65,    66,    68,    69,    70,
      71,    72,    73,     0,     0,     0,    17,    20,     0,    46,
       0,     0,     0,     0,     0,   113,    15,    12,    13,    14,
       0,     0,     0,    16,     0,   116,     0,   103,   107,   112,
       0,    94,     0,     0,     0,     0,     0,    61,     0,     0,
       0,    19,    36,     0,    37,    40,     0,     0,     6,    44,
       0,    45,     0,   123,     0,     0,     0,    74,    63,     0,
       0,     0,     0,   119,   118,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    11,     0,    26,
       0,     0,     0,    25,     0,     0,    22,    23,    24,     0,
       0,    39,    52,    52,    42,    43,     7,     7,    78,     0,
       0,     0,     0,     0,   121,   117,     0,   100,    99,   101,
     102,    98,    97,     0,     0,     0,    91,   104,   105,   106,
     108,   111,   109,   110,     0,    83,    80,    84,    85,    79,
       0,    10,     0,     0,     0,     0,    33,     0,     0,    21,
      35,     0,     0,     0,    50,     0,     0,     0,   122,     0,
      76,   114,   120,   115,     0,     0,    89,    90,    88,    87,
       0,     0,    81,    95,    29,    27,     0,    31,    32,     0,
      28,    41,     0,    58,     0,    54,     0,     0,     0,    47,
       5,    49,    75,     0,     0,     0,    82,     0,     0,    57,
      51,     0,     0,     0,    48,    93,    92,     0,     0,    34,
      53,    55,    56,    86,    30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     6,   195,   196,     8,    15,    18,    65,
      20,    46,    47,   135,   136,   137,   138,   185,   186,    83,
      49,    84,    85,    88,    89,    90,    91,    92,   193,   224,
     225,   226,   227,    12,    32,    16,    33,    34,    35,    36,
      37,   212,    38,    39,    40,   210,    41,   165,   166,    42,
     125,    93,    67,    68,    69,    94
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -157
static const yytype_int16 yypact[] =
{
     -45,     8,    21,  -157,    16,  -157,    17,     6,     2,  -157,
    -157,  -157,  -157,  -157,    46,    -4,   103,    38,    75,    93,
      45,    48,   110,   190,  -157,   136,   135,   190,   134,  -157,
     190,   144,  -157,   139,  -157,  -157,  -157,  -157,  -157,  -157,
    -157,  -157,  -157,   197,   137,   138,    93,  -157,   162,   -13,
     190,   190,   164,   190,     7,   102,  -157,  -157,  -157,  -157,
     190,   190,   190,  -157,   160,  -157,   218,    36,    15,  -157,
     142,  -157,   207,   190,    57,   259,   190,  -157,   156,   197,
      12,  -157,  -157,   113,   162,  -157,   171,   172,   -13,  -157,
     165,  -157,   167,   294,    72,   282,   150,   294,  -157,   190,
     190,   180,   253,  -157,  -157,   190,   190,   190,   190,   190,
     190,   190,   211,   190,   190,   190,   190,   190,   190,   190,
     190,   129,   175,   190,   129,   104,   294,  -157,   176,   174,
     162,   177,   162,  -157,   194,   192,  -157,  -157,  -157,   216,
      12,  -157,   219,   219,  -157,  -157,  -157,  -157,  -157,   190,
     208,   190,   106,   288,  -157,  -157,   112,    36,    36,    36,
      36,    36,    36,   220,   222,   206,  -157,    15,    15,    15,
    -157,  -157,  -157,  -157,   200,   198,  -157,   294,  -157,  -157,
     190,  -157,   242,   120,    19,     4,  -157,   122,   197,  -157,
    -157,   232,    10,   234,  -157,   235,     6,   238,   294,   190,
     294,  -157,  -157,  -157,   129,   129,  -157,  -157,  -157,  -157,
     190,   129,  -157,   294,  -157,  -157,   243,  -157,  -157,    12,
    -157,  -157,   162,   236,    62,  -157,   245,   246,    19,  -157,
    -157,  -157,   294,   241,   248,   265,  -157,   223,   250,   236,
    -157,    10,    19,    19,  -157,  -157,  -157,   129,    12,  -157,
    -157,  -157,  -157,  -157,  -157
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -157,  -157,  -157,  -157,   119,   254,  -157,  -157,  -157,   -43,
    -157,  -157,   224,  -137,  -156,  -157,  -157,  -157,    83,  -118,
    -157,  -157,   187,  -157,   185,  -157,   204,  -157,   151,  -157,
      54,  -157,  -157,   101,    -6,   269,  -116,   247,  -157,  -157,
    -157,  -157,  -157,  -157,  -157,  -157,  -157,  -157,   163,  -157,
    -157,   -21,   215,    43,    32,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      78,    13,    66,   191,     1,   175,    72,    82,   178,    75,
      21,     4,   183,    82,   187,   129,    56,    57,    58,    59,
     130,     5,   129,    56,    57,    58,    59,   130,   216,    86,
      95,     9,    97,    10,   116,    87,   128,   134,    14,   102,
      11,    11,    23,   117,   217,   131,   118,   119,    25,    17,
      26,    27,    19,   113,   114,   126,    50,    28,    51,    29,
      21,    43,    22,   132,    52,   115,    30,   187,   222,   164,
      31,   240,   244,    63,   223,    53,   133,   241,    44,   153,
      63,   148,   238,   133,   152,   149,   251,   252,   233,   234,
     156,    11,    23,   103,   104,   236,    45,   134,    25,   174,
      26,    27,   177,    48,   239,   122,    21,    28,    22,    29,
      99,   254,   100,   179,   123,   201,    30,   180,   101,   149,
      31,   203,   164,   223,    54,   149,   139,   140,   198,   215,
     200,   253,    21,   139,    22,   139,   219,    11,    23,    70,
      71,   134,    73,    24,    25,   220,    26,    27,   170,   171,
     172,   173,    76,    28,    77,    29,   167,   168,   169,   213,
      79,    80,    30,    11,    23,    82,    31,    96,   105,   120,
      25,   127,    26,    27,   142,   143,   134,   151,   232,    28,
     146,    29,   147,   154,   176,   134,   182,   184,    30,   235,
      13,   181,    31,    55,    56,    57,    58,    59,    60,   134,
     134,    56,    57,    58,    59,   134,   188,   189,    61,   163,
      56,    57,    58,    59,   163,    56,    57,    58,    59,   190,
      62,   106,   107,   108,   109,   110,   111,   192,   106,   107,
     108,   109,   110,   111,   204,   199,   205,   211,   208,   106,
     107,   108,   109,   110,   111,   214,   206,   221,   228,   139,
     229,    63,    64,   231,   237,   209,   245,     7,    63,   242,
     243,   121,   155,   246,   112,   249,   197,    63,   218,   248,
      81,   141,    63,   144,   106,   107,   108,   109,   110,   111,
     106,   107,   108,   109,   110,   111,   106,   107,   108,   109,
     110,   111,   145,   150,   194,   250,   124,   230,    74,   202,
       0,    98,   247,   106,   107,   108,   109,   110,   111,   106,
     107,   108,   109,   110,   111,   106,   107,   108,   109,   110,
     111,   157,   158,   159,   160,   161,   162,     0,   207
};

static const yytype_int16 yycheck[] =
{
      43,     7,    23,   140,    49,   121,    27,     3,   124,    30,
       3,     3,   130,     3,   132,     3,     4,     5,     6,     7,
       8,     0,     3,     4,     5,     6,     7,     8,   184,    42,
      51,    15,    53,    16,    19,    48,    79,    80,    36,    60,
      34,    34,    35,    28,    40,    33,    31,    32,    41,     3,
      43,    44,    56,    17,    18,    76,     8,    50,    10,    52,
       3,    23,     5,    51,    16,    29,    59,   185,    58,   112,
      63,     9,   228,    61,   192,    27,    64,    15,     3,   100,
      61,     9,   219,    64,    99,    13,   242,   243,   204,   205,
     105,    34,    35,    61,    62,   211,     3,   140,    41,   120,
      43,    44,   123,    58,   222,    73,     3,    50,     5,    52,
       8,   248,    10,     9,    57,     9,    59,    13,    16,    13,
      63,     9,   165,   241,    14,    13,    13,    14,   149,     9,
     151,   247,     3,    13,     5,    13,    14,    34,    35,     3,
       5,   184,     8,    40,    41,   188,    43,    44,   116,   117,
     118,   119,     8,    50,    15,    52,   113,   114,   115,   180,
      23,    23,    59,    34,    35,     3,    63,     3,     8,    27,
      41,    15,    43,    44,     3,     3,   219,    27,   199,    50,
      15,    52,    15,     3,     9,   228,    12,    10,    59,   210,
     196,    15,    63,     3,     4,     5,     6,     7,     8,   242,
     243,     4,     5,     6,     7,   248,    12,    15,    18,     3,
       4,     5,     6,     7,     3,     4,     5,     6,     7,     3,
      30,    21,    22,    23,    24,    25,    26,     8,    21,    22,
      23,    24,    25,    26,    14,    27,    14,    39,    38,    21,
      22,    23,    24,    25,    26,     3,    40,    15,    14,    13,
      15,    61,    62,    15,    11,    55,    15,     3,    61,    14,
      14,    54,     9,    15,    46,    15,   147,    61,   185,    46,
      46,    84,    61,    88,    21,    22,    23,    24,    25,    26,
      21,    22,    23,    24,    25,    26,    21,    22,    23,    24,
      25,    26,    88,    11,   143,   241,    37,   196,    29,    11,
      -1,    54,    37,    21,    22,    23,    24,    25,    26,    21,
      22,    23,    24,    25,    26,    21,    22,    23,    24,    25,
      26,   106,   107,   108,   109,   110,   111,    -1,   165
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    49,    66,    67,     3,     0,    68,    70,    71,    15,
      16,    34,    98,    99,    36,    72,   100,     3,    73,    56,
      75,     3,     5,    35,    40,    41,    43,    44,    50,    52,
      59,    63,    99,   101,   102,   103,   104,   105,   107,   108,
     109,   111,   114,    23,     3,     3,    76,    77,    58,    85,
       8,    10,    16,    27,    14,     3,     4,     5,     6,     7,
       8,    18,    30,    61,    62,    74,   116,   117,   118,   119,
       3,     5,   116,     8,   100,   116,     8,    15,    74,    23,
      23,    77,     3,    84,    86,    87,    42,    48,    88,    89,
      90,    91,    92,   116,   120,   116,     3,   116,   102,     8,
      10,    16,   116,   119,   119,     8,    21,    22,    23,    24,
      25,    26,    46,    17,    18,    29,    19,    28,    31,    32,
      27,    54,   119,    57,    37,   115,   116,    15,    74,     3,
       8,    33,    51,    64,    74,    78,    79,    80,    81,    13,
      14,    87,     3,     3,    89,    91,    15,    15,     9,    13,
      11,    27,   120,   116,     3,     9,   120,   117,   117,   117,
     117,   117,   117,     3,    74,   112,   113,   118,   118,   118,
     119,   119,   119,   119,   116,   101,     9,   116,   101,     9,
      13,    15,    12,    84,    10,    82,    83,    84,    12,    15,
       3,    78,     8,    93,    93,    69,    70,    69,   116,    27,
     116,     9,    11,     9,    14,    14,    40,   113,    38,    55,
     110,    39,   106,   116,     3,     9,    79,    40,    83,    14,
      74,    15,    58,    84,    94,    95,    96,    97,    14,    15,
      98,    15,   116,   101,   101,   116,   101,    11,    78,    84,
       9,    15,    14,    14,    79,    15,    15,    37,    46,    15,
      95,    79,    79,   101,    78
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 49 "parse.y"
    { parseresult = makeProgram((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); }
    break;

  case 3:
#line 51 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 4:
#line 53 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 5:
#line 55 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 6:
#line 59 "parse.y"
    { (yyval) = (yyvsp[(5) - (5)]); }
    break;

  case 8:
#line 64 "parse.y"
    { (yyval) = NULL; }
    break;

  case 10:
#line 68 "parse.y"
    { instConst((yyvsp[(2) - (5)]), (yyvsp[(4) - (5)])); }
    break;

  case 11:
#line 69 "parse.y"
    { instConst((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 12:
#line 71 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 13:
#line 72 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 14:
#line 73 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 15:
#line 74 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 16:
#line 75 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 17:
#line 78 "parse.y"
    { (yyval) = NULL; }
    break;

  case 19:
#line 81 "parse.y"
    { (yyval) = NULL; }
    break;

  case 20:
#line 82 "parse.y"
    { (yyval) = NULL; }
    break;

  case 21:
#line 84 "parse.y"
    { instType((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 22:
#line 86 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 23:
#line 87 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 24:
#line 88 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 25:
#line 90 "parse.y"
    { (yyval) = findType((yyvsp[(1) - (1)])); }
    break;

  case 26:
#line 91 "parse.y"
    { (yyval) = findType((yyvsp[(1) - (1)])); }
    break;

  case 27:
#line 92 "parse.y"
    { (yyval) = instEnum((yyvsp[(2) - (3)])); }
    break;

  case 28:
#line 93 "parse.y"
    { (yyval) = instDotdot((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 29:
#line 94 "parse.y"
    { (yyval) = instDotdot((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 30:
#line 97 "parse.y"
    { (yyval) = instArray((yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); }
    break;

  case 31:
#line 99 "parse.y"
    { (yyval) = instRec((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); }
    break;

  case 32:
#line 101 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 33:
#line 102 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 34:
#line 104 "parse.y"
    { (yyval) = instFields((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)]));; }
    break;

  case 35:
#line 106 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 36:
#line 107 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 37:
#line 110 "parse.y"
    { (yyval) = NULL; }
    break;

  case 39:
#line 113 "parse.y"
    { (yyval) = NULL; }
    break;

  case 40:
#line 114 "parse.y"
    { (yyval) = NULL; }
    break;

  case 41:
#line 116 "parse.y"
    { instVars((yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 42:
#line 119 "parse.y"
    { (yyval) = NULL; }
    break;

  case 43:
#line 120 "parse.y"
    { (yyval) = NULL; }
    break;

  case 44:
#line 121 "parse.y"
    { (yyval) = NULL; }
    break;

  case 45:
#line 122 "parse.y"
    { (yyval) = NULL; }
    break;

  case 47:
#line 126 "parse.y"
    { (yyval) = NULL; }
    break;

  case 48:
#line 129 "parse.y"
    { (yyval) = NULL; }
    break;

  case 49:
#line 132 "parse.y"
    { (yyval) = NULL; }
    break;

  case 50:
#line 134 "parse.y"
    { (yyval) = NULL; }
    break;

  case 51:
#line 136 "parse.y"
    { (yyval) = NULL; }
    break;

  case 53:
#line 140 "parse.y"
    { (yyval) = NULL; }
    break;

  case 54:
#line 141 "parse.y"
    { (yyval) = NULL; }
    break;

  case 55:
#line 144 "parse.y"
    { (yyval) = NULL; }
    break;

  case 56:
#line 146 "parse.y"
    { (yyval) = NULL; }
    break;

  case 57:
#line 148 "parse.y"
    { (yyval) = NULL; }
    break;

  case 58:
#line 150 "parse.y"
    { (yyval) = NULL; }
    break;

  case 59:
#line 154 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 60:
#line 156 "parse.y"
    { (yyval) = makePnb((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); }
    break;

  case 61:
#line 158 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); }
    break;

  case 63:
#line 161 "parse.y"
    { (yyval) = doLabel((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 64:
#line 162 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 65:
#line 164 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 66:
#line 165 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 67:
#line 166 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 68:
#line 167 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 69:
#line 168 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 70:
#line 169 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 71:
#line 170 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 72:
#line 171 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 73:
#line 172 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 74:
#line 174 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 75:
#line 176 "parse.y"
    { (yyval) = binop((yyvsp[(5) - (6)]), arrayRef((yyvsp[(1) - (6)]), NULL, (yyvsp[(3) - (6)]), NULL), (yyvsp[(6) - (6)])); }
    break;

  case 76:
#line 177 "parse.y"
    { (yyval) = binop((yyvsp[(4) - (5)]), reduceDot((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)])), (yyvsp[(5) - (5)])); }
    break;

  case 77:
#line 179 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(1) - (1)]), NULL, NULL); }
    break;

  case 78:
#line 180 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(2) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 79:
#line 182 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(2) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 80:
#line 183 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(2) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 81:
#line 186 "parse.y"
    { (yyval) = makeIf((yyvsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), (yyvsp[(5) - (5)])); }
    break;

  case 82:
#line 188 "parse.y"
    { (yyval) = (yyvsp[(2) - (2)]); }
    break;

  case 84:
#line 191 "parse.y"
    { (yyval) = makeRepeat((yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); }
    break;

  case 85:
#line 193 "parse.y"
    { (yyval) = makeWhile((yyvsp[(1) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(3) - (4)]), (yyvsp[(4) - (4)])); }
    break;

  case 86:
#line 196 "parse.y"
    { (yyval) = makeFor((yyvsp[(1) - (8)]), binop((yyvsp[(3) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)])), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(7) - (8)]), (yyvsp[(8) - (8)])); }
    break;

  case 87:
#line 198 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 88:
#line 199 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 89:
#line 202 "parse.y"
    { (yyval) = NULL; }
    break;

  case 90:
#line 204 "parse.y"
    { (yyval) = NULL; }
    break;

  case 91:
#line 205 "parse.y"
    { (yyval) = NULL; }
    break;

  case 92:
#line 207 "parse.y"
    { (yyval) = NULL; }
    break;

  case 93:
#line 208 "parse.y"
    { (yyval) = NULL; }
    break;

  case 94:
#line 210 "parse.y"
    { (yyval) = doGoto((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 95:
#line 212 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)])); }
    break;

  case 96:
#line 213 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 97:
#line 215 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 98:
#line 216 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 99:
#line 217 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 100:
#line 218 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 101:
#line 219 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 102:
#line 220 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 103:
#line 221 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 104:
#line 223 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 105:
#line 224 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 106:
#line 225 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 107:
#line 226 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 108:
#line 228 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 109:
#line 229 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 110:
#line 230 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 111:
#line 231 "parse.y"
    { (yyval) = binop((yyvsp[(2) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 112:
#line 232 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 113:
#line 234 "parse.y"
    { (yyval) = findId((yyvsp[(1) - (1)])); }
    break;

  case 114:
#line 235 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(2) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 115:
#line 237 "parse.y"
    { (yyval) = makeFuncall((yyvsp[(2) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); }
    break;

  case 116:
#line 238 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;

  case 117:
#line 239 "parse.y"
    { (yyval) = (yyvsp[(2) - (3)]); }
    break;

  case 118:
#line 240 "parse.y"
    { (yyval) = unaryop((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 119:
#line 241 "parse.y"
    { (yyval) = unaryop((yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); }
    break;

  case 120:
#line 242 "parse.y"
    { (yyval) = arrayRef((yyvsp[(1) - (4)]), NULL, (yyvsp[(3) - (4)]), NULL); }
    break;

  case 121:
#line 243 "parse.y"
    { (yyval) = reduceDot((yyvsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 122:
#line 245 "parse.y"
    { (yyval) = cons((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); }
    break;

  case 123:
#line 246 "parse.y"
    { (yyval) = (yyvsp[(1) - (1)]); }
    break;


/* Line 1267 of yacc.c.  */
#line 2283 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 248 "parse.y"



int yyerror(s) char *s; {
	fprintf(stderr, "Parser Error at line %d: %s\n", lineCnt, s);
	return 0;
}

void senmaticError(char* s) {
	fprintf(stderr, "Senmatic Error at line %d: %s\n", lineCnt, s);
	exit(-1);
}

