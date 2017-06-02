/* lexanl.c                Gordon S. Novak Jr.            06 Jul 12     */

/* This is a main program for testing the lexical analyzer written in lex. */

/* Copyright (c) 2012 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/* 09 Feb 01; 06 Jul 12     */

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

/* To use, enter:
        make lex2                          starter file
        lex2
        i := 3 .

        make lexer                         your version lexan.l
        lexer <graph1.pas >result          run to produce result file */

#include <stdio.h>
#include <ctype.h>
#include "token.h"
#include "lexan.h"

int main()          /* Calls yylex repeatedly to test */
  { 
    int res, done;
    extern TOKEN yylval;
    printf("Started scanner test.\n");
    done = 0;
    while (done == 0)
      { res = yylex();    /* yylex is the entry point to the lex program */
        if (res != 0)
           {
             printf("yylex() = %4d   ", res);
             printtoken(yylval);
           }
           else done = 1;
      }
    return 0;
  }
