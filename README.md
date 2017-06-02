# SPL-compiler

reference from CS375: UT

materials: ftp://ftp.cs.utexas.edu/pub/novak/cs375/



## Part 1

Lexical Analyzer

#### Files Needed

Modified/new files

- lexan.l
- Makefile

Files req'd from /projects/CS375/

1) lexan.h

2) lexanl.c

- printtoken.c
- token.h

### Instruction

```
$ make lexer
$ ./lexer < [filename.spl]
```

```
yylex() =  306   tokentype:  2  which:   19      program
yylex() =  258   tokentype:  3  value:            graph1
yylex() =  283   tokentype:  1  which:    4            (
yylex() =  258   tokentype:  3  value:            output
yylex() =  284   tokentype:  1  which:    5            )
yylex() =  281   tokentype:  1  which:    2            ;
yylex() =  314   tokentype:  2  which:   27          var
yylex() =  258   tokentype:  3  value:                 i
yylex() =  280   tokentype:  1  which:    1            ,
yylex() =  258   tokentype:  3  value:               lim
yylex() =  282   tokentype:  1  which:    3            :
yylex() =  258   tokentype:  3  value:           integer
yylex() =  281   tokentype:  1  which:    2            ;
yylex() =  289   tokentype:  2  which:    2        begin
yylex() =  258   tokentype:  3  value:               lim
yylex() =  265   tokentype:  0  which:    5           :=
yylex() =  260   tokentype:  5  type:     0            7
yylex() =  281   tokentype:  1  which:    2            ;
yylex() =  297   tokentype:  2  which:   10          for
yylex() =  258   tokentype:  3  value:                 i
yylex() =  265   tokentype:  0  which:    5           :=
yylex() =  260   tokentype:  5  type:     0            0
yylex() =  311   tokentype:  2  which:   24           to
yylex() =  258   tokentype:  3  value:               lim
yylex() =  292   tokentype:  2  which:    5           do
yylex() =  258   tokentype:  3  value:           writeln
yylex() =  283   tokentype:  1  which:    4            (
yylex() =  259   tokentype:  4  value:                 *
yylex() =  284   tokentype:  1  which:    5            )
yylex() =  295   tokentype:  2  which:    8          end
yylex() =  273   tokentype:  0  which:   13            .
```