%{
// analisador sintático para uma calculadora
// com suporte a definição de variáveis  
#include <iostream>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::cout;

// protótipos das funções especiais
int yylex(void);
int yyparse(void);
void yyerror(const char *);

// tabela de símbolos 
unordered_map<string,double> variables; 
%}

%union {
	double num;
	char id[16];
}

%token <id> ID
%token <num> NUM

%type <num> expr

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

math: math calc '\n'
	| calc '\n'
	;

calc: ID '=' expr 			{ variables[$1] = $3; } 	
	| expr					{ cout << "= " << $1 << "\n"; }
	; 

expr: expr '+' expr			{ $$ = $1 + $3; }
	| expr '-' expr   		{ $$ = $1 - $3; }
	| expr '*' expr			{ $$ = $1 * $3; }
	| expr '/' expr			
	{ 
		if ($3 == 0)
			yyerror("divisão por zero");
		else
			$$ = $1 / $3; 
	}
	| '(' expr ')'			{ $$ = $2; }
	| '-' expr %prec UMINUS { $$ = - $2; }
	| ID					{ $$ = variables[$1]; }
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
	extern char * yytext;   // definido no analisador léxico 
    cout << "Erro (" << s << "): símbolo \"" << yytext << "\" (linha " << yylineno << ")\n";
}
