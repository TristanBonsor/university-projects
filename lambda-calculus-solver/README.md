# Lambda Calculus Solver

Type 'make' in directory with Makefile to build program.

Test input files provided named test1.txt -> test5.txt

PLEASE USE THE FOLLOWING BNF GRAMMAR OR ELSE YOU WILL HAVE A BAD TIME.

E = Expression
I = Identifer

E ::= (lI.E) | (E E) | I

Accepts identifiers of multiple lengths, just be sure to seperate them
by whitespace.

An example input file should look like:

((la.(lb.((a b) b)))(lb.b))b  // example from assignment -> (la.lb.abb)(lb.b)b

Program is capable of beta reductions, alpha renaming, and encoding
replacement (bonus) (syntax for encoding replacement must strictly
follow example given in assignment handout along with BNF
restrictions for lambda expressions).

Any syntax errors in an input file may lead to undefined behaviour.

Sorry for the lack of comments in the code; I was rushed.

Basic program behaviour:
1: Scan input (tokenize).
2: Parse tokens (build a lambda expression tree).
3: Reduce and replace until no longer possible (recursion for most of it).

As stated above, failure to adhere to the BNF given for lambda expressions
in the input text file will result in undefined program behaviour
(that means use brackets)!
