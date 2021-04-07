#include <iostream>
#include <fstream>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include "ast.h"
#include "gen.h"
#include "checker.h"

using namespace std;

ifstream fin;
Lexer * scanner;

// programa pode receber nomes de arquivos
int main(int argc, char **argv)
{
	if (argc == 2)
	{
		fin.open(argv[1]);
		if (!fin.is_open())
		{
			cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
			exit(EXIT_FAILURE);
		}

		//TestLexer();
		Lexer leitor;
		scanner = &leitor;
		Statement * ast;		
		Parser tradutor;
		try
		{
			// gera árvore sintática
			ast = tradutor.Start();
			
			// gera código intermediário
			ast->Gen();
		}
		catch (SyntaxError err)
		{
			err.What();
		}
		fin.close();
		//TestParser(ast);		
	}
}