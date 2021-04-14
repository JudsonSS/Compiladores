%{
// Reconhecedor de palavras com uma tabela de símbolos
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

enum {
	CONSULTA = 0, /* padrão - buscar em vez de definir */
	VERB,
	ADJ,
	ADV,
	SUBS,
	PREP,
	PRON,
	CONJ
};

int estado; 
unordered_map<string,int> tabela;

void inserir(int tipo, string palavra);
int buscar(string palavra);
%}

%option noyywrap

%%
\n		estado = CONSULTA; 	// fim de linha, retorna ao estado padrão

^verb	estado = VERB; 
^adj	estado = ADJ; 
^adv	estado = ADV;
^subs	estado = SUBS; 
^prep	estado = PREP; 
^pron	estado = PRON; 
^conj	estado = CONJ; 

[a-zA-Z]+ {
		    // palavra reconhecida, defina-a ou busque-a
	        if (estado != CONSULTA) 
		    {
				inserir(estado, YYText());
	        } 
		    else 
		    {
		      	switch(buscar(YYText())) 
		      	{
		      	  	case VERB: cout << YYText() << ": verbo\n"; break;
		      	  	case ADJ:  cout << YYText() << ": adjetivo\n"; break;
		      	  	case ADV:  cout << YYText() << ": advérbio\n"; break;
		      	  	case SUBS: cout << YYText() << ": substantivo\n"; break;
		      	  	case PREP: cout << YYText() << ": preposição\n"; break;
		      	  	case PRON: cout << YYText() << ": pronome\n"; break;
		      	  	case CONJ: cout << YYText() << ": conjunção\n"; break;
		      	  	default:   cout << YYText() << ": não reconhecido\n"; break;
		      	}
            }
        }

.		; // ignore qualquer outra coisa 

%%

int main()
{
	yyFlexLexer lexer;
	lexer.yylex();
}

void inserir(int tipo, string palavra)
{
	if (buscar(palavra) != CONSULTA)
	{ 
		cout << "!!! atenção: palavra \""  << palavra << "\" já definida\n";
	}
	else
	{
		// palavra ainda não está na tabela, vamos inserir
		tabela.insert({palavra, tipo});
	}
}

int buscar(string s)
{
	// iterador aponta para o elemento ou tabela.end()
	auto pos = tabela.find(s);

	// se elemento não for encontrado
	if (pos == tabela.end()) 
		return 0;

	// elemento encontrado
	return pos->second;
}
