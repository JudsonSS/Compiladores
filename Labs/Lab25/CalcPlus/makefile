
# Compiladores
CPPC=g++
YACC=bison

# Dependências
all: calc

calc: calc.tab.c
	$(CPPC) calc.tab.c -std=c++17 -o calc

calc.tab.c: calc.y
	$(YACC) calc.y

clean:
	rm calc calc.tab.c
