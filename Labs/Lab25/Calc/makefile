all: calc

# Compiler
CPPC=g++

# Lexer
FLEX=flex

# Yacc 
BISON=bison

calc: lex.yy.c calc.tab.c
	$(CPPC) lex.yy.c calc.tab.c -std=c++17 -o calc

lex.yy.c: calc.l
	$(FLEX) calc.l

calc.tab.c: calc.y
	$(BISON) -d calc.y

clean:
	rm calc lex.yy.c calc.tab.c calc.tab.h
