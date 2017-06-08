#ifndef CODEGEN_H
#define CODEGEN_H

/* codegen.h     Gordon S. Novak Jr.    */
/* 15 Aug 13 */

/* Top-level entry for code generator.
   pcode    = pointer to code = parseresult: (program foo (output) (progn ...))
   varsize  = size of local storage in bytes = blockoffs[blocknumber]
   maxlabel = maximum label number used so far = labelnumber    */
void gencode(TOKEN pcode, int varsize, int maxlabel);

/* Generate arithmetic expression, return a register number */
int genarith(TOKEN code);

/* Generate code for a statement */
void genc(TOKEN code);




/* You may use the function headers below if you wish,
   or you may replace them if you wish.  */

/* Clear register used tables to mark all registers free.  */
void clearreg();

/* Mark a register unused */
void unused(int reg);

/* Mark a register used */
void used(int reg);

/* Get a register */
int getreg(int kind);

/* Make a register non-volatile by moving it if necessary.
   Result is the (possibly new) register number.   */
int nonvolatile(int reg);

/* Save caller-saves floating point registers on stack if in use */
void savereg();

/* Restore caller-saves floating point registers from stack if in use */
void restorereg();

/* test if there is a function call within code: 1 if true, else 0 */
int funcallin(TOKEN code);

/* find the op number that is equivalent to named function, if any */
int findfunop(char str[]);

/* Generate code for a function call */
int genfun(TOKEN code);

/* find the correct MOV op depending on type of code */
int moveop(TOKEN code);

/* Generate code for array references and pointers */
/* In Pascal, a (^ ...) can only occur as first argument of an aref. */
/* If storereg < 0, generates a load and returns register number;
   else, generates a store from storereg. */
int genaref(TOKEN code, int storereg);

#endif