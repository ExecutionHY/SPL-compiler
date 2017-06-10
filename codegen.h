#ifndef CODEGEN_H
#define CODEGEN_H

/* codegen.h     Gordon S. Novak Jr.    */
/* 15 Aug 13 */

/*
 * Last modified: 0321, 14-08-15
 */

#include "parse.h"	/* Contains boolean typedef */

boolean at_least_one_float(int lhs_reg, int rhs_reg);

boolean both_float(int lhs_reg, int rhs_reg);

/* Clear register used tables to mark all registers free.  */
void clearreg();

/* find the op number that is equivalent to named function, if any */
int findfunop(char str[]);

void free_reg(int reg_num);

/* test if there is a function call within code: 1 if true, else 0 */
boolean funcallin(TOKEN code);

/* Generate code for array references and pointers */
/* In Pascal, a (^ ...) can only occur as first argument of an aref. */
/* If storereg < 0, generates a load and returns register number;
   else, generates a store from storereg. */
int genaref(TOKEN code, int storereg);

/* Generate arithmetic expression, return a register number */
int genarith(TOKEN code);

/* Generate code for a statement */
void genc(TOKEN code);

/* Top-level entry for code generator.
   pcode    = pointer to code = parseresult: (program foo (output) (progn ...))
   varsize  = size of local storage in bytes = blockoffs[blocknumber]
   maxlabel = maximum label number used so far = labelnumber    */
void gencode(TOKEN pcode, int varsize, int maxlabel);

/* Generate code for a function call */
int genfun(TOKEN code);

int genop(TOKEN code, int rhs_reg, int lhs_reg);

/* Get a register */
int getreg(int kind);

boolean is_equal(TOKEN a, TOKEN b);

boolean is_fp_reg(int reg_num);

boolean is_gen_purpose_reg(int reg_num);

/* Mark a register unused */
void mark_reg_unused(int reg);

/* Mark a register used */
void mark_reg_used(int reg);

/* Make a register non-volatile by moving it if necessary.
   Result is the (possibly new) register number.   */
int nonvolatile(int reg);

int num_funcalls_in_tree(TOKEN tok, int num);

void reset_regs(void);

/* Restore caller-saves floating point registers from stack if in use */
void restorereg();

/* Save caller-saves floating point registers on stack if in use */
void savereg();

boolean search_tree_str(TOKEN tok, char str[]);

/* find the correct MOV op depending on type of code */
int moveop(TOKEN code);


#endif