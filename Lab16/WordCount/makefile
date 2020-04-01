
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: wordcount

wordcount: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o wordcount

lex.yy.cc: wordcount.l
	$(LEX) wordcount.l

clean:
	rm wordcount lex.yy.cc
