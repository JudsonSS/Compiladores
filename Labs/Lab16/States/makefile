
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: states

states: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o states

lex.yy.cc: states.l
	$(LEX) states.l

clean:
	rm states lex.yy.cc
