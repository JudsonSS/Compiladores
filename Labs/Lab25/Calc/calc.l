%{ 
/* analisador léxico para uma calculadora básica */ 
#include "calc.tab.h"
extern double variables[26];
%}

%option yylineno
%option noyywrap

numero  [0-9]+("."[0-9]+)?
var     [a-z]

%%

[\t ]+		{ ; /* ignora espaços em branco */ }
{numero}    { yylval.num = atof(yytext); return NUM; }
{var}		{ yylval.ind = yytext[0] - 'a'; return VAR; }
.|\n		{ return yytext[0]; }

%%

