/**********************************************************
 * File:    	parse.h
 * Project: 	SPL-compiler
 * Author:		Execution
 * Modified:	Jun 6, 2017
 **********************************************************/

#ifndef PARSE_H
#define PARSE_H

/* makeprogram makes the tree structures for the top-level program */
TOKEN makeProgram(TOKEN program_head, TOKEN routine);
/* makeop makes a new operator token with operator number opnum. */
TOKEN makeOp(int opNum);

/* makepnb is like makeprogn, except that if statements is already a progn,
   it just returns statements.  This is optional. */
TOKEN makePnb(TOKEN tok, TOKEN statements);
/* makeprogn makes a PROGN operator and links it to the list of statements.
   tok is a (now) unused token that is recycled. */
TOKEN makeProgn(TOKEN tok, TOKEN statements);

/* cons links a new item onto a list */
TOKEN cons(TOKEN list, TOKEN item);
/* instconst installs a constant in the symbol table */
void  instConst(TOKEN idtok, TOKEN consttok);
/* findtype looks up a type name in the symbol table, puts the pointer
   to its type into tok->symtype, returns tok. */
TOKEN findType(TOKEN tok);

/* binop links a binary operator op to two operands, lhs and rhs. */
TOKEN binop(TOKEN op, TOKEN lhs, TOKEN rhs);
TOKEN binop_type_coerce(TOKEN op, TOKEN lhs, TOKEN rhs);
/* makefix forces the item tok to be integer, by truncating a constant
   or by inserting a FIXOP operator */
TOKEN makefix(TOKEN tok);
/* makefloat forces the item tok to be floating, by floating a constant
   or by inserting a FLOATOP operator */
TOKEN makefloat(TOKEN tok);

/* instvars will install variables in symbol table.
   typetok is a token containing symbol table pointer for type. */
void  instVars(TOKEN idlist, TOKEN typetok);
/* wordaddress pads the offset n to be a multiple of wordsize.
   wordsize should be 4 for integer, 8 for real and pointers,
   16 for records and arrays */
int   wordaddress(int n, int wordsize);

/* findid finds an identifier in the symbol table, sets up symbol table
   pointers, changes a constant to its number equivalent */
TOKEN findId(TOKEN tok);
/* insttype will install a type name in symbol table.
   typetok is a token containing symbol table pointers. */
void  instType(TOKEN typename, TOKEN typetok);

/* instenum installs an enumerated subrange in the symbol table,
   e.g., type color = (red, white, blue)
   by calling makesubrange and returning the token it returns. */
TOKEN instEnum(TOKEN idlist);
/* makesubrange makes a SUBRANGE symbol table entry, puts the pointer to it
   into tok, and returns tok. */
TOKEN makeSubrange(TOKEN tok, int low, int high);
//TOKEN makesubrange(TOKEN tok, TOKEN low, TOKEN high);
/* makeintc makes a new token with num as its value */
TOKEN makeIntc(int num);

/* copytok makes a new token that is a copy of origtok */
TOKEN copyTok(TOKEN origtok);

/* instdotdot installs a .. subrange in the symbol table.
   dottok is a (now) unused token that is recycled. */
TOKEN instDotdot(TOKEN lowtok, TOKEN dottok, TOKEN hightok);

/* instarray installs an array declaration into the symbol table.
   bounds points to a SUBRANGE symbol table entry.
   The symbol table pointer is returned in token typetok. */
TOKEN instArray(TOKEN bounds, TOKEN typetok);

/* instrec will install a record definition.  Each token in the linked list
   argstok has a pointer its type.  rectok is just a trash token to be
   used to return the result in its symtype */
TOKEN instRec(TOKEN rectok, TOKEN argstok);
/* instfields will install type in a list idlist of field name tokens:
   re, im: real    put the pointer to REAL in the RE, IM tokens.
   typetok is a token whose symtype is a symbol table pointer.
   Note that nconc() can be used to combine these lists after instrec() */
TOKEN instFields(TOKEN idlist, TOKEN typetok);

/* dolabel is the action for a label of the form   <number>: <statement>
   tok is a (now) unused token that is recycled. */
TOKEN doLabel(TOKEN labeltok, TOKEN tok, TOKEN statement);

/* arrayref processes an array reference a[i]
   subs is a list of subscript expressions.
   tok and tokb are (now) unused tokens that are recycled. */
TOKEN arrayRef(TOKEN arr, TOKEN tok, TOKEN subs, TOKEN tokb);
/* makearef makes an array reference operation.
   off is be an integer constant token
   tok (if not NULL) is a (now) unused token that is recycled. */
TOKEN makeAref(TOKEN var, TOKEN off, TOKEN tok);
/* makeplus makes a + operator.
   tok (if not NULL) is a (now) unused token that is recycled. */
TOKEN makePlus(TOKEN lhs, TOKEN rhs, TOKEN tok);
/* addint adds integer off to expression exp, possibly using tok */
TOKEN addInt(TOKEN exp, TOKEN off, TOKEN tok);

/* makefuncall makes a FUNCALL operator and links it to the fn and args.
   tok is a (now) unused token that is recycled. */
TOKEN makeFuncall(TOKEN tok, TOKEN fn, TOKEN args);
TOKEN write_fxn_args_type_check(TOKEN fn, TOKEN args);

/* unaryop links a unary operator op to one operand, lhs */
TOKEN unaryop(TOKEN op, TOKEN lhs);

/* reducedot handles a record reference.
   dot is a (now) unused token that is recycled. */
TOKEN reduceDot(TOKEN var, TOKEN dot, TOKEN field);
TOKEN get_last_link(TOKEN tok);
TOKEN get_last_operand(TOKEN tok);
TOKEN makeRealTok(float num);

/* makeif makes an IF operator and links it to its arguments.
   tok is a (now) unused token that is recycled to become an IFOP operator */
TOKEN makeIf(TOKEN tok, TOKEN exp, TOKEN thenpart, TOKEN elsepart);

/* makerepeat makes structures for a repeat statement.
   tok and tokb are (now) unused tokens that are recycled. */
TOKEN makeRepeat(TOKEN tok, TOKEN statements, TOKEN tokb, TOKEN expr);
/* makelabel makes a new label, using labelnumber++ */
TOKEN makeLabel();
/* makegoto makes a GOTO operator to go to the specified label.
   The label number is put into a number token. */
TOKEN makeGoto(int label);

/* makewhile makes structures for a while statement.
   tok and tokb are (now) unused tokens that are recycled. */
TOKEN makeWhile(TOKEN tok, TOKEN expr, TOKEN tokb, TOKEN statement);

/* makefor makes structures for a for statement.
   sign is 1 for normal loop, -1 for downto.
   asg is an assignment statement, e.g. (:= i 1)
   endexpr is the end expression
   tok, tokb and tokc are (now) unused tokens that are recycled. */
TOKEN makeFor(TOKEN tok, TOKEN asg, TOKEN dir, TOKEN endexpr,
              TOKEN tokc, TOKEN statement);
TOKEN makeLoopIncr(TOKEN var, int incr_amt);

/* dogoto is the action for a goto statement.
   tok is a (now) unused token that is recycled. */
TOKEN doGoto(TOKEN tok, TOKEN labeltok);



/* parse.h     Gordon S. Novak Jr.    */
/* 16 Apr 04; 23 Feb 05; 17 Nov 05; 18 Apr 06; 26 Jul 12; 07 Aug 13 */

/* You may use the function headers below if you wish, or you may
   replace them if you wish.  */

/*
 * Last modified: 1730, 14/08/11
 */


/* addoffs adds offset, off, to an aref expression, exp */
TOKEN addoffs(TOKEN exp, TOKEN off);

/* appendst makes a progn containing statements followed by more */
TOKEN appendst(TOKEN statements, TOKEN more);

/* dopoint handles a ^ operator.
   tok is a (now) unused token that is recycled. */
TOKEN dopoint(TOKEN var, TOKEN tok);

/* fillintc smashes tok, making it into an INTEGER constant with value num */
TOKEN fillintc(TOKEN tok, int num);

TOKEN get_rec(TOKEN rec, TOKEN offset);

TOKEN get_rec_field(TOKEN rec, TOKEN field);


/* instlabel installs a user label into the label table */
void  instlabel (TOKEN num);

/* instpoint will install a pointer type in symbol table */
TOKEN instpoint(TOKEN tok, TOKEN typename);

//TOKEN makerealtok(double num);

TOKEN maketimes(TOKEN lhs, TOKEN rhs, TOKEN tok);


/* mulint multiplies expression exp by integer n */
TOKEN mulint(TOKEN exp, int n);

/* nconc concatenates two token lists, destructively, by making the last link
   of lista point to listb.
   (nconc '(a b) '(c d e))  =  (a b c d e)  */
/* nconc is useful for putting together two fieldlist groups to
   make them into a single list in a record declaration. */
TOKEN nconc(TOKEN lista, TOKEN listb);


/* searchins will search for symbol, inserting it if not present. */
SYMBOL searchinsst(char name[]);

TOKEN search_rec(SYMBOL recsym, TOKEN field);

/* settoktype sets up the type fields of token tok.
   typ = type pointer, ent = symbol table entry of the variable  */
void  settoktype(TOKEN tok, SYMBOL typ, SYMBOL ent);

TOKEN std_fxn_args_type_check(TOKEN fn, TOKEN args);

/* talloc allocates a new TOKEN record. */
TOKEN talloc();




typedef short boolean;
#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef true
#undef true
#endif
#define true 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef false
#undef false
#endif
#define false 0

/* Do not alter any macros below this line. */


#define NUM_COERCE_IMPLICIT		1
#define ELIM_NESTED_PROGN		1     /* disables makepnb() functionality and defaults to makeprogn() if 0 */
#define DEBUG_MASTER_SWITCH		1     /* 1 for true, 0 for false  */
#define DB_PRINT_ARGS			1     /* print function arguments */

#define DB_CONS       1             /* bit to trace cons() */
#define DB_BINOP      2             /* bit to trace binop() */
#define DB_MAKEIF     4             /* bit to trace makeif() */
#define DB_MAKEPROGN  8             /* bit to trace makeprogn() */
#define DB_PARSERES  16             /* bit to trace parseresult() */

#define DB_MAKEPROGRAM  32          /* bit to trace makeprogram() */
#define DB_MAKEFUNCALL  64          /* bit to trace makefuncall() */
#define DB_ADDINT       96          /* bit to trace addint() */
#define DB_FINDID       128            /* bit to trace findid() */
#define DB_FINDTYPE     256            /* bit to trace findtype() */
#define DB_ADDOFFS      384            /* bit to trace addoffs() */
#define DB_INSTVARS     512            /* bit to trace instvars() */
#define DB_INSTENUM     768            /* bit to trace instenum() */
#define DB_MAKEFOR      1024        /* bit to trace makefor() */
#define DB_INSTTYPE     1280        /* bit to trace insttype() */
#define DB_MULINT       2048        /* bit to trace mulint() */
#define DB_MAKEREPEAT   3072 //2560    /* bit to trace makerepeat() */
#define DB_UNARYOP      4096 //3072    /* bit to trace unaryop() */
#define DB_MAKEOP       5120 //3584    /* bit to trace makeop() */
#define DB_MAKEFLOAT    6144 //4096    /* bit to trace makefloat() */
#define DB_MAKEFIX      7168 //4608    /* bit to trace makefix() */
#define DB_MAKEGOTO     8192 //5120    /* bit to trace makegoto() */
#define DB_MAKELABEL    9216 //5632    /* bit to trace makelabel() */
#define DB_MAKEPNB      10240 //6144   /* bit to trace makepnb() */
#define DB_INSTCONST    11264 //6656   /* bit to trace instconst() */
#define DB_MAKEWHILE    12288 //7168   /* bit to trace makewhile() */
#define DB_COPYTOK      13312 //7680   /* bit to trace copytok() */
#define DB_INSTDOTDOT   14336 //8192   /* bit to trace instdotdot() */
#define DB_SEARCHINSST  15360 //8704   /* bit to trace searchinsst() */
#define DB_INSTPOINT    16384 //9216   /* bit to trace instpoint() */
#define DB_INSTREC      17408 //9728   /* bit to trace instrec() */
#define DB_INSTFIELDS   18432 //10240  /* bit to trace instfields() */
#define DB_MAKEPLUS     19456 //10752  /* bit to trace makeplus() */
#define DB_MAKEAREF     20480 //11264  /* bit to trace makearef() */
#define DB_REDUCEDOT    21504 //11776  /* bit to trace reducedot() */
#define DB_ARRAYREF     22528 //12288  /* bit to trace arrayref() */
#define DB_DOPOINT      23552 //12800  /* bit to trace dopoint() */
#define DB_INSTARRAY    24576 //13312  /* bit to trace instarray() */
#define DB_NCONC        25600 //13824  /* bit to trace nconc() */
#define DB_MAKEINTC     26624 //14336  /* bit to trace makeintc() */
#define DB_APPENDST     27648 //14848  /* bit to trace appendst() */
#define DB_DOGOTO       28672 //15360  /* bit to trace dogoto() */
#define DB_DOLABEL      29696 //15872  /* bit to trace dolabel() */
#define DB_INSTLABEL    30720 //16384  /* bit to trace instlabel() */
#define DB_SETTOKTYPE   31744 //16896  /* bit to trace settoktype() */
#define DB_MAKESUBRANGE 32768 //17408  /* bit to trace makesubrange() */

#define DEBUG (DB_MAKESUBRANGE * 2 - 1) & DEBUG_MASTER_SWITCH * (DB_MAKESUBRANGE * 2 - 1)    /* mask */


#define GEN_OUTPUT         1    /* write to output file (extension .out) */

/*  Note: you should add to the above values and insert debugging
   printouts in your routines similar to those that are shown here.     */

/* Maximum double */
#define DBL_MAX 1.7976931348623157e+308
/* Minimum normalised double */
#define DBL_MIN 2.2250738585072014e-308
/* Maximum float */
#define FLT_MAX 3.40282347e+38F
/* Minimum normalised float */
#define FLT_MIN 1.17549435e-38F

#define INT_MAX 2147483647
#define INT_MIN -2147483648


#endif   /* PARSE_H */