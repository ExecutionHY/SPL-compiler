/* genasm.c       Generate Assembly Code for X86    ; 03 May 17   */

/* Copyright (c) 2017 Gordon S. Novak Jr. and The University of Texas at Austin
    */

/* Routines for use with CS 375 Code Generation assignment for X86. */

/* We assume Linux assembly language conventions.   */

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

/* Written by Gordon S. Novak Jr. */

/* 22 Jan 00; 24 Jan 00; 26 Jan 00; 28 Jan 00; 02 Feb 00; 04 Feb 00;
   09 Feb 00; 17 Apr 00; 26 Apr 00; 13 Apr 01; 15 Aug 03; 05 Jul 12;
   06 Jul 12; 10 Jul 12; 16 Jul 12; 18 Jul 12; 23 Jul 12; 24 Jul 12;
   26 Jul 12; 31 Jul 12; 01 Aug 12; 03 Aug 12; 06 Aug 12; 07 Aug 12;
   08 Aug 12; 10 Aug 12; 13 Aug 12; 14 May 13; 09 Jul 13; 25 Apr 14
   02 May 14; 15 May 14; 17 Apr 15; 05 May 15; 07 May 15; 05 Feb 16
   10 Oct 16
  */

#include <stdio.h>
#include <ctype.h>
#include "token.h"
#include "symtab.h"
#include "genasm.h"

/* Print strings for registers */
/* EBX is put in position 3 since it is callee-save */
char* regpr[] = {"%eax", "%ecx", "%edx", "%ebx",
                 "%esi", "%edi", "%esp", "%ebp",
                 "%r8d", "%r9d", "%r10d", "%r11d",
                 "%r12d", "%r13d", "%r14d", "%r15d",
                 "%xmm0",  "%xmm1",   "%xmm2",  "%xmm3",
                 "%xmm4",  "%xmm5",  "%xmm6",  "%xmm7",
                 "%xmm8",  "%xmm9",  "%xmm10", "%xmm11",
                 "%xmm12", "%xmm13", "%xmm14", "%xmm15" };

/* Print strings for 64-bit versions of registers */
char* dregpr[] = {"%rax", "%rcx", "%rdx", "%rbx",
                  "%rsi", "%rdi", "%rsp", "%rbp",
                  "%r8", "%r9", "%r10", "%r11",
                  "%r12", "%r13", "%r14", "%r15"  };

/* Define jump op codes */

char* jumppr[]  = {"jmp", "jne", "je", "jge", "jl", "jg", "jle" };

/* Define jump comments */

char* jumpcompr[]  = {"", "if     !=", "if     ==", "if     >=", "if     <",
                      "if     >", "if     <=" };

/* Define op codes for other instructions */

 char* instpr[] =
/*   0        1       2       3      4       5      6        7  */
 {"movl", "movsd", "movq", "cltq", "addl", "subl", "imull", "divl",
/*   8     9     10       11     12     13       14      15  */
  "andl", "negl", "orl", "notl", "cmpl", "addsd", "subsd", "mulsd",
/*   16       17      18      19      */
  "divsd", "negsd", "cmpq", "cmpsd",
/*   20      21       22      23      24      25     26     */
   "addq", "subq", "imulq", "andq", "negq", "orq", "notq"
        };

/*                    0     1      2       3        4    5    6    7  */
char* instcompr[] = {"->", "->",  "->", "sign-extend", "+", "-", "*", "/",
/*                     8     9     10     11     12        13   14   15  */
                     " and", "-", "or", "notl", "compare", "+", "-", "*",
/*                     16    17      18      19      */
                     "/", "-", "compare", "compare",
/*   20      21       22      23      24      25     26     */
    "+",     "-",     "*",    "&",   "negq", "orq", "notq" };
char* topcode[] = {
  "# ---------------- Beginning of Generated Code --------------------",
  ""};

char* topcodeb[] = {
  ".LFB0:",
  "	.cfi_startproc",
  "	pushq	%rbp              # save base pointer on stack",
  "	.cfi_def_cfa_offset 16",
  "	movq	%rsp, %rbp        # move stack pointer to base pointer",
  "	.cfi_offset 6, -16",
  "	.cfi_def_cfa_register 6",
  ""};

char* topcodec[] = {
  "	movq	%rbx, %r9        # save %rbx (callee-saved) in %r9",
  "# ------------------------- begin Your code -----------------------------",
  ""};

char* bottomcode[] = { 
  "# ----------------------- begin Epilogue code ---------------------------",
  "	movq	%r9, %rbx        # restore %rbx (callee-saved) from %r9",
  "        leave",
  "        ret",
  "        .cfi_endproc",
  ".LFE0:",
  ""};

char* bottomcodeb[] = { 
  "# ----------------- end Epilogue; Literal data follows ------------------",
  "        .section        .rodata",
  ""};

char* bottomcodec[] = { 
  "        .ident  \"CS 375 Compiler - Spring 2017\"",
  /* "        .section     .note.GNU-stack,\"\",@progbits", /* need this? */
  ""};

/* constant needed for floating negation */
char* fnegconst[] = {
  "	.align 16",
  ".LC666:                    # constant for floating negation",
  "	.long	0",
  "	.long	-2147483648",
  "	.long	0",
  "	.long	0",
  ""};

/* Tables of literal constants */

static int   iliterals[100];  /* integer literals */
static int   ilabels[100];
static float fliterals[100];  /* floating literals */
static int   flabels[100];
static char  bliterals[1000]; /* byte literals */
static int   blindex[100];    /* index to byte literals */
static int   blabels[100];
static int   nilit = 0;
static int   nflit = 0;
static int   nblit = 0;
static int   floatconst = 0;  /* literal constant needed for floating */

static int   fnegused = 0;    /* constant for floating negation is used */

static int   stackframesize;  /* size of stack frame variables */


/* Print a section of canned code.  Quits on an empty string.  */
void cannedcode(char* lines[])
  { int i;
    i = 0;
    while ( lines[i][0] != 0 )
      { printf("%s\n", lines[i]); i++; }
  }

/* Round up n to be a multiple of m */
int roundup(int n, int m)
  { return ((n + m - 1) / m) * m; }

/* Make entry code for a program. */
/* name is entry name, size is size of local storage on stack frame. */
/* returns actual stack frame size */
int asmentry(char name[], int size)
  {  
/*     GCC requires stack aligned to 16-byte boundary */
/*     Add to make space for a floating temporary just below rbp */
     stackframesize = roundup(size + 16, 16);
     cannedcode(topcode);          /* canned stuff at top */
     printf("        .file   \"%s\"\n", "foo");
     printf("        .text\n");
     printf(".globl %s\n", name);
     printf("        .type   %s, @function\n", name);
     printf("%s:\n", name);     
     cannedcode(topcodeb);
     printf("        subq\t$%d, %%rsp \t  # make space for this stack frame\n",
            stackframesize );
     cannedcode(topcodec);
     return stackframesize;
   }

/* Make exit code for a program. */
/* This is just canned code following calling conventions for target machine */
void asmexit(char name[])
  {  cannedcode(bottomcode);
     printf("        .size   %s, .-%s\n", name, name);
     outlits();                         /* Output literals */
   }

/* Make a label */
void asmlabel(int labeln)
  {  printf(".L%d:\n", labeln);
   }

/* Subroutine call */
/* Example:  asmcall(code->stringval);    Call function */
void asmcall(char name[])
  {  printf("\tcall\t%s              \t#  %s()\n", name, name);
  }

/* Generate a jump instruction.  Example:  asmjump(JG, 17);   */
void asmjump(int code, int labeln)
{  printf("\t%s\t.L%d \t\t\t#  jump %s\n",
          jumppr[code], labeln, jumpcompr[code]);
   }

/* Get the right register name depending on instruction */
char* regnm(int reg, int instr)
{  return ( instr == MOVQ || instr == CMPQ ) ? dregpr[reg] : regpr[reg]; }

/* Generate an immediate instruction: move an int to a register   */
/* Example:  asmimmed(MOVL, 3, EAX);   Moves 3 to EAX  */
/* Example:  asmimmed(ADDL, 1, EAX);   Adds 1 to EAX  */
void asmimmed(int inst, int ival, int reg)
{   printf("\t%s\t$%d,%s", instpr[inst], ival, regnm(reg, inst));
    if ( inst == MOVL || inst == MOVSD  || inst == MOVQ)
      printf("         \t#  %d -> %s\n", ival, regnm(reg, inst));
    else printf("         \t#  %s %s %d -> %s\n",
                regnm(reg, inst), instcompr[inst], ival, regnm(reg, inst));
  }

/* Generate an instruction with just the op. */
/* Example:  asmop(CLTQ) */
void asmop(int inst)
{ printf("\t%s\t                  \t#  %s\n", instpr[inst], instcompr[inst]);
}

/* Generate a register to register instruction. */
/* op rs,rd     ADDL  */
/* Example:  asmrr(ADDL, ECX, EAX);  EAX + ECX -> EAX */
void asmrr(int inst, int srcreg, int dstreg)
  { printf("\t%s\t%s,%s", instpr[inst], regnm(srcreg, inst),
           regnm(dstreg, inst));
    if ( inst == CMPL || inst == CMPQ || inst == CMPSD )
      printf("           \t#  compare %s - %s\n", regnm(dstreg, inst),
             regnm(srcreg, inst));
    else if ( inst == MOVL || inst == MOVQ || inst == MOVSD )
      printf("         \t#  %s -> %s\n", regpr[srcreg], regpr[dstreg]);
    else printf("         \t#  %s %s %s -> %s\n",
                regpr[dstreg], instcompr[inst], regpr[srcreg], regpr[dstreg]);
  }

/* Generate a load instruction relative to RBP: */
/* Example:  if code points to an integer variable,
      asmld(MOVL, -code->symentry->offset, 0, code->stringval);   */
void asmld(int inst, int off, int reg, char str[])
{  printf("\t%s\t%d(%%rbp),%s", instpr[inst], off, regnm(reg, inst));
     printf("     \t#  %s -> %s\n", str, regnm(reg, inst));
  }

/* Generate a store instruction relative to RBP: */
/* Example:  asmst(MOVL, EAX, -code->symentry->offset, code->stringval);  */
void asmst(int inst, int reg, int off, char str[])
{  printf("\t%s\t%s,%d(%%rbp)", instpr[inst], regnm(reg, inst), off);
     printf("     \t#  %s -> %s\n", regnm(reg, inst), str);
  }

/* Generate a floating store into a temporary on stack */
/* Example:  asmst(MOVL, EAX, -code->symentry->offset, code->stringval);  */
void asmsttemp( int reg )
{  asmst( MOVSD, reg, -8, "temp");
  }

/* Generate a floating from a temporary on stack */
/* Example:  asmst(MOVL, EAX, -code->symentry->offset, code->stringval);  */
void asmldtemp( int reg )
{  asmld( MOVSD, -8, reg, "temp");
  }

/* Generate a load instruction using offset and a register: */
/* Example:  asmldr(MOVL, 4, RAX, ECX, code->stringval);  4(%rax) --> %ecx */
void asmldr(int inst, int offset, int reg, int dstreg, char str[])
{  printf("\t%s\t%d(%s),%s", instpr[inst], offset, dregpr[reg],
          regnm(dstreg, inst));
  printf("         \t#  %s[%d+%s] -> %s\n", str, offset, dregpr[reg],
         regnm(dstreg, inst));
  }

/* Generate a load instruction using offset, RBP and another register: */
/* Example:
   asmldrr(MOVL, -8, RAX, ECX, code->stringval);  -8(%rbp,%rax) --> %ecx */
void asmldrr(int inst, int offset, int reg, int dstreg, char str[])
  {  printf("\t%s\t%d(%%rbp,%s),%s", instpr[inst], offset, dregpr[reg],
            regnm(dstreg, inst));
    printf("         \t#  %s[%d] -> %s\n", str, offset, regnm(dstreg, inst));
  }

/* Generate a load instruction using offset, RBP and
   another register with multiplier (2, 4 or 8): */
/* Example:
   asmldrrm(MOVL, -8, RAX, 4, ECX, code->stringval); -8(%rbp,%rax,4) --> %ecx */
void asmldrrm(int inst, int offset, int reg, int mult, int dstreg, char str[])
{  printf("\t%s\t%d(%%rbp,%s,%d),%s", instpr[inst], offset, dregpr[reg], mult,
          regnm(dstreg, inst));
  printf("    \t#  %s[%d+%%rbp+%s*%d] -> %s\n", str, offset, dregpr[reg], mult,
          regnm(dstreg, inst));
  }

/* Generate a store instruction relative to a register: */
/* Example:  asmstr(MOVL, ECX, 4, RAX, code->stringval);  %ecx --> 4(%rax) */
void asmstr(int inst, int srcreg, int offset, int reg, char str[])
{  printf("\t%s\t%s,%d(%s)", instpr[inst], regnm(srcreg, inst), offset,
          dregpr[reg]);
  printf("         \t#  %s -> %s[%d+%s]\n", regnm(srcreg, inst), str, offset,
         dregpr[reg]);
  }

/* Generate a store instruction using offset, RBP and another register: */
/* Example:
   asmstrr(MOVL, ECX, -8, RAX, code->stringval);  %ecx --> -8(%rbp,%rax) */
void asmstrr(int inst, int srcreg, int offset, int reg, char str[])
  {  printf("\t%s\t%s,%d(%%rbp,%s)", instpr[inst], regnm(srcreg, inst), offset,
            dregpr[reg] );
     printf("\t#  %s -> %s[%s]\n", regnm(srcreg, inst), str,
            dregpr[reg]);
  }

/* Generate a store instruction using offset, RBP and
   another register with multiplier (2, 4 or 8): */
/* Example:
   asmstrrm(MOVL, ECX, -8, RAX, 4, code->stringval); %ecx --> -8(%rbp,%rax,4) */
void asmstrrm(int inst, int srcreg, int offset, int reg, int mult, char str[])
{  printf("\t%s\t%s,%d(%%rbp,%s,%d)", instpr[inst], regnm(srcreg, inst), offset,
          dregpr[reg], mult);
     printf("   \t#  %s -> %s[%d+%%rbp+%s*%d]\n", regnm(srcreg, inst), str,
            offset, dregpr[reg], mult);
  }

/* Load float literal into specified register */
/* Example:  asmldflit(MOVSD, 7, XMM0);  literal with label .LC7 --> XMM0 */
void asmldflit(int inst, int label, int dstreg)
{  int i;
     double d = 0.0;
     for (i=0; i<nflit; i++)
       if ( label == flabels[i] )  d = fliterals[i];
     printf("\t%s\t.LC%d(%%rip),%s   \t#  %f -> %s\n", instpr[inst],
               label, regpr[dstreg], d, regpr[dstreg]);
  }

/* Set up a literal address argument for subroutine call */
/* Example:  asmlitarg(8, EDI);   addr of literal 8 --> %edi */
void asmlitarg(int labeln, int dstreg)
  {  printf("\tmovl\t$.LC%d,%s       \t#  addr of literal .LC%d\n",
             labeln, regpr[dstreg], labeln);
  }

/* Generate instructions to float data from an integer register to F reg. */
/* reg is integer source, freg is double float destination register. */
void asmfloat(int reg, int freg)
  { printf("\tcvtsi2sd\t%s,%s    \t#  float %s -> %s\n", regpr[reg],
              regpr[freg], regpr[reg], regpr[freg]);
  }

/* Generate instruction to fix data from float register freg to int reg. */
/* freg is double float source, reg is integer destination register. */
void asmfix(int freg, int reg)
  { printf("\tcvttsd2si\t%s,%s    \t#  fix %s -> %s\n", regpr[freg],
              regpr[reg], regpr[freg], regpr[reg]);
  }

/* Generate instructions to negate a float reg. */
/* reg is value to be negated, extrareg is another float register. */
void asmfneg(int reg, int extrareg)
  { fnegused = 1;
    asmldflit(MOVSD, 666, extrareg);
    printf("\txorpd\t%s,%s           \t#  negate %s\n",
              regpr[extrareg], regpr[reg], regpr[reg]);
  }

/* Make a literal for integer n with label number labeln */
void makeilit(int n, int labeln)
  { iliterals[nilit] = n;
    ilabels[nilit] = labeln;
    nilit++;
  }

/* Make a literal for float f with label number labeln */
void makeflit(float f, int labeln)
  { fliterals[nflit] = f;
    flabels[nflit] = labeln;
    nflit++;
  }

/* Make a byte literal for string s with label number labeln */
void makeblit(char s[], int labeln)
  { int indx, i, done;
    if ( nblit == 0 )
      { indx = 0;
        blindex[0] = 0;
      }
      else indx = blindex[nblit];
    i = 0;
    done = 0;
    while ( i < 16 && done == 0 )
        { bliterals[indx++] = s[i];
          if ( s[i] == '\0' ) done = 1;
          i++;
        };
    blabels[nblit] = labeln;
    nblit++;
    blindex[nblit] = indx;
  }

typedef struct dtoi {
union {double dbl; int iarr[2]; } val; } Dtoi;

Dtoi dtoitmp;

/* Get half words of a double float */
/* Note: the [0] and [1] below are correct for Linux;
     you may need to switch for other machine or OS. */
int lefth(d)
  double d;
  {  dtoitmp.val.dbl = d;
    return ( dtoitmp.val.iarr[1]); } /* [0] for Sun, [1] for Linux */

int righth(d)
  double d;
  {  dtoitmp.val.dbl = d;
    return ( dtoitmp.val.iarr[0]); } /* [1] for Sun, [0] for Linux */


/* Output literals and end material*/
void outlits()
  {  int i, j, start; double d; int ida, idb;
     cannedcode(bottomcodeb);
     if ( fnegused ) cannedcode(fnegconst);
     for ( i = 0; i < nilit; i++ )
       { printf("\t.align  4\n");
         printf(".LC%d:\n", ilabels[i]);
         printf("\t.long\t%d    \t#  %d\n", iliterals[i], iliterals[i]);
       };
     for ( i = 0; i < nblit; i++ )
       { printf("\t.align  4\n");
         printf(".LC%d:\n", blabels[i]);
         printf("\t.string\t\"%s\"\n", &bliterals[blindex[i]]);
       };
     for ( i = 0; i < nflit; i++ )
       { d = fliterals[i];
         ida = lefth(d);
         idb = righth(d);
         printf("\t.align  8\n");
         printf(".LC%d:\n", flabels[i]);
	 if ( ida == 0 && idb != 0)
           { printf(";***** WARNING: following constant is probably wrong.\n");
	     printf("; See comments about Linux in genasm.c\n"); };
         printf("\t.long\t%d   \t#  %f\n", idb, fliterals[i]);
         printf("\t.long\t%d\n", ida);
       };
     printf("\n");
     cannedcode(bottomcodec);
   }
