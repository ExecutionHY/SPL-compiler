# Makefile for SPL-compiler
clean:
	rm -rf *.o

# To compile your file lexan.l --> lexer
# 
lexer:  lex.yy.o lexanl.o printtoken.o token.h lexan.h
	cc -o lexer lex.yy.o lexanl.o printtoken.o


lex.yy.o: lex.yy.c
	cc -c lex.yy.c

lex.yy.c: lexan.l token.h
	lex lexan.l

lexanl.o: lexanl.c token.h lexan.h
	cc -c lexanl.c

printtoken.o: printtoken.c token.h
	cc -c printtoken.c


# To compile your file parse.y --> parser
#      using your file lexan.l
parser: y.tab.o lex.yy.o printtoken.o pprint.o symtab.o
	cc -o parser y.tab.o lex.yy.o printtoken.o pprint.o symtab.o -ll


y.tab.o: y.tab.c
	cc -c y.tab.c

y.tab.c: parse.y token.h parse.h symtab.h lexan.h
	yacc parse.y

lex.yy.o: lex.yy.c
	cc -c lex.yy.c

lex.yy.c: lexan.l token.h
	lex lexan.l

pprint.o: pprint.c token.h
	cc -c pprint.c

symtab.o: symtab.c token.h symtab.h
	cc -c symtab.c



# To compile your file codegen.c --> compiler
#      using your files lexan.l and parse.y
compiler: y.tab.o lex.yy.o printtoken.o pprint.o symtab.o codegen.o genasm.o
	cc -o compiler y.tab.o lex.yy.o printtoken.o pprint.o symtab.o \
             codegen.o genasm.o

genasm.o: genasm.c token.h symtab.h genasm.h
	cc -c genasm.c

codegen.o: codegen.c token.h symtab.h genasm.h
	cc -c codegen.c

