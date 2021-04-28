%{
// strings e comentários C++
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
