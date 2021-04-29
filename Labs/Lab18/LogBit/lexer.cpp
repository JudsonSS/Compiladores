#include "lexer.h"
#include <iostream>
using std::cin;
using std::cout;

// construtor
Lexer::Lexer()
{
}

// retorna tokens para o analisador sintático
int Lexer::Scan()
{
	// salta espaços em branco, tabulações e novas linhas
	while (isspace(peek))
	{
		if (peek == '\n')
			line += 1;
		peek = cin.get();
	}

	// retorna números
	if (isdigit(peek))
	{
		do
			peek = cin.get();
		while (isdigit(peek));

		// exibe e retorna o token reconhecido
		cout << "<NUM> ";
		return NUM;
	}

	// retorna identificadores
	if (isalpha(peek))
	{
		do
			peek = cin.get();
		while (isalpha(peek));

		// exibe e retorna o token reconhecido
		cout << "<ID> ";
		return ID;
	}

	// ------------------------------
	// operadores lógicos e bit a bit
	// ------------------------------

	// responda aqui

	// ------------------------------

	// exibe e retorna o token para caracteres
	cout << "<" << peek << "> ";
	peek = cin.get();
	return int(peek);
}

void Lexer::Start()
{
	// simula o analisador sintático pedindo tokens para o analisador léxico
	while (peek != '\n')
		Scan();
}
