%{
// analisador sintático para uma calculadora básica 
#include <iostream>
using std::cout;

int yylex(void);
int yyparse(void);
void yyerror(const char *);

/* cada letra do alfabeto é uma variável que pode receber um valor */
double variables[26];

%}

%union {
	double num;
	int ind;
}

%token <ind> VAR
%token <num> NUM

%type <num> expr

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

math: math calc '\n'
	| calc '\n'
	;

calc: VAR '=' expr 			{ variables[$1] = $3; } 	
	| expr					{ cout << "= " << $1 << "\n"; }
	; 

expr: expr '+' expr			{ $$ = $1 + $3; }
	| expr '-' expr   		{ $$ = $1 - $3; }
	| expr '*' expr			{ $$ = $1 * $3; }
	| expr '/' expr			{ 
								if ($3 == 0)
									yyerror("divisão por zero");
								else
									$$ = $1 / $3; 
							}
	| '(' expr ')'			{ $$ = $2; }
	| '-' expr %prec UMINUS { $$ = - $2; }
	| VAR					{ $$ = variables[$1]; }
	| NUM
	;

%%

int main()
{
	yyparse();
}

void yyerror(const char * s)
{
	extern int yylineno;    // definido no analisador léxico
	extern char * yytext;   // definifo no analisador léxico
    cout << "Erro (" << s << "): símbolo \"" << yytext << "\" (linha " << yylineno << ")\n";
}

