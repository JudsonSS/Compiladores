%{
// strings e comentários em C++
#include <iostream>
using std::cout;
%}

%option noyywrap

%%

%%

int main()
{
    yyFlexLexer lexer;
	lexer.yylex();
}
