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
		// {
		// 	switch(t->tag)
		// 	{
		// 		case ID: cout << "<ID," << t->toString() << ">" << endl; break;
		// 		case INT: cout << "<INT," << t->toString() << ">" << endl; break;
		// 		case FLOAT: cout << "<FLOAT," << t->toString() << ">" << endl; break;
		// 		case TYPE: cout << "<TYPE," << t->toString() << ">" << endl; break;
		// 		case TRUE: cout << "<TRUE," << t->toString() << ">" << endl; break;
		// 		case FALSE: cout << "<FALSE," << t->toString() << ">" << endl; break;
		// 		case MAIN: cout << "<MAIN," << t->toString() << ">" << endl; break;
		// 		case IF: cout << "<IF," << t->toString() << ">" << endl; break;
		// 		case WHILE: cout << "<WHILE," << t->toString() << ">" << endl; break;
		// 		case DO: cout << "<DO," << t->toString() << ">" << endl; break;
		// 		case OR: cout << "<OR," << t->toString() << ">" << endl; break;
		// 		case AND: cout << "<AND," << t->toString() << ">" << endl; break;
		// 		case EQ: cout << "<EQ," << t->toString() << ">" << endl; break;
		// 		case NEQ: cout << "<NEQ," << t->toString() << ">" << endl; break;
		// 		case LTE: cout << "<LTE," << t->toString() << ">" << endl; break;
		// 		case GTE: cout << "<GTE," << t->toString() << ">" << endl; break;
		// 		default: cout << "<" << t->toString() << ">" << endl; break;
		// 	}
		// }

		fin.close();
	}
}
