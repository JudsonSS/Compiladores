# compilador 
CPP=g++
ARGS=-c -g -std=c++17

# dependências
all: postfix

postfix: postfix.o parser.o
	$(CPP) postfix.o parser.o -o postfix

postfix.o: postfix.cpp parser.h 
	$(CPP) $(ARGS) postfix.cpp

parser.o: parser.cpp parser.h
	$(CPP) $(ARGS) parser.cpp

clean:
	rm postfix postfix.o parser.o
