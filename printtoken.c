/* printtoken.c      print tokens for debugging           09 Feb 01       */

/* Functions to allocate token records and print tokens for debugging */

/* Copyright (c) 1998 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/* This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License (file gpl.text) for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

/* These arrays are defined for printing debugging information.
   You may use them if you wish, or you may copy them and
   modify them as you see fit. */

static char* opprnt[]  = {" ", "+", "-", "*", "/", ":=", "=", "<>", "<", "<=",
                          ">=", ">",  "^", ".", "and", "or", "not", "div",
                          "mod", "in", "if", "goto", "progn", "label",
                          "funcall", "aref", "program", "float"};
static char *delprnt[] = { "  ", " ,", " ;", " :", " (", " )", " [", " ]",
                           ".."} ;
static char *resprnt[] = { " ", "array", "begin", "case", "const", "do",
                           "downto", "else", "end", "file", "for",
		           "function", "goto", "if", "label", "nil",
                           "of", "packed", "procedure", "program", "record",
                           "repeat", "set", "then", "to", "type",
		           "until", "var", "while", "with" };

TOKEN talloc()           /* allocate a new token record */
  { TOKEN tok;
    tok = (TOKEN) calloc(1,sizeof(struct tokn));
    if ( tok != NULL ) return (tok);
       else printf("talloc failed.");
  }

void printtoken(TOKEN tok)
  {
    switch (tok->tokentype)
	{case OPERATOR:
           printf ("tokentype: %2d  which: %4d   %10s\n",
	           tok->tokentype, tok->whichval,
                   opprnt[tok->whichval] );
           break;
         case DELIMITER:
           printf ("tokentype: %2d  which: %4d   %10s\n",
	           tok->tokentype, tok->whichval,
                   delprnt[tok->whichval] );
           break;
         case RESERVED:
           printf ("tokentype: %2d  which: %4d   %10s\n",
	           tok->tokentype, tok->whichval,
                   resprnt[tok->whichval] );
           break;
         case IDENTIFIERTOK: case STRINGTOK:
           printf ("tokentype: %2d  value:  %16s\n",
	           tok->tokentype, tok->stringval);
           break;
         case NUMBERTOK:
           switch (tok->datatype)
             {case INTEGER:
                printf ("tokentype: %2d  type:  %4d %12d\n",
	                tok->tokentype, tok->datatype, tok->intval);
                break;
	      case REAL:
                printf ("tokentype: %2d  type:  %4d %12e\n",
	                tok->tokentype, tok->datatype, tok->realval);
                break;
	      }
	 }
  }
