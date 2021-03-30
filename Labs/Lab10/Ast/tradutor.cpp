#include <iostream>
#include <fstream>
#include <cstring>
#include "parser.h"
#include "error.h"
using namespace std;

// protótipos
void ScanTest();

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

		// teste do analisador léxico
		//ScanTest();
		
		Parser tradutor;
		try
		{
			tradutor.Start();
		}
		catch (SyntaxError err)
		{
			err.What();
		}

		fin.close();
	}
}

void ScanTest()
{
	Lexer scanner;
	Token *t = nullptr;
	while ((t = scanner.Scan()) && (t->tag != EOF))
	{
		switch (t->tag)
		{
		case ID:
			cout << "<ID," << t->ToString() << "> ";
			break;
		case INTEGER:
			cout << "<INTEGER," << t->ToString() << "> ";
			break;
		case REAL:
			cout << "<REAL," << t->ToString() << "> ";
			break;
		case TYPE:
			cout << "<TYPE," << t->ToString() << "> ";
			break;
		case TRUE:
			cout << "<TRUE," << t->ToString() << "> ";
			break;
		case FALSE:
			cout << "<FALSE," << t->ToString() << "> ";
			break;
		case MAIN:
			cout << "<MAIN," << t->ToString() << "> ";
			break;
		case IF:
			cout << "<IF," << t->ToString() << "> ";
			break;
		case WHILE:
			cout << "<WHILE," << t->ToString() << "> ";
			break;
		case DO:
			cout << "<DO," << t->ToString() << "> ";
			break;
		case OR:
			cout << "<OR," << t->ToString() << "> ";
			break;
		case AND:
			cout << "<AND," << t->ToString() << "> ";
			break;
		case EQ:
			cout << "<EQ," << t->ToString() << "> ";
			break;
		case NEQ:
			cout << "<NEQ," << t->ToString() << "> ";
			break;
		case LTE:
			cout << "<LTE," << t->ToString() << "> ";
			break;
		case GTE:
			cout << "<GTE," << t->ToString() << "> ";
			break;
		default:
			cout << "<" << t->ToString() << "> ";
			break;
		}
	}

	fin.clear();
	fin.seekg(0, ios::beg);
	cout << endl << endl;
}