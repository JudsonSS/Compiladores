#include <iostream>
#include "parser.h"
#include "error.h"
using namespace std;

int main()
{
	Parser tradutor;
	try
	{
		tradutor.Start();
	}
	catch (SyntaxError erro)
	{
		erro.What();
	}
	cout << endl;
}