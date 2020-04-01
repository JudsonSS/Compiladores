%{
// analisador sintático para reconhecer frases em português 
#include <iostream>
using std::cout;

int yylex(void);
int yyparse(void);
void yyerror(const char *);
%}

%token ARTIGO PRONOME VERBO ADJETIVO ADVERBIO SUBSTANTIVO

%%

texto: texto frase
	 | frase		     
	 ;

frase: sujeito VERBO objeto  { cout << "A frase é válida.\n"; }
	 ;

sujeito: PRONOME
	   | ARTIGO SUBSTANTIVO
	   ;

objeto:	ARTIGO SUBSTANTIVO
	  | ADJETIVO
	  | ADVERBIO
	  ;

%%

extern FILE * yyin;  // definido pelo analisador léxico

int main(int argc, char ** argv)
{
	// se foi passado um nome de arquivo
	if (argc > 1)
	{
		FILE * file;
		file = fopen(argv[1], "r");
		if (!file)
		{
			cout << "Arquivo " << argv[1] << " não encontrado!\n";
			exit(1);
		}
		
		// entrada ajustada para ler do arquivo
		yyin = file;
	}

	yyparse();
}

void yyerror(const char * s)
{
	extern int yylineno;    // definido pelo analisador léxico  
	extern char * yytext;   // definido pelo analisador léxico
	cout << "Erro sintático: símbolo \"" << yytext << "\" (linha " << yylineno << ")\n";
}
