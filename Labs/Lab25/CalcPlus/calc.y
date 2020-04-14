%{
#include <iostream>
#include <cctype>
#define YYSTYPE double   // tipo double para os atributos em yylval

using std::cin;
using std::cout;

int yylex(void);
int yyparse(void);
void yyerror(const char *);

%}

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

int yylex() 
{
	char ch;
	// ignora espaços em branco
	while ((ch=cin.get()) == ' ')
		continue;

	// se encontrar um número
    if ((ch == '.') || isdigit(ch))
	{
		cin.unget();
		cin >> yylval;
		return NUMBER;
	}

	// qualquer outro caractere é um token
	return ch;
}

void yyerror(const char * s)
{
   	cout << "ERRO: " << s << '\n';
}

int main()
{
	yyparse();
}
