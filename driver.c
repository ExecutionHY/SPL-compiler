/* driver.c        Driver for Assembly Code for X86    ; 01 Aug 12     */

/* Copyright (c) 2012 Gordon S. Novak Jr. and The University of Texas at Austin
    */

/* 11 Aug 06; 19 Jul 12; 30 Jul 12 */

/* Routines for use with CS 375 Code Generation assignment.
   To use:    cc driver.c test.s -lm
              a.out
      where test.s is the assembly language program to be tested.       */

/* Written by Gordon S. Novak Jr.                  */

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

void graph1();

int main()
  { printf("calling graph1\n");
    graph1();
    printf("exit from graph1\n");
  }

/* write for a string */
void write(char str[])  {  printf("%s", str);  }

/* writeln for a string */
void writeln(char str[])  {  printf("%s\n", str);  }

/* write for a float */
void writef(double x)  {  printf("%g", x);  }

/* write for an integer */
void writei(int n)  {  printf("%d", n);  }

/* writeln for a float */
void writelnf(double x)  {  printf("%g\n", x);  }

/* writeln for an integer */
void writelni(int n)  {  printf("%d\n", n);  }

/* allocate new storage for a record */
int * new(int size)  { return (int *) malloc(size); }

/* name changed to iround since math.h defines round */
int iround(double x)
  {  int n;
     if ( x >= 0.0 )
       n = x + 0.5;
       else n = x - 0.5;
     return ( n );
   }


/* The following may be useful for debugging.      */

/* sin with trace to print its argument and result */
float trsin(double x)
  { float s;
    s = sin(x);
    printf(" x = %e     sin x = %e\n", x, s);
    return (s);
  }

/* exp with trace to print its argument and result */
float trexp(double x)
  { float s;
    s = exp(x);
    printf(" x = %e     exp x = %e\n", x, s);
    return (s);
  }

/* new with trace to print its argument and result */
int *trnew(int size)
  { int *i;
    i = new(size);
    printf(" new(%d) = %ld\n", size, (long) i);
    return (i);
  }
