%{
// Este exemplo apenas repete os caracteres digitados
%}

%option noyywrap

%%
.|\n	ECHO;
%%

int main()
{
	yyFlexLexer lexer;
	lexer.yylex();
}