
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: numtext

numtext: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o numtext

lex.yy.cc: numtext.l
	$(LEX) numtext.l

clean:
	rm numtext lex.yy.cc
