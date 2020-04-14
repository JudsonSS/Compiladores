# compilador 
CPP=g++
ARGS=-c -g -std=c++17

# dependências
all: expressions

expressions: expressions.o lexer.o
	$(CPP) expressions.o lexer.o -o expressions

expressions.o: expressions.cpp lexer.h 
	$(CPP) $(ARGS) expressions.cpp

lexer.o: lexer.cpp lexer.h
	$(CPP) $(ARGS) lexer.cpp

clean:
	rm expressions expressions.o lexer.o
