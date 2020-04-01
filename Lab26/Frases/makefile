all: frases

# Compiladores
CPP=g++
FLEX=flex 
BISON=bison

frases: lex.yy.c frases.tab.c
	$(CPP) lex.yy.c frases.tab.c -std=c++17 -o frases

lex.yy.c: frases.l
	$(FLEX) frases.l

frases.tab.c: frases.y
	$(BISON) -d frases.y

clean:
	rm frases lex.yy.c frases.tab.c frases.tab.h
