
# Compiladores
CC=g++
LEX=flex++

# Dependências
all: echo

echo: lex.yy.cc
	$(CC) lex.yy.cc -std=c++17 -o echo

lex.yy.cc: echo.l
	$(LEX) echo.l

clean:
	rm echo lex.yy.cc
