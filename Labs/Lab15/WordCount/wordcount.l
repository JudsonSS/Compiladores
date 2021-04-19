%{
// Faz contagem de caracteres, palavras e linhas de um arquivo
#include <iostream>
#include <fstream>
using namespace std;

unsigned charCount = 0, wordCount = 0, lineCount = 0;
%}

%option noyywrap

word [^ \t\n]+
eol  \n

%%

{word}	{ wordCount++; charCount += YYLeng(); }
{eol}	{ charCount++; lineCount++; }
.	    { charCount++; }

%%

int main(int argc, char ** argv)
{
	yyFlexLexer lexer;  // lexer lê da entrada padrão (teclado)
	ifstream fin;

	if (argc > 1) 
	{    
		fin.open(argv[1]);
		if (!fin.is_open()) 
        {
			cout << "Arquivo " << argv[1] << " não pode ser aberto!\n";
			exit(EXIT_FAILURE);
		}
		else
		{
			// altera entrada para arquivo aberto
			lexer.switch_streams(&fin);
		}
	}

	lexer.yylex();

	cout << "Lines: " << lineCount << "\n"
		 << "Words: " << wordCount << "\n"
		 << "Chars: " << charCount << "\n";

	fin.close();
	return 0;
}
