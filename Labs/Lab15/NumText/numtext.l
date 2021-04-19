%{
// Exemplo de reconhecimento de números e texto 
#include <iostream>
using std::cout;
%}

%option noyywrap

%%
[ \t\n]+                                            ; // ignora espaços em branco
[+-]?(([0-9]+)|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)  cout << "número\n";
.+                                                  cout << "texto\n";
%%

int main()
{
    yyFlexLexer lexer;
	lexer.yylex();
}