%{
// Exemplo de reconhecimento de verbos
#include <iostream>
using std::cout; 
%}
%option noyywrap

%%
[\t ]+		; // ignora espaços em branco
ser |
saber |
estar |
poder |
ter |
ir |
fazer |
dever |
dar 		cout << YYText() << " é um verbo\n";
[a-zA-Z]+	cout << YYText() <<  " não é um verbo\n";
.|\n		; // ignora qualquer outra coisa
%%

int main()
{
	yyFlexLexer lexer;
	lexer.yylex();
}
