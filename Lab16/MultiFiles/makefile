
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: multifiles

multifiles: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o multifiles

lex.yy.cc: multifiles.l
	$(LEX) multifiles.l

clean:
	rm multifiles lex.yy.cc
