%{
#include "calc.tab.h"
%}

%option noyywrap

numero		[0-9]+("."[0-9]+)?

%%
[\t ]+		; // ignora espaço em branco
{numero}    yylval = atof(yytext); return NUMBER;
.|\n		return yytext[0]; 

%%
