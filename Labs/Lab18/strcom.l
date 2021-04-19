%{
#include <iostream>
using std::cout;
%}

%option noyywrap

%%
[ \t\n]+                ; // ignora espaços em branco
\"(\\.|[^\"\\])*\"      cout << "string\n";
"//".*$                 cout << "comment\n";
.+                      cout << "outro\n";
%%

int main()
{
    yyFlexLexer lexer;
	lexer.yylex();
}
