%{
/**********************************************************
 *	File:		parse.y
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 4, 2017
 **********************************************************/

/* Yacc reports 1 shift/reduce conflict, due to the ELSE part of
   the IF statement, but Yacc's default resolves it in the right way.*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexan.h"
#include "symtab.h"
#include "parse.h"

/* define the type of the Yacc stack element to be TOKEN */

#define YYSTYPE TOKEN

TOKEN parseresult;
extern int lineCnt;

%}

/* Order of tokens corresponds to tokendefs.c; do not change */

%token ID CONST_STR CONST_INT CONST_REAL CONST_CHAR   /* token types */

%token LP RP LB RB DOTDOT COMMA COLON SEMI    /* Delimiters */

%token DOT PLUS MINUS MUL DIV_R LT LE EQ NE GT GE ASSIGN
%token AND OR NOT DIV MOD /* Operator */

%token ARRAY BEGIN_T CASE CONST DO DOWNTO ELSE END FOR FUNCTION GOTO
%token IF IN OF PACKED PROCEDURE PROGRAM READ RECORD REPEAT SET THEN
%token TO TYPE UNTIL VAR WHILE WITH
%token SYS_CON SYS_FUNCT SYS_PROC SYS_TYPE	/* reserved */

%%

program			: program_head routine DOT			{ parseresult = makeProgram($1, $2); }
				;
program_head	: PROGRAM ID SEMI					{ $$ = $2; }
				;
routine			: routine_head routine_body			{ $$ = cons($1, $2); }
				;
sub_routine		: routine_head routine_body			{ $$ = cons($1, $2); }
				;

routine_head	: label_part const_part type_part var_part routine_part
													{ $$ = $5; }	// definitions are not in code tree
				;
label_part		: /* empty */						{ $$ = NULL; }
				;
/* const part */
const_part		: CONST const_expr_list				{ $$ = NULL; }		// TODO
				| /* empty */						{ $$ = NULL; }
				;
const_expr_list	: const_expr_list ID EQ const_value SEMI
													{ $$ = NULL; }
				| ID EQ const_value SEMI			{ $$ = NULL; }
				;
const_value		: CONST_INT							{ $$ = $1; }
				| CONST_REAL						{ $$ = $1; }
				| CONST_CHAR						{ $$ = $1; }
				| CONST_STR							{ $$ = $1; }
				| SYS_CON							{ $$ = $1; }
				;
/* type part */
type_part		: TYPE type_decl_list				{ $$ = NULL; }		// TODO
				| /* empty */						{ $$ = NULL; }
				;
type_decl_list	: type_decl_list type_definition	{ $$ = cons($1, $2); }
				| type_definition					{ $$ = $1; }
				;
type_definition	: ID EQ type_decl SEMI				{ $$ = NULL; }		// TODO
				;
type_decl		: simple_type_decl					{ $$ = $1; }
				| array_type_decl					{ $$ = $1; }
				| record_type_decl					{ $$ = $1; }
				;
simple_type_decl: SYS_TYPE							{ $$ = findtype($1); }
				| ID								{ $$ = NULL; }
				| LP name_list RP					{ $$ = NULL; }
				| const_value DOTDOT const_value	{ $$ = NULL; }
				| ID DOTDOT ID						{ $$ = NULL; }
				;
array_type_decl	: ARRAY LB simple_type_decl RB OF type_decl
													{ $$ = NULL; }	// instarray
				;
record_type_decl: RECORD field_decl_list END		{ $$ = NULL; }	// intrec
				;
field_decl_list	: field_decl_list field_decl		{ $$ = cons($1, $2); }
				| field_decl						{ $$ = $1; }
				;
field_decl		: name_list COLON type_decl SEMI	{ $$ = NULL; }	// instfields
				;
name_list		: name_list COMMA ID				{ $$ = cons($1, $3); }
				| ID								{ $$ = $1; }
				;
/* var part */
var_part		: VAR var_decl_list					{ $$ = $2; }
				| /* empty */						{ $$ = NULL; }
				;
var_decl_list	: var_decl_list var_decl			{ $$ = cons($1, $2); }
				| var_decl							{ $$ = $1; }
				;
var_decl		: name_list COLON type_decl SEMI	{ instvars($1, $3); }	// install vars
				;
/* routine part */	//TODO
routine_part	:									{ $$ = NULL; }
				;

/* routine body */
routine_body	: compound_stmt						{ $$ = $1; }
				;
compound_stmt	: BEGIN_T stmt_list END				{ $$ = $2; }
				;
stmt_list		: stmt_list stmt SEMI				{ $$ = cons($1, $2); }
				| /* empty */						{ $$ = NULL; }
				;
stmt			: CONST_INT COLON non_label_stmt	{ $$ = dolabel($1, $2, $3); }	// dolabel
				| non_label_stmt					{ $$ = NULL; }
				;
non_label_stmt	: assign_stmt						{ $$ = $1; }
				| proc_stmt							{ $$ = $1; }
				| compound_stmt						{ $$ = $1; }
				| if_stmt							{ $$ = $1; }
				| repeat_stmt						{ $$ = $1; }
				| while_stmt						{ $$ = $1; }
				| for_stmt							{ $$ = $1; }
				| case_stmt							{ $$ = $1; }
				| goto_stmt							{ $$ = $1; }
				;
assign_stmt		: ID ASSIGN expression				{ $$ = binop($2, $1, $3); }
				| ID LB expression RB ASSIGN expression
													{ $$ = binop($5, makefuncall($2, $1, $3), $6); }
				| ID DOT ID ASSIGN expression		{ $$ = binop($4, makefuncall($2, $1, $3), $5); }
				;
proc_stmt		: ID								{ $$ = NULL; }
				| ID LP args_list RP				{ $$ = NULL; }
				| SYS_PROC							{ $$ = NULL; }
				| SYS_PROC LP expression_list RP	{ $$ = NULL; }
				| READ LP factor RP					{ $$ = NULL; }
				;
if_stmt			: IF expression THEN stmt else_clause		
													{ $$ = NULL; }
				;
else_clause		: ELSE stmt							{ $$ = NULL; }
				|									{ $$ = NULL; }
				;
repeat_stmt		: REPEAT stmt_list UNTIL expression	{ $$ = NULL; }
				;
while_stmt		: WHILE expression DO stmt			{ $$ = NULL; }
				;
for_stmt		: FOR ID ASSIGN expression direction expression DO stmt
													{ $$ = NULL; }
				;
direction		: TO								{ $$ = NULL; }
				| DOWNTO							{ $$ = NULL; }
				;
case_stmt		: CASE expression OF case_expr_list END
													{ $$ = NULL; }
				;
case_expr_list	: case_expr_list case_expr			{ $$ = NULL; }
				| case_expr							{ $$ = NULL; }
				;
case_expr		: const_value COLON stmt SEMI		{ $$ = NULL; }
				| ID COLON stmt SEMI				{ $$ = NULL; }
				;
goto_stmt		: GOTO CONST_INT					{ $$ = NULL; }
				;
expression_list	: expression_list COMMA expression	{ $$ = NULL; }
				|									{ $$ = NULL; }
				;
expression		: expression GE expr				{ $$ = NULL; }
				| expression GT expr				{ $$ = NULL; }
				| expression LE expr				{ $$ = NULL; }
				| expression LT expr				{ $$ = NULL; }
				| expression EQ expr				{ $$ = NULL; }
				| expression NE expr				{ $$ = NULL; }
				| expr								{ $$ = $1; }
				;
expr			: expr PLUS term					{ $$ = NULL; }
				| expr MINUS term					{ $$ = NULL; }
				| expr OR term						{ $$ = NULL; }
				| term								{ $$ = $1; }
				;
term			: term MUL factor					{ $$ = NULL; }
				| term DIV factor					{ $$ = NULL; }
				| term MOD factor					{ $$ = NULL; }
				| term AND factor					{ $$ = NULL; }
				| factor							{ $$ = $1; }
				;
factor			: ID								{ $$ = NULL; }
				| ID LP args_list RP				{ $$ = NULL; }
				| SYS_FUNCT							{ $$ = NULL; }
				| SYS_FUNCT LP args_list RP			{ $$ = NULL; }
				| const_value						{ $$ = $1; }
				| LP expression RP					{ $$ = NULL; }
				| NOT factor						{ $$ = NULL; }
				| MINUS factor						{ $$ = NULL; }
				| ID LB expression RB				{ $$ = NULL; }
				| ID DOT ID							{ $$ = NULL; }
				;
args_list		: args_list COMMA expression		{ $$ = NULL; }
				| expression						{ $$ = NULL; }

%%


/* See parse.h for all debug constants */

int labelnumber = 0;  /* sequential counter for internal label numbers */
int debug_call_num = 0;	/* sequential counter for finding a specific place during program execution */
char *last_method = "yyparse()"; 	/* the method that called the currently-executing method, if applicable */


TOKEN makeProgram(TOKEN program_name, TOKEN routine) {
	if (DEBUG & DB_MAKEPROGRAM) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeProgram(),\n");
		dbugprint2toks(program_name, routine);
		last_method = "makeProgram()";
	}
	
	TOKEN program = makeop(OP_PROGRAM);    //26
	if (!program) {
		printf("Error: failed to alloc TOKEN in makeProgram().\n");
		return NULL;
	}
	
	// Create the head of tree [parseresult]
	program->operands = program_name;
	program_name->link = routine;
	if (routine) routine->link = NULL;
	
	if (DEBUG & DB_MAKEPROGRAM) {
		printf("Finished makeProgram().\n");
		dbugprinttok(program);
	}
	return program;
	
}



TOKEN cons(TOKEN list, TOKEN item) {
	if (list == NULL) return item;
	TOKEN t = list;
	while (t->link != NULL) {
		t = t->link;
	}
	t->link = item;
	if (DEBUG & DB_CONS) {
		printf("cons\n");
		dbugprinttok(list);
		dbugprinttok(item);
	};
	return item;
}

/* arrayref processes an array reference a[i]
 subs is a list of subscript expressions.
 tok and tokb are (now) unused tokens that are recycled. */
TOKEN arrayref(TOKEN arr, TOKEN tok, TOKEN subs, TOKEN tokb) {
	if (DEBUG & DB_ARRAYREF) {
		printf("(%d)\n", debug_call_num++);
		printf("In arrayref(), from %s\n", last_method);
		dbugprint4args(arr, tok, subs, tokb);
		last_method = "arrayref()";
	}
	
	TOKEN array_ref = NULL;
	SYMBOL arr_varsym, typesym, temp;
	SYMBOL arrsyms[10];
	
	arr_varsym = searchst(arr->stringval);
	
	if (!arr_varsym) {
		printf(" Error: array \"%s\" not found in symbol table, arrayref().\n", arr->stringval);
		return NULL;
	}
	
	temp = arr_varsym->dataType;
	
	int counter = 0;
	int num_arr_dims = 0;   // not being used for anything
	
	/* Store the SYM_ARRAYs into an array */
	while (temp && temp->kind != SYM_TYPE) {
		arrsyms[counter] = temp;
		num_arr_dims++;
		counter++;
		temp = temp->dataType;
	}
	
	/* The type of the array */
	typesym = temp;
	
	if (subs->link == NULL && (subs->tokenType == TOKEN_NUM)) {
		int total_size = (subs->whichval - 1) * typesym->size;
		array_ref = makearef(arr, makeintc(total_size), NULL);
		array_ref->dataType = arr_varsym->basicType;
		return array_ref;
	}
	// else if tokenType is IDENTIFIER?
	
	counter = 0;
	TOKEN curr_sub = subs;
	
	while (curr_sub) {
		
		if (counter == 0) {     // first iteration; need to make aref
			SYMBOL curr_sym = arrsyms[counter];
			int curr_size = curr_sym->size / (curr_sym->highBound - curr_sym->lowBound + 1);
			
			// TOKEN mul_op = maketimes(makeintc(curr_size), curr_sub, NULL);
			
			TOKEN mul_op = makeop(OP_MUL);
			TOKEN pos_typesym_size = makeintc(curr_size);
			TOKEN neg_typesym_size = makeintc(curr_size * -1);
			
			mul_op->operands = pos_typesym_size;
			pos_typesym_size->link = curr_sub;
			
			neg_typesym_size->link = mul_op;
			
			TOKEN plus_op = makeplus(neg_typesym_size, mul_op, NULL);
			
			array_ref = makearef(arr, plus_op, NULL);
			array_ref->dataType = arr_varsym->basicType;
			
		}
		else {
			
			if (curr_sub->tokenType == TOKEN_NUM) {      // smash constants (optimization)
				TOKEN add_to = array_ref->operands->link->operands;
				add_to = addint(add_to, makeintc(curr_sub->whichval * typesym->size), NULL);
			}
			else {
				/* Smash constants and add to tree */
				
				SYMBOL curr_sym = arrsyms[counter];
				int curr_size = curr_sym->size / (curr_sym->highBound - curr_sym->lowBound + 1);
				
				TOKEN mul_op = makeop(OP_MUL);
				TOKEN pos_typesym_size = makeintc(curr_size);
				TOKEN neg_typesym_size = makeintc(curr_size * -1);
				
				mul_op->operands = pos_typesym_size;
				pos_typesym_size->link = curr_sub;
				
				TOKEN added = array_ref->operands->link->operands;
				added = addint(added, neg_typesym_size, NULL);
				
				TOKEN add_to = array_ref->operands->link->operands->link;
				TOKEN plus_op = makeplus(add_to, mul_op, NULL);
				add_to = plus_op;
				
			}
		}
		
		/* Destroy the current link */
		TOKEN unlink = curr_sub;
		curr_sub = curr_sub->link;
		unlink->link = NULL;
		counter++;
	}
	
	if (DEBUG & DB_ARRAYREF) {
		printf(" Finished arrayref().\n");
		dbugprint1tok(array_ref);
		last_method = "arrayref()";
	}
	
	return array_ref;
}

/* instarray installs an array declaration into the symbol table.
 bounds points to a SUBRANGE symbol table entry.
 The symbol table pointer is returned in token typetok. */
TOKEN instarray(TOKEN bounds, TOKEN typetok) {
	if (DEBUG & DB_INSTARRAY) {
		printf("(%d)\n", debug_call_num++);
		printf("In instarray(), from %s\n", last_method);
		dbugprint2args(bounds, typetok);
		//    dbprsymbol(typetok->symType);
		last_method = "instarray()";
	}
	
	// bounds->symType->[low/highBound]
	
	TOKEN curr_bound = bounds;
	SYMBOL prev_sym = NULL;
	
	SYMBOL typesym = searchst(typetok->stringval);
	int low, high;
	
	// if (!typesym) {
	//    printf(" Error: array \"%s\" not found in symbol table, instarray().\n", typetok->stringval);
	//    return NULL;
	// }
	
	while (curr_bound) {
		SYMBOL arrsym = symalloc();
		arrsym->kind = SYM_ARRAY;
		
		if (typesym) {
			arrsym->dataType = typesym;
		}
		else {
			//       arrsym->basicType = typetok->dataType;
		}
		
		low = curr_bound->symType->lowBound;
		high = curr_bound->symType->highBound;
		
		if (!prev_sym) {
			arrsym->size = (high - low + 1) * typesym->size;
		}
		else {
			arrsym->dataType = typetok->symType;
			arrsym->size = (high - low + 1) * prev_sym->size;
		}
		
		typetok->symType = arrsym;
		prev_sym = arrsym;
		
		arrsym->lowBound = low;
		arrsym->highBound = high;
		
		curr_bound = curr_bound->link;
	}
	
	if (DEBUG & DB_INSTARRAY) {
		printf(" Finished instarray().\n");
		dbugprint1tok(typetok);
	}
	
	return typetok;
}

/* instrec will install a record definition.  Each token in the linked list
 argstok has a pointer its type.  rectok is just a trash token to be
 used to return the result in its symType */
TOKEN instrec(TOKEN rectok, TOKEN argstok) {
	if (DEBUG & DB_INSTREC) {
		printf("In instrec(), from %s\n", last_method);
		dbugprint2args(rectok, argstok);
		last_method = "instrec()";
	}
	
	int total_size, offset;
	
	SYMBOL recsym = symalloc();
	recsym->kind = SYM_RECORD;
	rectok->symType = recsym;
	recsym->dataType = argstok->symType;
	offset = wordaddress(argstok->symType->size, 8);   // TODO: test this w/ something size 8
	total_size = offset;
	
	if (DEBUG & DB_INSTREC) {
		printf(" Installing...\n");
		dbugprint1sym(recsym->dataType);
	}
	
	TOKEN curr = argstok;
	TOKEN next = argstok->link;
	while (next) {
		/* Link the symTypes of each TOKEN (the record fields) together
		 and calculate the total size of the record */
		
		curr->symType->link = next->symType;
		offset = wordaddress(next->symType->size, 8);
		next->symType->offset = total_size;
		total_size += offset;
		
		if (DEBUG & DB_INSTREC) {
			printf("  ");
			dbprsymbol(next->symType);
		}
		
		curr = next;
		next = next->link;
	}
	
	recsym->size = total_size;
	
	if (DEBUG & DB_INSTREC) {
		printf(" Finished instrec().\n");
		dbugprint1tok(rectok);
	}
	
	return rectok;
}

/* insttype will install a type name in symbol table.
 typetok is a token containing symbol table pointers. */
void insttype(TOKEN typename, TOKEN typetok) {
	if (DEBUG & DB_INSTTYPE) {
		printf("(%d)\n", debug_call_num++);
		printf("In insttype(), from %s\n", last_method);
		dbugprint2args(typename, typetok);
		printf(" Installing...\n");
		last_method = "insttype()";
	}
	
	SYMBOL sym, typesym;
	
	typesym = typetok->symType;
	
	sym = searchins(typename->stringval);  // insert if not found
	sym->kind = SYM_TYPE;
	sym->size = typesym->size;
	sym->dataType = typesym;
	sym->basicType = typesym->basicType;
	
	if (DEBUG & DB_INSTTYPE) {
		printf(" ");
		dbugprinttok(typename);
		printf(" ");
		dbprsymbol(sym);
		printf(" ");
		dbprsymbol(typesym);
		printf("\n");
	}
	
}

/* makesubrange makes a SUBRANGE symbol table entry, puts the pointer to it
 into tok, and returns tok. */
TOKEN makesubrange(TOKEN tok, int low, int high) {
	if (DEBUG & DB_MAKESUBRANGE) {
		printf("(%d)\n", debug_call_num++);
		printf("In makesubrange(), from %s\n", last_method);
		dbugprint1arg(tok);
		printf("  low bound: %d\n  high bound: %d\n\n", low, high);
		last_method = "makesubrange()";
	}
	if (low > high) {
		printf(" Error: low bound cannot be greater than high bound\n\n");
		return NULL;
	}
	
	TOKEN out = copytok(tok);
	
	SYMBOL subrange_entry = symalloc();
	subrange_entry->kind = SYM_SUBRANGE;
	subrange_entry->basicType = TYPE_INT;
	subrange_entry->lowBound = low;
	subrange_entry->highBound = high;
	subrange_entry->size = basicsizes[TYPE_INT]; // each entry is a constant (int)
	
	out->symType = subrange_entry;
	
	if (DEBUG & DB_ADDOFFS) {
		printf(" Finished makesubrange().\n");
		dbugprint1tok(out);
		last_method = "makesubrange()";
	}
	
	return out;
}

TOKEN get_rec(TOKEN rec, TOKEN offset) {
	TOKEN out = NULL;
	
	SYMBOL recsym = rec->symType;
	while (recsym && !out) {
		
		if (recsym->offset == offset->whichval) {
			out = talloc();
			out->symType = recsym;
		}
		else {
			recsym = recsym->link;
		}
	}
	
	return out;
}

/* Search the given record for the specified field.
 Returns a pointer to the record field SYMBOL if
 found (stored in TOKEN->symType), NULL otherwise. */
TOKEN get_rec_field(TOKEN rec, TOKEN field) {
	
	TOKEN out = NULL;
	
	SYMBOL recsym = rec->symType;
	while (recsym && !out) {
		if (strcmp(recsym->nameString, field->stringval) == 0) {
			out = talloc();
			out->symType = recsym;
		}
		else {
			recsym = recsym->link;
		}
	}
	
	return out;
}

/* Helper method for reducedot(). Searches for the offset
 of the given field in var. Returns a TOKEN_NUM whose
 whichval field is the offset if found, otherwise returns
 with a whichval of -1. */
TOKEN get_offset(TOKEN var, TOKEN field) {
	
	TOKEN offset = makeintc(-1);
	
	TOKEN root_name = get_last_operand(var);
	TOKEN last_offset = get_last_link(var->operands);
	
	SYMBOL found = NULL;
	SYMBOL varsym = searchst(root_name->stringval);
	
	int var_is_arefop = 0;
	if (var->whichval == OP_AREF) {
		var_is_arefop = 1;
	}
	
	if (!varsym) {
		printf(" Error: could not find symbol \"%s\" in symbol table, get_offset():1.\n", root_name->stringval);
		return NULL;
	}
	
	SYMBOL temp = varsym;
	while (temp) {
		if (temp->dataType && temp->dataType->kind == SYM_BASIC) {
			break;
		}
		temp = temp->dataType;
	}
	
	if (!temp) {
		printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():2.\n", root_name->stringval);
		return NULL;
	}
	
	while (temp) {
		
		if ((strcmp(temp->nameString, field->stringval) == 0)) {
			found = temp;
			if (var_is_arefop) {
				
				/* TOKEN var is an aref TOKEN; ignore */
				if (last_offset->tokenType == OPERATOR) {
					offset->whichval = -1;
				}
				else {
					offset->whichval = last_offset->whichval + found->offset;
				}
				
				/* Set offset's link field to a REAL TOKEN_NUM.
				 Is necessary to ensure correct behaviour of
				 binop() during type checking/coercion.
				 
				 The link MUST be nulled out after use, otherwise
				 the console output will be incorrect. */
				if (found->dataType->basicType == TYPE_REAL) {
					offset->link = makerealtok(0);
					offset->link->realval = -DBL_MAX;
				}
			}
			else {
				
				offset->whichval = found->offset;
				if (found->dataType->basicType == TYPE_REAL) {
					offset->link = makerealtok(0);
					offset->link->realval = -DBL_MAX;
				}
			}
			
			return offset;
		}
		else if (var_is_arefop && (temp->offset == last_offset->whichval)) {
			found = temp;
			break;
		}
		
		temp = temp->link;
	}
	
	/* NOT an error condition if !found here. */
	
	if (!var_is_arefop && found) {
		offset->whichval = found->offset;
		if (found->dataType->basicType == TYPE_REAL) {
			offset->link = makerealtok(0);
			offset->link->realval = -DBL_MAX;
		}
		return offset;
	}
	else if (var_is_arefop && found) {
		/* TOKEN field represents a record field; search for
		 the record in the symbol table and then search for
		 the field within that record. */
		
		SYMBOL temp1 = searchst(found->dataType->nameString);
		found = NULL;
		if (!temp1) {
			printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():3.\n", root_name->stringval);
			return NULL;
		}
		
		while (temp1) {
			if (temp1->dataType && temp1->dataType->kind == SYM_BASIC) {
				break;
			}
			temp1 = temp1->dataType;
		}
		
		if (!temp1) {
			printf(" Error: symbol table entry \"%s\" is corrupt, get_offset():4.\n", root_name->stringval);
			return NULL;
		}
		
		while (temp1 && !found) {
			if (strcmp(temp1->nameString, field->stringval) == 0) {
				found = temp1;
			}
			else {
				temp1 = temp1->link;
			}
		}
		
		/* NOT an error condition if !found here. */
		if (found) {
			offset->whichval = last_offset->whichval + found->offset;
			if (found->dataType->basicType == TYPE_REAL) {
				offset->link = makerealtok(0);
				offset->link->realval = -DBL_MAX;
			}
			
			return offset;
		}
	}
	
	return offset;
}

/* reducedot handles a record reference.
 dot is a (now) unused token that is recycled. */
TOKEN reducedot(TOKEN var, TOKEN dot, TOKEN field) {
	if (DEBUG & DB_REDUCEDOT) {
		printf("(%d)\n", debug_call_num++);
		printf("In reducedot(), from %s\n", last_method);
		dbugprint3args(var, dot, field);
		last_method = "reducedot()";
	}
	
	TOKEN aref;
	TOKEN offset = get_offset(var, field);
	
	if (var->whichval == OP_AREF) {
		/* Avoid adding multiple copies to the tree. */
		
		if (offset->whichval >= 0) {
			var->operands->link = offset;
		}
		aref = var;
	}
	else {
		aref = makearef(var, offset, NULL);
	}
	
	/* Change aref's dataType to REAL to avoid incorrect
	 fixes/floats in binop(). NULL out the dummy link. */
	if (offset->link && offset->link->dataType == TYPE_REAL &&
	offset->link->realval == -DBL_MAX) {
		
		aref->dataType = TYPE_REAL;
		offset->link = NULL;
	}
	
	if (DEBUG & DB_REDUCEDOT) {
		printf(" Finished reducedot().\n");
		dbugprint1tok(aref);
		last_method = "reducedot()";
		// ppexpr(aref);
		// printf("\n");
	}
	
	return aref;
}

/* ############################################################################################################################################################### */

/* addint adds integer off to expression exp, possibly using tok */
TOKEN addint(TOKEN exp, TOKEN off, TOKEN tok) {
	if (!exp || !off) {
		return NULL;
	}
	
	int a = exp->intval;
	int b = off->intval;
	
	if (DEBUG & DB_ADDINT) {
		printf("(%d)\n", debug_call_num++);
		printf("In addint(), from %s\n Adding %d to %d\n", last_method, b, a);
		dbugprint3args(exp, off, tok);
		last_method = "addint()";
	}
	
	if ((b > 0) && (a > INT_MAX - b)) { // if ((INT_MAX / exp->intval) > off->intval)
		printf(" Error: integer overflow detected, addint()\n");
		printf(" Cannot add %d to %d\n", b, a);
	}
	else {
		exp->intval = a + b;
	}
	
	if (DEBUG & DB_ADDINT) {
		printf(" New value of exp: %d\n", exp->intval);
		dbugprint1tok(exp);
	}
	
	return exp;
}

/* addoffs adds offset, off, to an aref expression, exp */
TOKEN addoffs(TOKEN exp, TOKEN off) {
	if (DEBUG & DB_ADDOFFS) {
		printf("(%d)\n", debug_call_num++);
		printf("In addoffs(), from %s\n", last_method);
		dbugprint2args(exp, off);
		last_method = "addoffs()";
	}
	
	exp = addint(exp, off, NULL);
	
	if (DEBUG & DB_ADDOFFS) {
		printf(" Finished addoffs().\n");
		dbugprint1tok(exp);
		last_method = "addoffs()";
	}
	
	return exp;
}

/* binop links a binary operator op to two operands, lhs and rhs.
 
 TODO
 - boolean types?
 - funcall return types?
 */
TOKEN binop(TOKEN op, TOKEN lhs, TOKEN rhs) {
	
	if (DEBUG & DB_BINOP) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In binop(), from %s\n", last_method);
		dbugprint3args(op, lhs, rhs);
		last_method = "binop()";
	}
	
	int lhs_dataType = lhs->dataType;
	int rhs_dataType = rhs->dataType;
	int op_type = op->whichval;
	
	/* Type checking/coercion needed. */
	
	if (lhs_dataType != rhs_dataType) {
		op = binop_type_coerce(op, lhs, rhs);
	}
	else {
		op->dataType = lhs->dataType;
		op->operands = lhs;     // link operands to operator
		lhs->link = rhs;     // link second operand to first
		rhs->link = NULL;    // terminate operand list
	}
	
	if (DEBUG & DB_BINOP) {
		printf(" Finished binop().\n");
		dbugprint1tok(op);
		last_method = "binop()";
		//    ppexpr(op);
	}
	
	return op;
}

/* This method fixes or floats a TOKEN_NUM as necessary. */
TOKEN binop_type_coerce(TOKEN op, TOKEN lhs, TOKEN rhs) {
	
	TOKEN cast_tok;
	
	int lhs_dataType = lhs->dataType;
	int rhs_dataType = rhs->dataType;
	int op_type = op->whichval;
	
	if (lhs_dataType == TYPE_INT && rhs_dataType == TYPE_REAL) {
		/* LHS is integer, RHS is real.
		 Fix if OP_ASSIGN, else float the integer. */
		
		op->dataType = TYPE_REAL;
		
		if (op_type == OP_ASSIGN) {
			cast_tok = makefix(rhs);
			op->operands = lhs;
			lhs->link = cast_tok;
		}
		else {
			cast_tok = makefloat(lhs);
			op->operands = cast_tok;
			cast_tok->link = rhs;
		}
		
	}
	else if (lhs_dataType == TYPE_REAL && rhs_dataType == TYPE_INT) {
		/* LHS is real, RHS is integer.
		 Float the integer value.
		 DO NOT fix; lhs still takes precedence. */
		
		cast_tok = makefloat(rhs);
		
		op->dataType = TYPE_REAL;
		op->operands = lhs;
		lhs->link = cast_tok;
		cast_tok->link = NULL;
		rhs->link = NULL;
		
	}
	else {
		op->dataType = lhs->dataType;
		op->operands = lhs;     // link operands to operator
		lhs->link = rhs;     // link second operand to first
		rhs->link = NULL;    // terminate operand list
	}
	
	return op;
}


/* copytok makes a new token that is a copy of origtok */
TOKEN copytok(TOKEN origtok) {
	
	if (DEBUG & DB_COPYTOK) {
		printf("(%d)\n", debug_call_num++);
		printf("In copytok(), from %s\n", last_method);
		dbugprint1arg(origtok);
		last_method = "copytok()";
	}
	
	if (!origtok) {
		return NULL;
	}
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, copytok().\n");
		return NULL;
	}
	
	out->tokenType = origtok->tokenType;
	out->dataType = origtok->dataType;
	
	/* Shallow copies only (as expected). */
	out->symType = origtok->symType;
	out->symEntry = origtok->symEntry;
	out->operands = origtok->operands;
	out->link = origtok->link;
	
	if (origtok->stringval[0] != '\0') {
		strncpy(out->stringval, origtok->stringval, 16);
	}
	else if (origtok->whichval != -1) {
		out->whichval = origtok->whichval;
	}
	else if (origtok->realval != -DBL_MIN) {
		out->realval = origtok->realval;
	}
	else {
		out->intval = origtok->intval;
	}
	
	if (DEBUG & DB_COPYTOK) {
		printf(" Finished copytok().\n");
		printf("  Original:\n    ");
		dbugprinttok(origtok);
		printf("  Copy:\n    ");
		dbugprinttok(out);
		printf("\n");
	}
	
	return out;
}

/* dogoto is the action for a goto statement.
 tok is a (now) unused token that is recycled. */
TOKEN dogoto(TOKEN tok, TOKEN labeltok) {
	
	// THIS METHOD SHOULD ONLY BE CALLED FOR A USER LABEL
	
	if (DEBUG & DB_DOGOTO) {
		printf("(%d)\n", debug_call_num++);
		printf("In dogoto(), from %s\n", last_method);
		dbugprint2args(tok, labeltok);
		last_method = "dogoto()";
	}
	
	if (labeltok->intval < 0) {
		printf(" Warning: label number is negative, dogoto().\n");
	}
	
	int internal_label_num = get_internal_label_num(labeltok->intval);
	if (internal_label_num == -1) {
		printf(" Error: could not find internal label number corresponding to user label number %d\n", labeltok->intval);
		return NULL;
	}
	
	if (DEBUG & DB_DOGOTO) {
		printf(" Finished dogoto().\n");
		printf("  Found internal label number %d corresponding to user label number %d\n", internal_label_num, labeltok->intval);
		printf("  Transferring to makegoto()...\n\n");
	}
	
	return (makegoto(internal_label_num));
}

/* dolabel is the action for a label of the form   <number>: <statement>
 tok is a (now) unused token that is recycled. */
TOKEN dolabel(TOKEN labeltok, TOKEN tok, TOKEN statement) {
	
	if (DEBUG & DB_DOLABEL) {
		printf("(%d)\n", debug_call_num++);
		printf("In dolabel(), from %s\n", last_method);
		dbugprint3args(labeltok, tok, statement);
		last_method = "dolabel()";
	}
	
	int internal_label_num = get_internal_label_num(labeltok->intval);
	if (internal_label_num == -1) {
		printf(" Error: label %d not found in label table\n", labeltok->intval);
		return NULL;
	}
	
	TOKEN do_progn_tok = makeop(OP_PROGN);
	TOKEN label_tok = makeop(OP_LABEL);
	// TOKEN do_progn_tok = makeprogn(makeop(OP_PROGN), makeop(OP_LABEL));  // second arg will never be progn, so skip makepnb()
	TOKEN label_num_tok = makeintc(internal_label_num);
	
	if (!do_progn_tok || !label_tok || !label_num_tok) {
		printf(" Failed to alloc label TOKEN(s), dolabel().\n");
		return NULL;
	}
	
	do_progn_tok->operands = label_tok;
	label_tok->operands = label_num_tok;
	label_tok->link = statement;
	
	if (DEBUG & DB_DOLABEL) {
		printf(" Finished dolabel().\n");
		dbugprint1tok(do_progn_tok);
		last_method = "dolabel()";
	}
	
	return do_progn_tok;
}

/* dopoint handles a ^ operator.
 tok is a (now) unused token that is recycled. */
TOKEN dopoint(TOKEN var, TOKEN tok) {
	if (DEBUG & DB_DOPOINT) {
		printf("(%d)\n", debug_call_num++);
		printf("In dopoint(), from %s\n", last_method);
		dbugprint2args(var, tok);
		last_method = "dopoint()";
	}
	
	tok->operands = var;
	
	if (DEBUG & DB_DOPOINT) {
		printf(" Finished dopoint().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

/* findid finds an identifier in the symbol table, sets up symbol table\n
 pointers, changes a constant to its number equivalent
 
 Adapted from Prof. Novak's class notes with permission. */
TOKEN findid(TOKEN tok) {
	
	if (DEBUG & DB_FINDID) {
		printf("(%d)\n", debug_call_num++);
		printf("In findid(), from %s\n", last_method);
		dbugprint1arg(tok);
		last_method = "findid()";
	}
	
	SYMBOL sym, typ;
	sym = searchst(tok->stringval);
	
	if (sym->kind == SYM_CONST) {
		
		tok->tokenType = TOKEN_NUM;
		
		if (sym->basicType == 0) {
			tok->dataType = TYPE_INT;
			tok->intval = sym->constval.intNum;
		}
		else if (sym->basicType == 1) {
			tok->dataType = TYPE_REAL;
			tok->realval = sym->constval.realNum;
		}
		
	}
	else {
		
		tok->symEntry = sym;
		typ = sym->dataType;
		tok->symType = typ;
		
		if (typ->kind == SYM_BASIC || typ->kind == SYM_POINTER) {
			tok->dataType = typ->basicType;
		}
		
	}
	
	if (DEBUG & DB_FINDID) {
		printf(" Finished findid().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

/* findtype looks up a type name in the symbol table, puts the pointer
 to its type into tok->symType, returns tok. */
TOKEN findtype(TOKEN tok) {
	
	if (DEBUG & DB_FINDTYPE) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In findtype(), from %s\n", last_method);
		dbugprint1arg(tok);
		last_method = "findtype()";
	}
	
	SYMBOL sym, typ;
	// sym = searchins(tok->stringval); // triggers segfault
	sym = searchst(tok->stringval);
	
	if (!sym) {
		printf(" Error: type \"%s\" not found in symbol table, findtype().\n", tok->stringval);
		return NULL;
	}
	
	typ = sym->dataType;
	int kind = sym->kind;
	
	/* Arg represents symbol of basic dataType (INTEGER, REAL, STRINGTYPE, BOOLETYPE, POINTER) */
	if (kind == SYM_BASIC) {
		tok->dataType = sym->basicType;
		tok->symType = sym;
	}
	else {
		tok->symType = typ;
	}
	
	if (DEBUG & DB_FINDTYPE) {
		printf(" Finished findtype().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

/* Gets and returns the last TOKEN (in)directly
 connected via ->link to TOKEN tok. Mostly used
 to handle elimination of nested progns. */
TOKEN get_last_link(TOKEN tok) {
	if (!tok) {
		return NULL;
	}
	
	TOKEN curr = tok;
	TOKEN curr_link = curr->link;
	while (curr_link) {
		curr = curr_link;
		curr_link = curr_link->link;
	}
	
	return curr;
}

/* Gets and returns the last TOKEN (in)directly
 connected via ->operands to TOKEN tok. */
TOKEN get_last_operand(TOKEN tok) {
	if (!tok) {
		return NULL;
	}
	
	TOKEN curr = tok;
	TOKEN curr_operand = curr->operands;
	while (curr_operand) {
		curr = curr_operand;
		curr_operand = curr_operand->operands;
	}
	
	return curr;
}

/* instconst installs a constant in the symbol table */
void instconst(TOKEN idtok, TOKEN consttok) {
	
	if (DEBUG & DB_INSTCONST) {
		printf("(%d)\n", debug_call_num++);
		printf("In instconstant(), from %s\n", last_method);
		dbugprint2args(idtok, consttok);
		last_method = "instconst()";
	}
	
	SYMBOL sym;
	
	sym = insertsym(idtok->stringval);
	sym->kind = SYM_CONST;
	sym->basicType = consttok->dataType;
	
	if (sym->basicType == TYPE_INT) {      // INTEGER
		sym->constval.intNum = consttok->intval;
		sym->size = basicsizes[TYPE_INT];   // 4
	}
	else if (sym->basicType == TYPE_REAL) {   // REAL
		sym->constval.realNum = consttok->realval;
		sym->size = basicsizes[TYPE_REAL];  // 8
	}
	else if (sym->basicType == TYPE_STR) {
		strncpy(sym->constval.stringConst, consttok->stringval, 16);
		sym->size = basicsizes[TYPE_STR];
	}
	
	if (DEBUG & DB_INSTCONST) {
		printf(" Finished instconstant().\n");
		dbugprint2toks(idtok, consttok);
	}
	
}

/* instdotdot installs a .. subrange in the symbol table.
 dottok is a (now) unused token that is recycled. */
TOKEN instdotdot(TOKEN lowtok, TOKEN dottok, TOKEN hightok) {
	if (DEBUG & DB_INSTDOTDOT) {
		printf("(%d)\n", debug_call_num++);
		printf("In instdotdot(), from %s\n", last_method);
		dbugprint3args(lowtok, dottok, hightok);
		last_method = "instdotdot()";
	}
	
	TOKEN out = makesubrange(dottok, lowtok->intval, hightok->intval);
	
	if (DEBUG & DB_INSTDOTDOT) {
		printf(" Finished instdotdot().\n");
		dbugprint1tok(out);
		last_method = "instdotdot()";
	}
	
	return out;
}

/* instenum installs an enumerated subrange in the symbol table,
 e.g., type color = (red, white, blue)
 by calling makesubrange and returning the token it returns. */
TOKEN instenum(TOKEN idlist) {
	if (DEBUG & DB_INSTENUM) {
		printf("(%d)\n", debug_call_num++);
		printf("In instenum(), from %s\n", last_method);
		dbugprint1arg(idlist);
		last_method = "instenum()";
	}
	
	int total_size = 0;
	TOKEN temp = idlist;
	while (temp) {
		instconst(temp, makeintc(total_size));
		total_size++;
		temp = temp->link;
	}
	
	TOKEN subrange_tok = makesubrange(idlist, 0, (total_size - 1));
	
	if (DEBUG & DB_INSTENUM) {
		printf(" Finished instenum().\n");
		dbugprint1tok(subrange_tok);
		last_method = "instenum()";
	}
	
	return subrange_tok;
}

/* instlabel installs a user label into the label table */
void instlabel (TOKEN num) {
	
	// DO NOT CALL THIS METHOD FOR COMPILER-GENERATED LABELS
	
	if (DEBUG & DB_INSTLABEL) {
		printf("(%d)\n", debug_call_num++);
		printf("In dolabel(), from %s\n", last_method);
		dbugprint1arg(num);
		last_method = "instlabel()";
	}
	
	// insert_label(labelnumber++, num->intval);
	insert_label(labelnumber++, num);
	
	if (DEBUG & DB_INSTLABEL) {
		printf(" Finished dolabel().\n");
	}
}

/* instvars will install variables in symbol table.
 typetok is a token containing symbol table pointer for type.
 
 Note that initsyms() is already called in main().
 
 Adapted from Prof. Novak's class notes with permission. */
void instvars(TOKEN idlist, TOKEN typetok) {
	
	if (DEBUG & DB_INSTVARS) {
		printf("(%d) line: %d\n", debug_call_num++, lineCnt);
		printf("In instvars(), from %s\n", last_method);
		printf(" typetok: ");
		ppexpr(typetok);
		dbugprint1tok(typetok);
		printf(" Installing...\n");
		last_method = "instvars()";
	}
	
	SYMBOL sym, typesym;
	int align;
	
	typesym = typetok->symType;
	align = alignsize(typesym);
	
	while (idlist != NULL) {
		sym = insertsym(idlist->stringval);
		sym->kind = SYM_VAR;
		sym->offset = wordaddress(blockoffs[blocknumber], align);
		sym->size = typesym->size;
		blockoffs[blocknumber] = sym->offset + sym->size;
		sym->dataType = typesym;
		sym->basicType = typesym->basicType;
		
		if (typesym->dataType != NULL && typesym->dataType->kind == SYM_ARRAY) {
			SYMBOL arr_type = typesym->dataType;
			while (arr_type && arr_type->kind == SYM_ARRAY) {
				arr_type = arr_type->dataType;
			}
			if (arr_type->kind == SYM_BASIC) {
				sym->basicType = arr_type->basicType;
			}
		}
		
		if (DEBUG & DB_INSTVARS) {
			printf(" ");
			dbugprinttok(idlist);
		}
		
		idlist = idlist->link;
	}
	
	if (DEBUG & DB_INSTVARS) {
		printf("\n");
	}
}

/* makearef makes an array reference operation.
 off is be an integer constant token
 tok (if not NULL) is a (now) unused token that is recycled. */
TOKEN makearef(TOKEN var, TOKEN off, TOKEN tok) {
	if (DEBUG & DB_MAKEAREF) {
		printf("(%d)\n", debug_call_num++);
		printf("In makearef(), from %s\n", last_method);
		dbugprint3args(var, off, tok);
		last_method = "makearef()";
	}
	
	TOKEN aref = makeop(OP_AREF);
	aref->operands = var;
	var->link = off;
	
	if (DEBUG & DB_MAKEAREF) {
		printf(" Finished makearef().\n");
		dbugprint1tok(aref);
		last_method = "makearef()";
	}
	
	return aref;
}

/* makefix forces the item tok to be integer, by truncating a constant
 or by inserting a OP_FIX operator */
TOKEN makefix(TOKEN tok) {
	
	if (DEBUG & DB_MAKEFIX) {
		printf("(%d)\n", debug_call_num++);
		printf("In makefix(), from %s\n", last_method);
		last_method = "makefix()";
		dbugprint1arg(tok);
		last_method = "makefix()";
	}
	
	TOKEN out = makeop(OP_FIX);   // create OP_FIX TOKEN
	if (!out) {
		printf(" Failed to alloc TOKEN, makefix().\n");
		return NULL;
	}
	
	out->operands = tok;
	out->link = NULL;
	
	if (DEBUG & DB_MAKEFIX) {
		printf(" Finished makefix().\n");
		dbugprint1tok(out);
		last_method = "makefix()";
	}
	
	return out;
}

/* makefloat forces the item tok to be floating, by floating a constant
 or by inserting a OP_FLOAT operator */
TOKEN makefloat(TOKEN tok) {
	
	if (DEBUG & DB_MAKEFLOAT) {
		printf("(%d)\n", debug_call_num++);
		printf("In makefloat(), from %s\n", last_method);
		dbugprint1arg(tok);
		last_method = "makefloat()";
	}
	
	TOKEN out;
	
	if (tok->tokenType == TOKEN_NUM) {
		/* e.g., floating 34 prints "3.400000e+01" to console */
		out = tok;
		out->dataType = TYPE_REAL;
		out->realval = out->intval;
		out->intval = INT_MIN;
	}
	else {
		/* e.g., floating 34 prints "(float 34)" to console */
		out = makeop(OP_FLOAT);
		if (!out) {
			printf(" Failed to alloc TOKEN, makefloat().\n");
			return NULL;
		}
		
		out->operands = tok;
		out->link = NULL;
	}
	
	if (DEBUG & DB_MAKEFLOAT) {
		printf(" Finished makefloat().\n");
		dbugprint1tok(out);
		last_method = "makefloat()";
	}
	
	return out;
}

/* makefor makes structures for a for statement. sign == 1 for regular
 for-loop, -1 for downto for-loop. tok, tokb and tokc are (now) unused
 tokens that are recycled. */
TOKEN makefor(int sign, TOKEN tok, TOKEN asg, TOKEN tokb,
TOKEN endexpr, TOKEN tokc, TOKEN statement) {
	
	if (DEBUG & DB_MAKEFOR) {
		printf("(%d)\n", debug_call_num++);
		printf("In makefor(), from %s\n", last_method);
		dbugprint6args(tok, asg, tokb, endexpr, tokc, statement);
		last_method = "makefor()";
	}
	
	TOKEN for_asg_progn_tok = makepnb(talloc(), asg);
	TOKEN label_tok = makelabel();
	
	TOKEN stop_op_tok = makeop(OP_LE);
	TOKEN do_progn_tok = makepnb(talloc(), statement);
	TOKEN ifop_tok = makeif(makeop(OP_IF), stop_op_tok, do_progn_tok, NULL);
	
	TOKEN loop_stop_tok = copytok(asg->operands); // the counter var, eg "i" in trivb.pas
	TOKEN stmt_incr_tok = makeloopincr(asg->operands, sign);
	TOKEN goto_tok;
	
	if (!for_asg_progn_tok || !label_tok || !ifop_tok || !stop_op_tok ||
	!do_progn_tok || !loop_stop_tok || !stmt_incr_tok) {
		printf(" Failed to alloc TOKEN(s), makefor().\n");
		return NULL;
	}
	
	goto_tok = makegoto(label_tok->operands->intval);  // TODO: not null-checked
	
	if (sign == -1) {
		stop_op_tok->whichval = OP_GE;   // "downto"
	}
	
	/* Link all the tokens together. */
	asg->link = label_tok;
	label_tok->link = ifop_tok;
	stop_op_tok->operands = loop_stop_tok;
	loop_stop_tok->link = endexpr;
	
	/* Handle elimination of nested progns */
	if (do_progn_tok->whichval != OP_PROGN) {
		do_progn_tok->operands = statement;
		statement->link = stmt_incr_tok;
	}
	else {   // do_progn_tok == statement
		get_last_link(do_progn_tok->operands)->link = stmt_incr_tok;
	}
	stmt_incr_tok->link = goto_tok;
	
	if (DEBUG & DB_MAKEFOR) {
		printf(" Finished makefor().\n");
		dbugprint1tok(for_asg_progn_tok);
		last_method = "makefor()";
	}
	
	return for_asg_progn_tok;
}

/* makefuncall makes a FUNCALL operator and links it to the fn and args.
 tok is a (now) unused token that is recycled. */
TOKEN makefuncall(TOKEN tok, TOKEN fn, TOKEN args) {
	
	if (DEBUG & DB_MAKEFUNCALL) {
		printf("(%d)\n", debug_call_num++);
		printf("In makefuncall(), from %s\n", last_method);
		dbugprint3args(tok, fn, args);
		last_method = "makefuncall()";
	}
	
	TOKEN funcall_tok;
	
	if (strcmp(fn->stringval, "new") != 0) {
		
		funcall_tok = makeop(OP_FUNCALL);      // 24
		if (!funcall_tok) {
			printf(" Failed to allocate TOKEN, makefuncall().\n");   // according to the Prof, print message and coredump
			return NULL;
		}
		
		SYMBOL this_fxn = searchst(fn->stringval);
		if (!this_fxn) {
			printf(" Failed to find function with name \"%s\" in symbol table.\n", fn->stringval);
			return NULL;
		}
		
		funcall_tok->dataType = this_fxn->dataType->basicType;
		
		/* Check for type compatibility between the write functions and
		 the arguments. Correct if necessary. */
		if (strcmp(fn->stringval, "write") == 0 || strcmp(fn->stringval, "writeln") == 0) {
			fn = write_fxn_args_type_check(fn, args);
			if (!fn) {
				return NULL;
			}
		}
		
		/* (+ i j) => +->operands = i and i->link = j; (funcall_tok fn args) */
		funcall_tok->operands = fn;
		fn->link = args;
	}
	else {
		/* For whenever new() is called */
		
		funcall_tok = makeop(OP_ASSIGN);
		TOKEN funcall = makeop(OP_FUNCALL);
		
		SYMBOL this_type = searchst(args->stringval);
		
		if (!this_type) {
			printf(" Error: type \"%s\" not found in symbol table, findtype().\n", args->stringval);
			return NULL;
		}
		
		funcall_tok->operands = args;
		args->link = funcall;
		funcall->operands = fn;
		fn->link = makeintc(this_type->dataType->dataType->dataType->size);
		
	}
	
	if (DEBUG & DB_MAKEFUNCALL) {
		printf(" Finished makefuncall().\n");
		dbugprint3toks(funcall_tok, funcall_tok->operands, funcall_tok->operands->link);
		//    ppexpr(funcall_tok);
		last_method = "makefuncall()";
	}
	
	return funcall_tok;
}

/* makegoto makes a GOTO operator to go to the specified label.
 The label number is put into a number token. */
TOKEN makegoto(int label) {
	
	if (DEBUG & DB_MAKEGOTO) {
		printf("(%d)\n", debug_call_num++);
		printf("In makegoto(), from %s\n", last_method);
		if (DB_PRINT_ARGS) {
			printf(" Arguments:\n  %d\n\n", label);
		}
		last_method = "makegoto()";
	}
	
	if (label < 0) {
		printf(" Warning: label number is negative (%d), makegoto().\n", label);
	}
	
	TOKEN goto_tok = makeop(OP_GOTO);
	TOKEN goto_num_tok = makeintc(label);
	if (!goto_tok || !goto_num_tok) {
		printf(" Failed to alloc TOKEN, makegoto().\n");
		return NULL;
	}
	
	goto_tok->operands = goto_num_tok;  // operand together
	
	if (DEBUG & DB_MAKEGOTO) {
		printf(" Finished makegoto().\n");
		dbugprint1tok(goto_tok);
		last_method = "makegoto()";
	}
	
	return goto_tok;
}

/* makeif makes an IF operator and links it to its arguments.
 tok is a (now) unused token that is recycled to become an OP_IF operator */
TOKEN makeif(TOKEN tok, TOKEN exp, TOKEN thenpart, TOKEN elsepart) {
	
	if (DEBUG & DB_MAKEIF) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeif(), from %s\n", last_method);
		dbugprint4args(tok, exp, thenpart, elsepart);
		last_method = "makeif()";
	}
	
	tok->tokenType = OPERATOR; /* Make it look like an operator. */
	tok->whichval = OP_IF;
	
	if (elsepart != NULL) {
		elsepart->link = NULL;
	}
	
	thenpart->link = elsepart;
	exp->link = thenpart;
	tok->operands = exp;
	
	if (DEBUG & DB_MAKEIF) {
		printf(" Finished makeif().\n");
		dbugprint1tok(tok);
	}
	
	return tok;
}

/* makeintc makes a new token with num as its value */
TOKEN makeintc(int num) {
	
	if (DEBUG & DB_MAKEINTC) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeintc(), from %s\n", last_method);
		printf(" Arguments:\n  %d\n\n", num);
		last_method = "makeintc()";
	}
	
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makeintc().\n");
		return NULL;
	}
	
	out->tokenType = TOKEN_NUM;
	out->dataType = TYPE_INT;
	out->intval = num;
	
	if (DEBUG & DB_MAKEINTC) {
		printf(" Finished makeintc().\n");
		dbugprint1tok(out);
	}
	
	return out;
}

/* makelabel makes a new label, using labelnumber++ */
TOKEN makelabel() {
	
	// DO NOT CALL FOR USER LABELS
	
	if (DEBUG & DB_MAKELABEL) {
		printf("(%d)\n", debug_call_num++);
		printf("In makelabel(), from %s\n\n", last_method);
		last_method = "makelabel()";
	}
	
	TOKEN label_tok = makeop(OP_LABEL);
	TOKEN label_num_tok = makeintc(labelnumber++);  // increment it after creation
	
	if (!label_tok || !label_num_tok) {
		printf(" Failed to alloc TOKEN(s), makelabel().\n");
		return NULL;
	}
	
	label_tok->operands = label_num_tok;   // operand together
	
	if (DEBUG & DB_MAKELABEL) {
		printf(" Finished makelabel().\n");
		dbugprint1tok(label_tok);
		last_method = "makelabel()";
	}
	
	return label_tok;
}

/* A helper method used to create the TOKENs required to increment
 the counter var in a for-loop.
 
 Returned TOKENs are of the form "(:= i (+ i 1))".  */
TOKEN makeloopincr(TOKEN var, int incr_amt) {
	// if in makefor(), send in asg->operands
	
	TOKEN assignop = makeop(OP_ASSIGN);
	TOKEN var_cpy = copytok(var);
	TOKEN plusop = makeplus(copytok(var), makeintc(incr_amt), NULL);  // OP_PLUS operand "var" link amt
	
	assignop->operands = var_cpy;
	var_cpy->link = plusop;
	
	return assignop;
}

/* makeop makes a new operator token with operator number opnum.
 Example:  makeop(OP_FLOAT)  */
TOKEN makeop(int opnum) {
	
	if (DEBUG & DB_MAKEOP) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeop(), from %s\n", last_method);
		if (DB_PRINT_ARGS) {
			printf(" Arguments:\n  %d\n\n", opnum);
		}
		last_method = "makeop()";
	}
	
	if (opnum < 0) {
		printf(" Warning: opnum is negative (%d), makeop().\n", opnum);
	}
	
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makeop().\n");
		return NULL;
	}
	
	out->tokenType = OPERATOR;
	out->whichval = opnum;
	
	if (DEBUG & DB_MAKEOP) {
		printf(" Finished makeop().\n");
		dbugprint1tok(out);
	}
	
	return out;
}

/* makeplus makes a + operator.
 tok (if not NULL) is a (now) unused token that is recycled. */
TOKEN makeplus(TOKEN lhs, TOKEN rhs, TOKEN tok) {
	
	if (DEBUG & DB_MAKEPLUS) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeplus(), from %s\n", last_method);
		dbugprint3args(lhs, rhs, tok);
		last_method = "makeplus()";
	}
	
	TOKEN out = makeop(OP_PLUS);
	if (lhs && rhs) {
		out->operands = lhs;
		lhs->link = rhs;
		rhs->link = NULL;
	}
	
	if (DEBUG & DB_MAKEPLUS) {
		printf(" Finished makeplus().\n");
		dbugprint1tok(out);
		last_method = "makeplus()";
	}
	
	return out;
}

/* Make a * operator TOKEN. Sets operands and
 fields if not NULL and returns this, otherwise
 just returns a * OPERATOR TOKEN. */
TOKEN maketimes(TOKEN lhs, TOKEN rhs, TOKEN tok) {
	
	TOKEN out = makeop(OP_MUL);
	if (lhs && rhs) {
		out->operands = lhs;
		lhs->link = rhs;
		rhs->link = NULL;
	}
	
	return out;
}

/* makepnb is like makeprogn, except that if statements is already a progn,
 it just returns statements.  This is optional. */
TOKEN makepnb(TOKEN tok, TOKEN statements) {
	
	if (statements->whichval == OP_PROGN && ELIM_NESTED_PROGN) {
		if (DEBUG & DB_MAKEPNB) {
			printf("(%d)\n", debug_call_num++);
			printf("In makepnb(), from %s\n", last_method);
			dbugprint2args(tok, statements);
			printf(" Finished makepnb().\n");
			dbugprint1tok(statements);
		}
		return statements;
	}
	if (DEBUG & DB_MAKEPNB) {
		printf("(%d)\n", debug_call_num++);
		printf("In makepnb(), from %s; transferring to makeprogn()...\n", last_method);
	}
	return (makeprogn(tok, statements));
}

/* makeprogn makes a PROGN operator and links it to the list of statements.
 tok is a (now) unused token that is recycled. */
TOKEN makeprogn(TOKEN tok, TOKEN statements) {
	
	if (DEBUG & DB_MAKEPROGN) {
		printf("(%d)\n", debug_call_num++);
		printf("In makeprogn(), from %s\n", last_method);
		dbugprint2args(tok, statements);
		last_method = "makeprogn()";
	}
	
	tok->tokenType = OPERATOR;    // 0
	tok->whichval = OP_PROGN;     // 22
	tok->operands = statements;
	
	if (DEBUG & DB_MAKEPROGN) {
		printf(" Finished makeprogn().\n");
		dbugprint2toks(tok, statements);
	}
	
	return tok;
}

/* Not really much use for this except
 in get_offset() */
TOKEN makerealtok(float num) {
	TOKEN out = talloc();
	if (!out) {
		printf(" Failed to alloc TOKEN, makerealtok().\n");
		return NULL;
	}
	
	out->tokenType = TOKEN_NUM;
	out->dataType = TYPE_REAL;
	out->realval = num;
	
	return out;
}

/* makerepeat makes structures for a repeat statement.
 tok and tokb are (now) unused tokens that are recycled. */
TOKEN makerepeat(TOKEN tok, TOKEN statements, TOKEN tokb, TOKEN expr) {
	
	if (DEBUG & DB_MAKEREPEAT) {
		printf("(%d)\n", debug_call_num++);
		printf("In makerepeat(), from %s\n", last_method);
		dbugprint4args(tok, statements, tokb, expr);
		last_method = "makerepeat()";
	}
	
	TOKEN label_tok = makelabel();
	TOKEN goto_tok = makegoto(label_tok->operands->intval);
	TOKEN rpt_progn_tok = makepnb(talloc(), label_tok);   // operand label_tok to rpt_progn_tok
	TOKEN do_progn_tok = makeop(OP_PROGN);
	TOKEN ifop_tok = makeif(makeop(OP_IF), expr, do_progn_tok, NULL);
	
	if (!label_tok || !goto_tok || !rpt_progn_tok ||
	!do_progn_tok ||!ifop_tok) {
		printf(" Failed to alloc TOKEN(s), makerepeat().\n");
		return NULL;
	}
	
	/* Link TOKENs together */
	label_tok->link = statements;
	do_progn_tok->link = goto_tok;
	
	/* Handle elimination of nested progns */
	get_last_link(statements)->link = ifop_tok;
	
	if (DEBUG & DB_MAKEREPEAT) {
		printf(" Finished makerepeat().\n");
		dbugprint1tok(rpt_progn_tok);
		last_method = "makerepeat()";
	}
	
	return rpt_progn_tok;
}

/* makewhile makes structures for a while statement.
 tok and tokb are (now) unused tokens that are recycled. */
TOKEN makewhile(TOKEN tok, TOKEN expr, TOKEN tokb, TOKEN statement) {
	
	if (DEBUG & DB_MAKEWHILE) {
		printf("(%d)\n", debug_call_num++);
		printf("In makewhile(), from %s\n", last_method);
		dbugprint4args(tok, expr, tokb, statement);
		last_method = "makewhile()";
	}
	
	TOKEN label_tok = makelabel();
	TOKEN goto_tok = makegoto(label_tok->operands->intval);
	TOKEN while_progn_tok = makepnb(talloc(), label_tok); // operand label_tok to while_progn_tok
	TOKEN do_progn_tok = makepnb(talloc(), statement);    // operand statement to do_progn_tok
	TOKEN ifop_tok = makeif(makeop(OP_IF), expr, do_progn_tok, NULL);
	
	if (!label_tok || !goto_tok || !while_progn_tok ||
	!do_progn_tok ||!ifop_tok) {
		printf(" Failed to alloc TOKEN(s), makewhile().\n");
		return NULL;
	}
	
	label_tok->link = ifop_tok;
	
	/* Handle elimination of nested progns */
	if (do_progn_tok->whichval != OP_PROGN) {
		do_progn_tok->operands = statement;
		statement->link = goto_tok;
	}
	else {   // do_progn_tok == statement
		get_last_link(do_progn_tok->operands)->link = goto_tok;
	}
	
	if (DEBUG & DB_MAKEWHILE) {
		printf(" Finished makewhile().\n");
		dbugprint1tok(while_progn_tok);
		last_method = "makewhile()";
	}
	
	return while_progn_tok;
}

/* mulint multiplies expression exp by integer n */
TOKEN mulint(TOKEN exp, int n) {
	if (!exp) {
		return NULL;
	}
	
	if (exp->dataType == TYPE_INT) {
		
		int a = exp->intval;
		int val = a * n;
		
		if (DEBUG & DB_MULINT) {
			printf("(%d)\n", debug_call_num++);
			printf("In mulint(), from %s\n Multiplying %d to %d\n", last_method, n, a);
			last_method = "mulint()";
		}
		
		if (a != 0 && (a * n) / a != n) {
			printf(" Integer overflow detected, mulint()\n");
			printf(" Cannot multiply %d to %d\n", n, a);
		}
		else {
			exp->intval = val;
		}
		
		if (DEBUG & DB_MULINT) {
			printf(" New value of exp: %d\n", exp->intval);
			dbugprint1tok(exp);
		}
		
	}
	else if (exp->dataType == TYPE_REAL) {
		
		if (DEBUG & DB_MULINT) {
			printf("In mulint()\n Multiplying %d to %f\n", n, exp->realval);
		}
		
		double val = exp->realval * n;
		
		if ((val > FLT_MAX || val < FLT_MIN)) {
			printf(" Floating number out of range, mulint()\n");
			printf(" Cannot multiply %d to %f\n", n, exp->realval);
		}
		else {
			exp->realval = val;
		}
		
		if (DEBUG & DB_MULINT) {
			printf(" New value of exp: %f\n", exp->realval);
			dbugprint1tok(exp);
		}
		
	}
	
	return exp;
}

/* searchins will search for symbol, inserting it if not present. */
SYMBOL searchinsst(char name[]) {
	return (searchins(name));
}

/* settoktype sets up the type fields of token tok.
 typ = type pointer, ent = symbol table entry of the variable  */
void settoktype(TOKEN tok, SYMBOL typ, SYMBOL ent) {
	
	if (DEBUG & DB_SETTOKTYPE) {
		printf("(%d)\n", debug_call_num++);
		printf("In settoktype(), from %s\n", last_method);
		dbugprint1arg(tok);
		dbugprint2syms(typ, ent);
		last_method = "settoktype()";
	}
	
	if (!tok || !typ || !ent) {
		// something;
	}
	
	tok->symType = typ;
	tok->symEntry = ent;
	
	if (DEBUG & DB_SETTOKTYPE) {
		printf(" Finished settoktype().\n\n");
	}
}

/* unaryop links a unary operator op to one operand, lhs. +/-/NOT */
TOKEN unaryop(TOKEN op, TOKEN lhs) {
	
	if (DEBUG & DB_UNARYOP) {
		printf("(%d)\n", debug_call_num++);
		printf("In unaryop(), from %s\n", last_method);
		dbugprint2args(op, lhs);
		last_method = "unaryop()";
	}
	
	op->operands = lhs;
	lhs->link = NULL;
	
	if (DEBUG & DB_UNARYOP) {
		printf(" Finished unaryop().\n");
		dbugprint2toks(op, lhs);
	}
	
	return op;
}

/* wordaddress pads the offset n to be a multiple of wordsize.
 wordsize should be 4 for integer, 8 for real and pointers,
 16 for records and arrays */
int wordaddress(int n, int wordsize) {
	return ((n + wordsize - 1) / wordsize) * wordsize;
}

// TODO: assumes fn is always a write() or writeln() function
// TODO: does not do cross-conversions (e.g., send in writelnf() to convert to writelni())
// TODO: identifier dataTypes?
/* Check for type compatibility between the write functions and
 the arguments. Correct if necessary. */
TOKEN write_fxn_args_type_check(TOKEN fn, TOKEN args) {
	
	if (args->dataType == TYPE_STR) {
		return fn;
	}
	
	TOKEN out = NULL;
	
	SYMBOL fn_sym = searchst(fn->stringval);
	if (!fn_sym) {
		printf(" Error: function \"%s\" is not defined.\n", fn->stringval);
		return out;
	}
	
	int fn_arg_type = fn_sym->dataType->link->basicType;
	int args_type = args->dataType;
	
	if (args_type == TYPE_STR) {
		out = fn;
	}
	else {
		
		int replace_index = 5;
		if (strcmp(fn->stringval, "writeln") == 0) {
			replace_index = 7;
		}
		
		if (strcmp(fn->stringval, "write") == 0) {
			
			if (args_type == TYPE_INT) {
				fn->stringval[replace_index] = 'i';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			else if (args_type == TYPE_REAL) {
				fn->stringval[replace_index] = 'f';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			
		}
		else if (strcmp(fn->stringval, "writeln") == 0) {
			
			if (args_type == TYPE_INT) {
				fn->stringval[replace_index] = 'i';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			else if (args_type == TYPE_REAL) {
				fn->stringval[replace_index] = 'f';
				fn->stringval[replace_index + 1] = '\0';
				out = fn;
			}
			
		}
	}
	
	return out;
}



int yyerror(s) char *s; {
	fprintf(stderr, "Parse Error at line %d: %s\n", lineCnt, s);
	return 0;
}

int main() {
	int res;
	initsyms();
	res = yyparse();
	//printst();
	printf("yyparse result = %8d\n", res);
	if (DEBUG & DB_PARSERES) dbugprinttok(parseresult);
	ppexpr(parseresult);           /* Pretty-print the result tree */
	return 0;
}
