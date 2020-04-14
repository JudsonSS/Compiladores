#include <iostream>
#include <fstream>
#include <cstring>
#include "parser.h"
#include "error.h"
using namespace std;

// arquivo de entrada
ifstream fin;

// programa pode receber nomes de arquivos
int main(int argc, char **argv)
{
	if (argc == 2)
	{
		fin.open(argv[1]);
		if (!fin.is_open())
		{
			cout << "Falha na abertura do arquivo \'" << argv[1] << "\'.\n";
			cout << "Verifique se o nome está correto, ou se é protegido para leitura.\n";
			exit(EXIT_FAILURE);
		}

		Parser tradutor;
		try
		{
			tradutor.Start();
		}
		catch (SyntaxError err)
		{
			err.What();
		}

		// Lexer scanner;
		// Token * t = nullptr; 
		// while((t = scanner.Scan()) && (t->tag != EOF))
		// 	cout << t->toString() << endl; 

		fin.close();
	}
}
