# Compiladores
CC=g++
LEX=flex++

# Dependências
all: flexlb

flexlb: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o flexlb

lex.yy.cc: flexlb.l
	$(LEX) flexlb.l

clean:
	rm flexlb lex.yy.cc
