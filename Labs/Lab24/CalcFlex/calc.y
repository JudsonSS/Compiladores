%{
#include <iostream>
#include <cctype>
using std::cout;

int yylex(void);
int yyparse(void);
void yyerror(const char *);
%}

// tipo double para os atributos em yylval
%define api.value.type {double}

%token NUMBER

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

calc: calc expr '\n' 		{ cout << $2 << '\n'; }
	| calc '\n'
	| // vazio 
	; 

expr: expr '+' expr			{ $$ = $1 + $3; }
	| expr '-' expr   		{ $$ = $1 - $3; }
	| expr '*' expr			{ $$ = $1 * $3; }
	| expr '/' expr			{ $$ = $1 / $3; }
	| '(' expr ')'			{ $$ = $2; }
	| '-' expr %prec UMINUS { $$ = - $2; }
	| NUMBER 
	;

%%

int main()
{
	yyparse();
}

void yyerror(const char * s)
{
   	cout << "ERRO: " << s << '\n';
}