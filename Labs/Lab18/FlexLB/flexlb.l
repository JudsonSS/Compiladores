%{
#include <iostream>
using std::cout;
%}

%option noyywrap

white [ \t]+
num [0-9]+
id  [a-zA-Z_]+

%%

 /**********************************/
 /* operadores lógicos e bit a bit */
 /**********************************/

 /* responda aqui */

 /**********************************/

{white}			;
{num}			cout << "<NUM> ";
{id}			cout << "<ID> ";
.               cout << "<" << YYText() << "> "; 
%%

int main()
{
	yyFlexLexer scanner;
	scanner.yylex();
}
