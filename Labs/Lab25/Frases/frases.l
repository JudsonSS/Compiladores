%{
/* analisador léxico para o reconhecedor de frases */
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

#include "frases.tab.h"
#define	CONSULTA 0   /* padrão - buscar em vez de definir */

/* tabela armazena a palavra e sua respectiva classificação */
unordered_map<string,int> tabela;
int estado;

/* funções de inserção e busca na tabela */
int inserir(int tipo, string palavra);
int buscar(string palavra);

%}

%option yylineno
%option noyywrap

%%
\n		estado = CONSULTA; // fim de linha, retorna ao estado padrão
^pron	estado = PRONOME; 
^arti	estado = ARTIGO; 
^subs	estado = SUBSTANTIVO; 
^verb	estado = VERBO; 
^adj	estado = ADJETIVO; 
^adv	estado = ADVERBIO; 

[a-zA-Z]+  {
		/* palavra reconhecida, defina-a ou busque-a */
	    if (estado != CONSULTA) 
		{
	     	inserir(estado, yytext);
	    } 
		else 
		{
			switch(buscar(yytext)) 
			{
				case PRONOME: return PRONOME;
				case ARTIGO: return ARTIGO;
				case VERBO: return VERBO;
				case ADJETIVO: return ADJETIVO;
				case ADVERBIO: return ADVERBIO;
				case SUBSTANTIVO: return SUBSTANTIVO;
				default: cout << "Erro léxico: \"" << yytext << "\" não reconhecido\n";
			}
        }
    }

.	; /* ignore qualquer outra coisa */

%%

/* insere novos nomes na tabela */
int inserir(int tipo, string palavra)
{
	/* se palavra já está na tabela */
	if (buscar(palavra) != 0) {
		cout << "!!! atenção: " << palavra << " já definido\n";
		return 0;
	}
	
	/* palavra ainda não está na tabela, vamos inserir */
	tabela.insert({palavra, tipo});

	return 1;	/* tudo certo */
}

/* busca por um nome na tabela */
int buscar(string word)
{
	/* iterador aponta para o elemento ou tabela.end() */
	auto pos = tabela.find(word);

	/* se elemento não for encontrado */
	if (pos == tabela.end()) 
		return 0;

	/* elemento encontrado */
	return pos->second;
}

