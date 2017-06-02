{ First test program       -- file trivb.pas }


{ Results should look like this:
 yyparse result =        0
 Symbol table level 0   ... printed out so you can see it. }
{Symbol table level 1
 25590736           i  VAR    0 typ integer  lvl  1  siz     4  off     0   }
{25590832         lim  VAR    0 typ integer  lvl  1  siz     4  off     4
 token 25592784  OP       program  dtype  0  link 0  operands 25589968
}
{
(program graph1 (progn output)
                (progn (:= lim 7)
                       (progn (:= i 0)
                              (label 0)                        }
{                             (if (<= i lim)
                                  (progn (funcall writeln '*')
                                         (:= i (+ i 1))
                                         (goto 0))))))         }

program graph1(output);
var i,lim : integer;
begin
   lim := 7;
   for i := 0 to lim do
      writeln('*')
end.
