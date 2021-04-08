#include "lexer.h"
#include <fstream>
#include <sstream>
using std::stringstream;

extern std::ifstream fin;

// construtor 
Lexer::Lexer()
{
	// insere palavras-reservadas na tabela
	token_table["main"]  = Token{ Tag::MAIN, "main" };
	token_table["int"]   = Token{ Tag::TYPE, "int" };
	token_table["float"] = Token{ Tag::TYPE, "float" };
	token_table["bool"] = Token{ Tag::TYPE, "bool" };
	token_table["true"]  = Token{ Tag::TRUE, "true" };
	token_table["false"] = Token{ Tag::FALSE, "false" };
	token_table["if"]    = Token{ Tag::IF,    "if" };
	token_table["while"] = Token{ Tag::WHILE, "while" };
	token_table["do"]    = Token{ Tag::DO,    "do" };
	
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

	// ignora comentários
	while (peek == '/')
	{
		peek = fin.get();
		if (peek == '/')
		{
			// ignora caracteres até o fim da linha
			do
				peek = fin.get();
			while(peek != '\n');
			line += 1;
			peek = fin.get();
		}
		else if (peek == '*')
		{
			// ignora caracteres até achar */ ou EOF				
			while( (peek=fin.get()) != '*' ||  (peek=fin.get()) != '/' )
			{
				if (peek == '\n')
				{
					line += 1;
				}
				else if (peek == EOF)
				{
					token = Token {EOF};
					return &token;
				}
			}
			peek = fin.get();	
		}
		else
		{
			// barra encontrada não inicia um comentário
			fin.unget();
			peek = '/';
			break;
		}

		// remove espaços em branco, tabulações e novas linhas
		while (isspace(peek))
		{
			if (peek == '\n')
				line += 1;
			peek = fin.get();
		}
	}

	// retorna números
	if (isdigit(peek))
	{
		// ponto-flutuante não foi encontrado
		bool dot = false;
		
		// acumula dígitos do número
		stringstream ss;
		do 
		{
			if (peek == '.')
			{
				if (dot == false)
				{
					// primeiro ponto encontrado
					dot = true;
				}
				else
				{
					// segundo ponto encontrado
					break; 
				}
			}

			ss << peek;
			peek = fin.get();
		} 
		while (isdigit(peek) || peek == '.');

		// se o número é um ponto-flutuante
		if (dot)
		{
			token = Token{Tag::FLOATING, ss.str()};
			return &token;
		}
		else
		{
			token = Token{Tag::INTEGER, ss.str()};
			return &token;
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
		auto pos = token_table.find(s);

		// se o lexema já está na tabela
		if (pos != token_table.end())
		{
			// retorna o token da tabela
			token = pos->second;
			return &token;
		}

		// se o lexema ainda não está na tabela
		Token t {Tag::ID, s};
		token_table[s] = t;

		// retorna o token ID
		token = t;
		return &token;
	}

	// retorna operadores com mais de um caractere: >=, <=, == e !=
	switch(peek)
	{
		case '&':
		{
			char next = fin.get();
			if (next == '&')
			{
				peek = fin.get();
				token = Token{Tag::AND, "&&"};
				return &token;
			}
			else
			{
				fin.unget();
			}
		}
		break;

		case '|':
		{
			char next = fin.get();
			if (next == '|')
			{
				peek = fin.get();
				token = Token{Tag::OR, "||"};
				return &token;
			}
			else
			{
				fin.unget();
			}
		}
		break;

		case '>':
		{
			char next = fin.get();
			if (next == '=')
			{
				peek = fin.get();
				token = Token{Tag::GTE, ">="};
				return &token;
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
				token = Token{Tag::LTE, "<="};
				return &token;
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
				token = Token{Tag::EQ, "=="};
				return &token;
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
				token = Token{Tag::NEQ, "!="};
				return &token;	
			}
			else
			{
				fin.unget();
			}
		}
		break;
	}

	// retorna caracteres não alphanuméricos isolados: (, ), +, -, etc.
	token = Token{peek};
	peek = ' ';
	return &token;
}
