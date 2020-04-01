all: calc

# Compiladores
CPP=g++
FLEX=flex
BISON=bison

calc: lex.yy.c calc.tab.c
	$(CPP) lex.yy.c calc.tab.c -std=c++17 -o calc

lex.yy.c: calc.l
	$(FLEX) calc.l

calc.tab.c: calc.y
	$(BISON) -d calc.y

clean:
	rm calc lex.yy.c calc.tab.c calc.tab.h
