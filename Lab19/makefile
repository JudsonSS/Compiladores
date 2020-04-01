
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: strcom

strcom: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o strcom

lex.yy.cc: strcom.l
	$(LEX) strcom.l

clean:
	rm strcom lex.yy.cc
