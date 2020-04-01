
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: verbs

verbs: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o verbs

lex.yy.cc: verbs.l
	$(LEX) verbs.l

clean:
	rm verbs lex.yy.cc
