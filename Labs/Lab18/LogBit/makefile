# compilador 
CPP=g++
ARGS=-c -g -std=c++17

# dependências
all: logbit

logbit: logbit.o lexer.o
	$(CPP) logbit.o lexer.o -o logbit

logbit.o: logbit.cpp lexer.h 
	$(CPP) $(ARGS) logbit.cpp

lexer.o: lexer.cpp lexer.h
	$(CPP) $(ARGS) lexer.cpp

clean:
	rm logbit logbit.o lexer.o
