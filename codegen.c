/**********************************************************
 *	File:		codegen.c
 *	Project:	SPL-compiler
 *	Author:		Execution
 *	Modified:	Jun 8, 2017
 **********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "symtab.h"
#include "genasm.h"
#include "codegen.h"
#include "pprint.h"

void genc(TOKEN code);

/* Set DEBUGGEN to 1 for debug printouts of code generation */
#define DEBUGGEN 0

int nextlabel;    /* Next available label number */
int stkframesize;   /* total stack frame size */

TOKEN inline_funcall = NULL;        /* to handle function calls in the RHS of an ASSIGNOP statement */
int num_funcalls_in_curr_tree;
int num_inlines_processed;
int saved_inline_reg;
int saved_inline_regs[10];

int saved_label_num = -1;           /* saves the label number of the top-level label; used to handle repeat statements */

boolean new_funcall_flag = false;   /* if a FUNCALLOP is for new(), set to true */

double saved_float_reg = -DBL_MAX;  /* in an ASSIGNOP, holds the value of the float being assigned */
int saved_float_reg_num = -1;       /* the register where the float in saved_float_reg is stored */

boolean nil_flag = false;           /* if assigning a NUMBERTOK with whichval == 0 to a ptr, set to true */

TOKEN saved_rhs_reg = NULL;         /* in an ASSIGNOP, save the TOKEN representing the RHS (value to be assigned) */
int saved_rhs_reg_num = -1;         /* in an ASSIGNOP, save the register location of saved_rhs_reg */

boolean nested_refs = false;        /* used in POINTEROP in genop(). probably can be deprecated but have not checked */
TOKEN first_op_genarith = NULL;     /* used to signal if multiple pointer dereferences are made in a single statement */
TOKEN nested_ref_stop_at = NULL;    /* in an AREFOP in genop(), and if multiple ptr dereferences are made in a single statement,
                                       signals the second-to-last pointer dereference */

TOKEN last_ptr = NULL;              /* the last pointer to be referenced in any single statement */
int last_ptr_reg_num;               /* the register number last_ptr */
int last_ptr_deref_offs;            /* the offset of the field being dereferenced by the statement in which last_ptr is set */

int last_id_reg_num;                /* the register number location of the last identifier referenced in a statement (EXCEPT pointers) */

                     /* integer regs */
int used_regs[32] = { 0, 0, 0, 0, 0, 0, 0, 0,
                     /* FP regs; r8d to r15d */
                      0, 0, 0, 0, 0, 0, 0, 0,
                     /* xmm0 to xmm15 */
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                    };
int arg_reg[4] = {EDI, ESI, EDX, ECX};

char* funtopcode[] = {
  "	pushq	%rbp			# save base pointer on stack",
  "	movq	%rsp, %rbp		# move stack pointer to base pointer",
  "",
};
char* funbotcode[] = {
	"	leave",
	"	ret",
	"",
};

#define NUM_INT_REGS    8
#define NUM_FP_REGS     24
#define NUM_REGS        32

void print_used_regs();
int symbol_is_null_int(char *str);

/* Top-level entry for code generator.
   pcode    = pointer to code:  (program foo (output) (progn ...))
   varsize  = size of local storage in bytes
   maxlabel = maximum label number used so far

Add this line to the end of your main program:
    gencode(parseresult, blockoffs[blocknumber], labelnumber);
The generated code is printed out; use a text editor to extract it for
your .s file.
         */

void gencode(TOKEN pcode, int varsize, int maxlabel){
	TOKEN name, fund, body; // program name, function declear, program body
	name = pcode->operands;
	nextlabel = maxlabel + 1;
	fund = name->link;

	stkframesize = asmentry(name->stringval,varsize);
	asmjump(JMP, 0);	// jump to L0 (start)
	while (fund->whichval == OP_FUNDCL) {
		asmlabelstr(fund->operands->operands->link->stringval);
		genc(fund);
		fund = fund->link;
	}
	body = fund;
	asmlabel(0);		// label L0 (start)
	blocknumber = 1;

	genc(body);

	asmexit(name->stringval);
}

/* Trivial version: always returns RBASE + 0 */
/* Get a register.   */
/* Need a type parameter or two versions for INTEGER or REAL */
int getreg(int kind) {

	/*     ***** NAIVE IMPLEMENTATION *****   */

	int i = 0;
	int stop = NUM_INT_REGS;
	if (kind !=  TYPE_INT) {
		//        i = 8;
		i = 16;
		stop = NUM_REGS;
	}

	for (; i < stop; i++) {
		if (used_regs[i] == 0) {
			used_regs[i] = 1;
			return i;
		}
	}

	return RBASE;
}

/* Trivial version */
/* Generate code for arithmetic expression, return a register number */
int genarith(TOKEN code) {

    int num, reg_num, lhs_reg, rhs_reg;
    SYMBOL sym;

	if (DEBUGGEN) {
		printf("genarith\n");
		dbugprinttok(code);
	}
	switch ( code->tokenType ) {
		case TOKEN_NUM:
			switch (code->dataType) {
				case TYPE_INT: {
					num = code->intval;
					reg_num = getreg(TYPE_INT);
					if (num >= MINIMMEDIATE && num <= MAXIMMEDIATE && !nested_refs) {
						if (last_ptr && last_ptr_reg_num > -1) {

						// asmimmed(MOVQ, num, last_ptr_reg_num);
							asmimmed(MOVQ, num, reg_num);
							last_ptr_reg_num = -1;
						}
						else if (!nil_flag) {
							asmimmed(MOVL, num, reg_num);
						}
						else {
							asmimmed(MOVQ, num, reg_num);
						}

					}
				}
				break;
				case TYPE_REAL: {
					/* Generate literal for the value of the constant, then
					load the literal into a register. */

					reg_num = getreg(TYPE_REAL);
					saved_float_reg = code->realval;
					saved_float_reg_num = reg_num;

					makeflit(code->realval, nextlabel);
					asmldflit(MOVSD, nextlabel++, reg_num);
				}
				break;
			}
			break;
		case TOKEN_ID: {
			sym = searchst(code->stringval);
			if (!sym) {
				return symbol_is_null_int(code->stringval);
			}

			if (DEBUGGEN) {
				printf(" TOKEN_ID detected; symbol table entry:\n");
				ppexpr(code);
				dbprsymbol(sym);
				printf(" offset: %d\n", sym->offset);
				printf("\n");
			}

			num = sym->offset;

			if (sym->kind == SYM_FUNCTION) {
				reg_num = getreg(sym->dataType->basicType);
				inline_funcall = code;
				//genc(code->link);
			}
			else {

				reg_num = getreg(code->dataType);
				// TYPE_INT
				if (reg_num < NUM_INT_REGS) {

					SYMBOL temp = searchst(code->stringval);
					if (!temp) {
						return symbol_is_null_int(code->stringval);
					}

					SYMBOL next = temp->dataType;

					if (!next) {
						return symbol_is_null_int(NULL);
					}

					if (next->kind != SYM_ARRAY) {
						last_id_reg_num = reg_num;
						asmld(MOVL, sym->offset - stkframesize, reg_num, code->stringval);
					}

				}
				// TYPE_REAL
				else {

					last_id_reg_num = reg_num;
					asmld(MOVSD, sym->offset - stkframesize, reg_num, code->stringval);
				}
				
			}
		}
		break;
		case OPERATOR: {
			if (first_op_genarith == NULL) {
				first_op_genarith = code;
			}
			else {
				nested_refs = true;
			}

			lhs_reg = genarith(code->operands);

			if (code->operands->link) {
				if (code->whichval == OP_FUNCALL) {
					TOKEN arglist = code->operands->link;
					int temp_reg, index = 0;
					while (arglist) {
						temp_reg = genarith(arglist);
						asmrr(MOVL, temp_reg, arg_reg[index]); // score values into arg reg
						mark_reg_used(arg_reg[index++]);
						free_reg(temp_reg);
						arglist = arglist->link;
					}
				}
				else rhs_reg = genarith(code->operands->link);

			}
			else {
				// FLOATOP (and possibly FIXOP?)
				rhs_reg = 0;
			}

			if (code->operands->whichval == OP_FUNCALL) {

				free_reg(lhs_reg);
				lhs_reg = saved_inline_regs[num_inlines_processed - 1];
				mark_reg_used(lhs_reg);
			}
			if (code->operands->link) {
				if (code->operands->link->whichval == OP_FUNCALL) {

					free_reg(rhs_reg);
					rhs_reg = saved_inline_regs[num_inlines_processed - 2];
					mark_reg_used(rhs_reg);
				}
			}

			boolean same_reg_assn = false;
			if (lhs_reg == rhs_reg) {
				same_reg_assn = true;
				if (rhs_reg > 15) {
					lhs_reg = getreg(TYPE_REAL);
				}
				else {
					lhs_reg = getreg(TYPE_INT);
				}
			}

			/************* key code ***************/
			if (code->whichval == OP_FUNCALL) saved_inline_reg = EAX;
			lhs_reg = genop(code, rhs_reg, lhs_reg);

			if (code->whichval == OP_FUNCALL) {
				free_reg(arg_reg[0]);
				free_reg(arg_reg[1]);
				free_reg(arg_reg[2]);
				free_reg(arg_reg[3]);
			}
			else free_reg(rhs_reg);

			if (same_reg_assn) {
				int temp;
				if (lhs_reg > 15) {
					free_reg(lhs_reg);
					temp = getreg(TYPE_REAL);
				}
				else {
					free_reg(lhs_reg);
					temp = getreg(TYPE_INT);
				}
				lhs_reg = temp;
			}

			if (code->whichval == OP_FUNCALL) {
				reg_num = EAX;
				mark_reg_used(reg_num);
			}
			else reg_num = lhs_reg;

		}
		break;
		default:
			return symbol_is_null_int(NULL);
		break;
	}

	first_op_genarith = NULL;
	
	return reg_num;
}


int genop(TOKEN code, int rhs_reg, int lhs_reg) {

    if (DEBUGGEN) {
        printf(" OPERATOR detected, from genarith().\n");
        //printf(" %s\n", opprint[which_val]);
        printf(" %d %d %d\n", code->whichval, rhs_reg, lhs_reg);
    }

    int out = 0;
    int which_val = code->whichval;

    if (which_val == OP_PLUS) {
        if (at_least_one_float(lhs_reg, rhs_reg)) {
            asmrr(ADDSD, rhs_reg, lhs_reg);
        }
        else {
            asmrr(ADDL, rhs_reg, lhs_reg);
        }
        out = lhs_reg;
    }
    else if (which_val == OP_MINUS) {
        if (lhs_reg > 15 && lhs_reg < NUM_REGS) {
            asmfneg(lhs_reg, getreg(TYPE_REAL));
            rhs_reg = lhs_reg;
        }
        else if (at_least_one_float(lhs_reg, rhs_reg)) {
            asmrr(SUBSD, rhs_reg, lhs_reg);
        }
        else {
            asmrr(SUBL, rhs_reg, lhs_reg);
        }
        out = lhs_reg;
    }
    else if (which_val == OP_MUL) {
        if (at_least_one_float(lhs_reg, rhs_reg)) {
            asmrr(MULSD, rhs_reg, lhs_reg);
        }
        else {
            asmrr(IMULL, rhs_reg, lhs_reg);
        }
        out = lhs_reg;
    }
    else if (which_val == OP_DIV_R) {
        if (at_least_one_float(lhs_reg, rhs_reg)) {
            asmrr(DIVSD, rhs_reg, lhs_reg);
        }
        else {
            asmrr(DIVL, rhs_reg, lhs_reg);
        }
        out = lhs_reg;
    }
    else if (which_val == OP_EQ) {
        out = nextlabel++;
        asmrr(CMPL, rhs_reg, lhs_reg);
        asmjump(JE, out);
    }
    else if (which_val == OP_NE) {
        out = nextlabel++;
        asmrr(CMPQ, rhs_reg, lhs_reg);
        asmjump(JNE, out);
    }
    else if (which_val == OP_LT) {
        out = nextlabel++;
        asmrr(CMPL, rhs_reg, lhs_reg);
        asmjump(JL, out);
    }
    else if (which_val == OP_LE) {
        out = nextlabel++;
        asmrr(CMPL, rhs_reg, lhs_reg);
        asmjump(JLE, out);
    }
    else if (which_val == OP_GE) {
        out = nextlabel++;
        asmrr(CMPL, rhs_reg, lhs_reg);
        asmjump(JGE, out);
    }
    else if (which_val == OP_GT) {
        out = nextlabel++;
        asmrr(CMPL, rhs_reg, lhs_reg);
        asmjump(JG, out);
    }
    else if (which_val == OP_FUNCALL) {

        if (inline_funcall) {

            if (num_funcalls_in_curr_tree > 1) {
                saved_inline_regs[num_inlines_processed] = saved_inline_reg;
                num_inlines_processed++;
                if (num_inlines_processed == 1) {
                    asmcall(inline_funcall->stringval);
                    asmsttemp(saved_inline_reg);
                }
                else if (num_inlines_processed > 0 && num_inlines_processed < num_funcalls_in_curr_tree) {
                    // load and then store?
                }
                else {
                    asmcall(inline_funcall->stringval);
                    asmldtemp(saved_inline_reg);
                }
            }
            else {
                asmcall(inline_funcall->stringval);
            }

            inline_funcall = NULL;
        }
        else {
            // ?????????????????????????????
        }

        out = lhs_reg;
    }
    else if (which_val == OP_AREF) {

        if (saved_float_reg != -DBL_MAX) {
            asmldr(MOVQ, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
        }
        else {
            if (last_id_reg_num > -1) {
                int temp = rhs_reg;
                if (last_id_reg_num > -1 && last_id_reg_num < 16) {
                    if (last_id_reg_num == rhs_reg) {
                        rhs_reg = getreg(TYPE_INT);
                        free_reg(temp);
                    }

                    if (last_ptr && last_ptr_reg_num > -1) {

                        boolean found = false;
                        SYMBOL temp0, temp1, temp2, temp3, temp4, temp5, typsym;
                        temp0 = searchst(last_ptr->stringval);
                        typsym = NULL;

                        if (!temp0) {
                            return symbol_is_null_int(code->stringval);
                        }

                        temp1 = searchst(temp0->link->nameString);

                        if (!temp1) {
                            return symbol_is_null_int(code->stringval);
                        }

                        if (temp1->dataType->kind == SYM_ARRAY) {
                            typsym = temp1->dataType;
                            while (typsym && typsym->kind == SYM_ARRAY) {
                                typsym = typsym->dataType;
                            }

                            if (!typsym) {
                                return symbol_is_null_int(code->stringval);
                            }

                            temp2 = typsym->dataType;
                            if (temp2 && temp2->kind == SYM_RECORD) {
                                temp3 = temp2->dataType;

                                while (temp3 && !found) {
                                    if (temp3->offset == last_ptr_deref_offs) {
                                        found = true;

                                        if (temp3->size > basicsizes[TYPE_INT]) {
                                            asmldr(MOVQ, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
                                        }
                                        else {
                                            asmldr(MOVL, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
                                        }
                                    }
                                    temp3 = temp3->link;
                                }
                            }

                        }
                        else {

                        }

                        // probably broken
                        if (!found) {
                            asmldr(MOVL, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
                        }

                        last_ptr_reg_num = -1;
                    }
                    else {
                        asmldr(MOVL, code->operands->link->intval, lhs_reg, rhs_reg, "^.");                        
                    }

                }
                else {
                    if (last_id_reg_num == rhs_reg) {
                        rhs_reg = getreg(TYPE_REAL);
                        free_reg(temp);
                    }
                    asmldr(MOVSD, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
                }
                // else // WHAT ABOUT IF LHS IS > 15 ???????????????????????????????????????????????????
            }
            else {
                free_reg(rhs_reg);
                rhs_reg = getreg(TYPE_REAL);
                asmldr(MOVSD, code->operands->link->intval, lhs_reg, rhs_reg, "^.");
            }

        }

        last_ptr_reg_num = rhs_reg;
        
        out = rhs_reg;

    }
    else if (which_val == OP_FLOAT) {
        int freg = getreg(TYPE_REAL);
        asmfloat(rhs_reg, freg);
        free_reg(lhs_reg);
        free_reg(rhs_reg);
        out = freg;
    }
    else if (which_val == OP_FIX) {
        int dreg = getreg(TYPE_INT);
        asmfix(lhs_reg, dreg);
        free_reg(lhs_reg);
        free_reg(rhs_reg);
        out = dreg;
    }

    if (inline_funcall != NULL && num_funcalls_in_tree > 0) {
        saved_inline_reg = rhs_reg;
    }

    return out;
}


/* Generate code for a Statement from an intermediate-code form */
void genc(TOKEN code){
	TOKEN tok, lhs, rhs;
	int reg_num, offs;
	SYMBOL sym;

	if (DEBUGGEN) {
		printf("genc\n");
		dbugprinttok(code);
	}
	if ( code->tokenType != OPERATOR ) {
		if (code->tokenType == TOKEN_NUM && code->dataType == TYPE_INT && new_funcall_flag) {
            reset_regs();
            new_funcall_flag = false;
            return;
        }

		printf("Bad code token");
		dbugprinttok(code);
	}

    reset_regs();
	
	switch ( code->whichval ) {
		case OP_PROGN: {

			if (DEBUGGEN) {
				printf(" PROGNOP detected.\n");
				ppexpr(code->operands);
				printf("\n");
			}
			last_ptr = NULL;
			last_ptr_reg_num = -1;
			last_ptr_deref_offs = -1;
			nested_ref_stop_at = NULL;

			int i;
			for (i = 0; i < 10; i++) {
				saved_inline_regs[i] = -1;
			}
			num_inlines_processed = 0;
			last_id_reg_num = -1;

			tok = code->operands;
			while (tok) {

				num_funcalls_in_curr_tree = num_funcalls_in_tree(tok->operands, 0);
				saved_inline_reg = 0;       // may need to set to -1 as sentinel value

				if (tok->whichval == OP_LABEL) {
					saved_label_num = tok->operands->intval;
				}
				if (search_tree_str(tok, "new")) {
					new_funcall_flag = true;
				}

				genc(tok);
				tok = tok->link;
			}
		}
		break;
		
		case OP_ASSIGN: {
			if (DEBUGGEN) {
				printf("\n ASSIGNOP detected.\n");
				ppexpr(code);
				printf("#####\n");
				ppexpr(code->operands);
				printf("\n");
			}

			TOKEN last_operand = get_last_operand(code);
			TOKEN outer_link = code->operands->link;

			lhs = code->operands;
			rhs = lhs->link;

			if (code->operands->operands != NULL) {
				nested_ref_stop_at = code->operands->operands;
			}

			reg_num = genarith(rhs);                        /* generate rhs into a register */
			saved_rhs_reg = rhs;
			saved_rhs_reg_num = reg_num;

			sym = searchst(lhs->stringval);

			int datatype = code->dataType;                  /* store value into lhs  */

			// simple var
			if (sym) {
				offs = sym->offset - stkframesize;          /* net offset of the var   */

				switch (code->dataType) {
					case TYPE_INT:
					asmst(MOVL, reg_num, offs, lhs->stringval);
					break;

					case TYPE_REAL:
					asmst(MOVSD, reg_num, offs, lhs->stringval);
					break;

					default:

					break;
				}
			}
			// array or record
			else {
				sym = searchst(lhs->operands->stringval);

				// array or record
				if (sym) {
					offs = sym->offset - stkframesize;

					TOKEN last_link = get_last_link(lhs->operands);

					if (last_link) {
						if (last_link->tokenType == TOKEN_NUM && last_link->dataType == TYPE_INT) {

							asmimmed(MOVL, last_link->intval, EAX);
							asmop(CLTQ);

							if (reg_num >= 0 && reg_num < 16) {
								asmstrr(MOVL, reg_num, offs, EAX, sym->nameString);
							}
							else {
								asmstrr(MOVSD, reg_num, offs, EAX, sym->nameString);
							}

						}
						else if (last_link->tokenType == OPERATOR && last_link->whichval == OP_PLUS) {
							TOKEN plus_operand = last_link->operands;
							TOKEN mul_operand = plus_operand->link->operands;
							TOKEN last_operand = mul_operand->link;

							mark_reg_unused(EAX);
							mark_reg_used(last_ptr_reg_num);

							int move_plus_to = getreg(TYPE_INT);
							int move_mul_to = getreg(TYPE_INT);
							int move_last_to;

							asmimmed(MOVL, plus_operand->intval, move_plus_to);
							asmimmed(MOVL, mul_operand->intval, move_mul_to);

							if (last_operand->tokenType == TOKEN_NUM) {
								move_last_to = getreg(TYPE_INT);
								asmimmed(MOVL, last_operand->intval, move_last_to);
							}
							else {
								sym = searchst(last_operand->stringval);
								offs = sym->offset - stkframesize;          // NOT NULL CHECKED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								asmld(MOVL, offs, EBX, sym->nameString);    // CONTENTS AREN'T ACTUALLY IN CALLEE-SAVE REG EBX (SEE last_ptr_reg_num)
								asmrr(IMULL, EBX, move_mul_to);
								asmrr(ADDL, move_mul_to, move_plus_to);
								asmop(CLTQ);

								sym = searchst(lhs->operands->stringval);
								offs = sym->offset - stkframesize;          // NOT NULL CHECKED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								asmstrr(MOVL, last_ptr_reg_num, offs, 0, lhs->operands->stringval);
							}
						}
						else {
						// ?????????????????????????????????????????????????????????????????????????????
						}
					}
					else {
						if (reg_num >= 0 && reg_num < 16) {
							asmstrr(MOVL, reg_num, offs, getreg(TYPE_INT), sym->nameString);
						}
						else {
							asmstrr(MOVSD, reg_num, offs, getreg(TYPE_INT), sym->nameString);
						}
					}
				}
				/*
				else {	
					sym = searchst(lhs->operands->operands->stringval);
					if (sym) {

						offs = sym->offset - stkframesize;
						int temp = getreg(TYPE_INT);

						last_ptr = lhs->operands->operands; // ??????????????????????????????????????????????????????????????????

						asmld(MOVQ, offs, temp, sym->nameString);
						offs = lhs->operands->link->intval;

						if (basicsizes[rhs->dataType] > basicsizes[TYPE_INT]) {
							// see third line in L1 for pasrec
							if (saved_float_reg != -DBL_MAX) {
								asmstr(MOVSD, reg_num, offs, temp, "^. ");
								// saved_float_reg = -DBL_MAX;
							}
							else {
								asmstr(MOVQ, reg_num, offs, temp, "^. ");
							}
						}
						else {
							if (!nil_flag) {     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								asmstr(MOVL, reg_num, offs, temp, "^. ");
							}
							else {
								asmstr(MOVQ, reg_num, offs, temp, "^. ");
							// nil_flag = false;
							}
						}

					}
				}*/
			} // end of [array or record]
			if (lhs->operands) {
				reg_num = genarith(lhs->operands);
			}

			nil_flag = false;
			saved_float_reg = -DBL_MAX;
			saved_rhs_reg = NULL;
			saved_rhs_reg_num = -1;
			nested_refs = false;    

			last_ptr = NULL;
			last_ptr_reg_num = -1;

			nested_ref_stop_at = NULL;
		} // end of case OP_ASSIGN
		break;
		case OP_GOTO: {
			/* Generate a branch to the label number (see genasm.c). */

			if (DEBUGGEN) {
				printf(" GOTOOP detected.\n");
				ppexpr(code->operands);
				printf("\n");
			}

			lhs = code->operands;
			asmjump(JMP, lhs->intval);
		}
		break;
		case OP_LABEL: {
			/* Generate a label with the label number (see genasm.c). */

			if (DEBUGGEN) {
				printf(" LABELOP detected.\n");
				ppexpr(code->operands);
				printf("\n");
			}

			lhs = code->operands;
			asmlabel(lhs->intval);
		}
		break;
		case OP_IF: {
			if (DEBUGGEN) {
				printf(" IFOP detected.\n");
				ppexpr(code->operands);
				printf("\n");
			}

			lhs = code->operands;
			rhs = code->operands->link;
			int if_label_num = genarith(lhs);

			int else_label_num, endif_label_num;
			if (rhs->link) else_label_num = nextlabel++;
			endif_label_num = nextlabel++;

			// have else
			if (rhs->link) {
				asmjump(JMP, else_label_num);	// false -> else_label

				asmlabel(if_label_num);			// if_label
				genc(rhs);
				asmjump(JMP, endif_label_num);	// jump -> endif

				asmlabel(else_label_num);		// else_label
				genc(rhs->link);
				asmlabel(endif_label_num);		// endif_label
			}
			// no else
			else {
				asmjump(JMP, endif_label_num);	// false -> endif_label

				asmlabel(if_label_num);			// if_label
				genc(rhs);
				//asmjump(JMP, endif_label_num);	// jump -> endif, not necessary

				asmlabel(endif_label_num);		// endif_label
			}
			
		}
		break;
		// procedures. functions will be generate by OP_ASSIGN / getarith
		case OP_FUNCALL: {

			if (DEBUGGEN) {
				printf(" FUNCALLOP detected.\n");
				ppexpr(code);
				ppexpr(code->operands);
				ppexpr(code->operands->link);
				printf("\n");
			}

			lhs = code->operands;
			rhs = code->operands->link;
			SYMBOL argsym;

			if (strstr(lhs->stringval, "write")) {      // write/writeln
				sym = searchst(lhs->stringval);

				if (rhs->tokenType == TOKEN_STR) {
					asmlitarg(nextlabel, EDI);
					asmcall(lhs->stringval);
					makeblit(rhs->stringval, nextlabel++);
				}
				else if (rhs->tokenType == TOKEN_CHAR) {
					char s[2];
					s[0] = rhs->charval;
					s[1] = '\0';
					asmlitarg(nextlabel, EDI);
					asmcall(lhs->stringval);
					makeblit(rhs->stringval, nextlabel++);
				}
				else if (rhs->tokenType == OPERATOR) {
					if (rhs->whichval == OP_AREF) {

						sym = searchst(rhs->operands->stringval);
						if (!sym) {
							sym = searchst(rhs->operands->operands->stringval);
							if (sym) {
								reg_num = getreg(TYPE_INT);
								offs = sym->offset - stkframesize;
								asmld(MOVQ, offs, reg_num, sym->nameString);

								offs = rhs->operands->link->intval;
								int temp = getreg(TYPE_REAL);
								asmldr(MOVSD, offs, reg_num, temp, "^.");

								asmcall(lhs->stringval);

							}
						}
					}
				}

				else if (sym != NULL && (sym->dataType->basicType == TYPE_INT ||
					sym->dataType->basicType == TYPE_REAL)) {

					SYMBOL argsym;

					if (rhs->tokenType == TOKEN_NUM) {
						printf("\nTOKEN_NUM UNFINISHED\n");
					}             

					else if (rhs->tokenType == TOKEN_ID) {
						argsym = searchst(rhs->stringval);
						if (!argsym) {
							printf("Error: no symbol table entry for var \"%s\"", rhs->stringval);
							return;
						}

						if (argsym->basicType == TYPE_INT) {
							reg_num = getreg(TYPE_INT);
							offs = argsym->offset - stkframesize;

							asmld(MOVL, offs, reg_num, argsym->nameString);
							asmrr(MOVL, reg_num, EDI);
							asmcall(lhs->stringval);
						}

						else if (argsym->basicType == TYPE_REAL) {
							reg_num = getreg(TYPE_REAL);
							offs = argsym->offset - stkframesize;
							asmld(MOVSD, offs, reg_num, argsym->nameString);
							asmrr(MOVSD, reg_num, EDI);
							asmcall(lhs->stringval);
						}
					}
				}
			}
			// other procedure
			else {
				TOKEN arglist = rhs;
				int temp_reg, index = 0;
				while (arglist) {
					temp_reg = genarith(arglist);
					asmrr(MOVL, temp_reg, arg_reg[index]); // score values into arg reg
					mark_reg_used(arg_reg[index++]);
					free_reg(temp_reg);
					arglist = arglist->link;
				}
				asmcall(lhs->stringval);
				free_reg(arg_reg[0]);
				free_reg(arg_reg[1]);
				free_reg(arg_reg[2]);
				free_reg(arg_reg[3]);
			}
		} // end of CASE OP_FUNCALL
		break;
		case OP_FUNDCL: {

			if (DEBUGGEN) {
				printf(" OP_FUNDCL detected.\n");
				ppexpr(code);
				ppexpr(code->operands);
				ppexpr(code->operands->link);
				printf("\n");
			}

			lhs = code->operands;
			rhs = code->operands->link;
			
			blocknumber = lhs->operands->intval;

			cannedcode(funtopcode);	// print function top code

			TOKEN arglist;

			// store arguments into vars(memory)
			if (strcmp(lhs->stringval, "function") == 0) {
				arglist = lhs->operands->link->link->link;
			}
			else {
				arglist = lhs->operands->link->link;
			}
			int index = 0;
			while (arglist) {
				SYMBOL argsym = searchst(arglist->stringval);
				switch (argsym->basicType) {
					case TYPE_INT: {
						reg_num = arg_reg[index++];
						mark_reg_used(reg_num);
						offs = argsym->offset - stkframesize;
						asmst(MOVL, reg_num, offs, argsym->nameString);
					}
					break;
					case TYPE_REAL: {
						reg_num = arg_reg[index++];
						mark_reg_used(reg_num);
						offs = argsym->offset - stkframesize;
						asmst(MOVSD, reg_num, offs, argsym->nameString);
					}
					break;
				}
				arglist = arglist->link;
			}

			genc(rhs);	// routine body

			TOKEN fun_name = lhs->operands->link;

			// store return value into %eax
			if (strcmp(lhs->stringval, "function") == 0) {
				char fun_var[16];
				int i;
				fun_var[0] = '_';
				for (i = 1; i < 16; i++) {
					fun_var[i] = fun_name->stringval[i-1];
				}
				SYMBOL sym = searchst(fun_var);
				switch (sym->basicType) {
					case TYPE_INT: {
						offs = sym->offset - stkframesize;
						asmld(MOVL, offs, EAX, sym->nameString);
					}
					break;
					case TYPE_REAL: {
						offs = sym->offset - stkframesize;
						asmld(MOVSD, offs, EAX, sym->nameString);
					}
					break;
				}
			}

			cannedcode(funbotcode);	// print function bottom code

		} // end of CASE OP_FUNDCL
		break;
	}
}

/* ##################################################################################################################################### */

void reset_regs() {
    int i;
    for (i = 0; i < NUM_REGS; i++) {
        used_regs[i] = 0;
    }
}

void free_reg(int reg_num) {
    if (reg_num < 0 || reg_num >= NUM_REGS) {
        printf("Error: cannot free register number %d\n", reg_num);
        return;
    }
    used_regs[reg_num] = 0;
}

boolean at_least_one_float(int lhs_reg, int rhs_reg) {
    if ((lhs_reg >= NUM_INT_REGS && lhs_reg < NUM_REGS) || 
        (rhs_reg >= NUM_INT_REGS && rhs_reg < NUM_REGS)) {
        return true;
    }
    return false;
}

boolean both_float(int lhs_reg, int rhs_reg) {
    if ((lhs_reg >= NUM_INT_REGS && lhs_reg < NUM_REGS) &&
        (rhs_reg >= NUM_INT_REGS && rhs_reg < NUM_REGS)) {
        return true;
    }
    return false;
}

void mark_reg_unused(int reg_num) {
    if (reg_num < 0 || reg_num >= NUM_REGS) {
        printf("Error: register %d out of bounds\n", reg_num);
        return;
    }
    used_regs[reg_num] = 0;
}

void mark_reg_used(int reg_num) {
    if (reg_num < 0 || reg_num >= NUM_REGS) {
        printf("Error: register %d out of bounds\n", reg_num);
        return;
    }
    used_regs[reg_num] = 1;    
}

/* test if there is a function call within code: 1 if true, else 0 */
boolean funcallin(TOKEN code) {
    int num = num_funcalls_in_tree(code, 0);
    if (num > 0) {
        return true;
    }
    return false;
}

int num_funcalls_in_tree(TOKEN tok, int num) {
    if (tok == NULL) {
        return num;
    }

    if (tok->whichval == OP_FUNCALL) {
//        ppexpr(tok);
        num++;
    }
    if (tok->link != NULL) {
        num = num_funcalls_in_tree(tok->link, num);
    }
    if (tok->operands != NULL) {
        num = num_funcalls_in_tree(tok->operands, num);
    }

    return num;
}

boolean search_tree_str(TOKEN tok, char str[]) {
    if (tok == NULL) {
        return false;
    }

//    dbugprinttok(tok);

    boolean found = false;

    if (strcmp(tok->stringval, str) == 0) {
        return true;
    }
    if (tok->link != NULL) {
        found = search_tree_str(tok->link, str);
    }
    if (tok->operands != NULL) {
        found = search_tree_str(tok->operands, str);
    }

    return found;
}

void print_used_regs() {
    printf("\nUsed registers: %d", used_regs[0]);
    
    int i;
    for (i = 1; i < NUM_REGS; i++) {
        printf(" %d", used_regs[i]);
    }
    printf("\n\n");
}

int symbol_is_null_int(char *str) {
    if (str) {
        printf("Error: NULL symbol (\"%s\")\n", str);
    }
    return 0;
}

boolean is_equal(TOKEN a, TOKEN b) {
    if (!a || !b) {
        return false;
    }

    if ((long) a == (long) b) {
        return true;
    }
    return false;
}

boolean is_gen_purpose_reg(int reg_num) {
    if (reg_num < 0 || reg_num >= NUM_INT_REGS) {
        return false;
    }
    return true;
}

boolean is_fp_reg(int reg_num) {
    if (reg_num < NUM_INT_REGS || reg_num >= NUM_REGS) {
        return false;
    }
    return true;
}

