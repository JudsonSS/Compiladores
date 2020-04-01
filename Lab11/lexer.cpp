#include "lexer.h"
#include <fstream>
#include <sstream>
using std::stringstream;

extern std::ifstream fin;

// construtor 
Lexer::Lexer()
{
	// insere palavras-reservadas na tabela de id's
	id_table["program"] = Id{ Tag::PROGRAM, "program" };
	id_table["int"]     = Id{ Tag::TYPE, "int" };
	id_table["float"]   = Id{ Tag::TYPE, "float" };
	id_table["true"]    = Id{ Tag::TRUE, "true" };
	id_table["false"]   = Id{ Tag::FALSE, "false" };
	id_table["if"]      = Id{ Tag::IF,    "if" };
	id_table["while"]   = Id{ Tag::WHILE, "while" };
	id_table["do"]      = Id{ Tag::DO,    "do" };
	
	// inicia leitura da entrada
	peek = fin.get();
}

// retorna número da linha atual
int Lexer::Lineno()
{
	return line;
}

// retorna tokens para o analisador sintático
Token * Lexer::Scan()
{
	// ignora espaços em branco, tabulações e novas linhas
	while (isspace(peek))
	{
		if (peek == '\n')
			line += 1;
		peek = fin.get();
	}

	// retorna números
	if (isdigit(peek))
	{
		int vi = 0;		// valor inteiro
		float vf = 0;	// valor float

		// ponto decimal do ponto-flutuante ainda não foi encontrado
		bool dot = false;
		float mult = 0.1;
		
		do 
		{
			// converte 'n' para o dígito numérico n
			float n = peek - '0';

			// se é um ponto-flutuante
			if (dot)
			{
				vf = vf + mult * n;
				mult *= 0.1;
			} 
			else
			{
				vi = 10 * vi + n;
			}
			peek = fin.get();

			// encontrado um ponto decimal
			if (peek == '.')
			{
				dot = true;
				vf = vi;
				peek = fin.get();
			}
		} 
		while (isdigit(peek));

		// se é um ponto-flutuante
		if (dot)
		{
			token.f = Floating{vf};
			return &token.f;
		}
		else
		{
			token.i = Integral{vi};
			return &token.i;
		}
		
		
	}

	// retorna palavras-chave e identificadores
	if (isalpha(peek))
	{
		stringstream ss;
		do 
		{
			ss << peek;
			peek = fin.get();
		} 
		while (isalpha(peek));

		string s = ss.str();
		auto pos = id_table.find(s);

		// se o lexema já está na tabela
		if (pos != id_table.end())
		{
			// retorna o token associado
			token.id = pos->second;
			return &token.id;
		}

		// se o lexema ainda não está na tabela
		Id new_id {Tag::ID, s};
		id_table[s] = new_id;

		// retorna o token ID
		token.id = new_id;
		return &token.id;
	}

	// retorna operadores com mais de um caractere: >=, <=, == e !=
	// eles serão armazenados em Tokens de tipo Id para que o método
	// toString retorne sua representação string corretamente
	switch(peek)
	{
		case '>':
		{
			char next = fin.get();
			if (next == '=')
			{
				peek = fin.get();
				Id gte {Tag::GTE, ">="};
				token.id = gte;
				return &token.id;
			}
			else
			{
				fin.unget();
			}
		}
		break;

		case '<':
		{
			char next = fin.get();
			if (next == '=')
			{
				peek = fin.get();
				Id lte {Tag::LTE, "<="};
				token.id = lte;
				return &token.id;
			}
			else
			{
				fin.unget();
			}
		}
		break;

		case '=':
		{
			char next = fin.get();
			if (next == '=')
			{
				peek = fin.get();
				Id eq {Tag::EQ, "=="};
				token.id = eq;
				return &token.id;
			}
			else
			{
				fin.unget();
			}
		}
		break;

		case '!':
		{
			char next = fin.get();
			if (next == '=')
			{
				peek = fin.get();
				Id neq {Tag::NEQ, "!="};
				token.id = neq;
				return &token.id;	
			}
			else
			{
				fin.unget();
			}
		}
		break;
	}

	// operadores (e caracteres não alphanuméricos isolados)
	token.t.tag = peek;
	peek = ' ';
	return &token.t;
}