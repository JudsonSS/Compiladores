%{
// Contagem de caracteres, palavras e linhas de vários arquivos
#include <iostream>
#include <fstream>
using namespace std;

char **fileList;
unsigned nFiles;
unsigned currentFile = 0;
bool openFile = false;
unsigned long charCount = 0, wordCount = 0, lineCount = 0;
unsigned long totalCC = 0, totalWC = 0, totalLC = 0;
ifstream fin;
%}

word [^ \t\n]+
eol  \n

%%

{word}	{ wordCount++; charCount += YYLeng(); }
{eol}	{ charCount++; lineCount++; }
.	    { charCount++; }

%%

int main(int argc, char ** argv)
{	
    fileList = argv + 1;         // salta nome do programa
    nFiles = argc - 1;  	     // desconsidera nome do programa
	
	yyFlexLexer lexer;	 		 // analisador léxico
    lexer.switch_streams(&fin);  // altera entrada para arquivo
	lexer.yywrap();				 // abre arquivo

	// processa arquivo aberto
	if (openFile)
	{
		// yylex chama yywrap ao final de cada arquivo
		lexer.yylex();

		// executado somente depois de processar todos os arquivos    
    	if (nFiles > 1) 
    	{
			cout << "-----------------\n";
			cout << " Total = " << totalLC << " " << totalWC << " " << totalCC << "\n";
		} 
    	else
		{
			cout << lineCount << " lines, " << wordCount << " words, " << charCount << " chars.\n";
		}
	}
}


// a função yylex chama yywrap para tratar EOF, 
// permitindo assim a abertura de outros aquivos

int yyFlexLexer::yywrap()
{
	// se não for o primeiro nem o último arquivo de uma sequência válida
	if ((currentFile != 0) && (nFiles > 1)) 
    {
		// exibe as estatísticas para o arquivo processado 
		cout << fileList[currentFile-1] << " = " << lineCount << " " << wordCount << " " << charCount << "\n";
		totalCC += charCount;
		totalWC += wordCount;
		totalLC += lineCount;
		charCount = wordCount = lineCount = 0;		
	}

	// fecha o arquivo processado
	if (openFile)
	{    
		openFile = false;
		fin.close();
	}

	// enquanto existir arquivos para processar, tentar abrir o próximo
	while (!openFile && (fileList[currentFile] != nullptr)) 
    {
		fin.open(fileList[currentFile++]);
		if (fin.is_open()) 
        {
			openFile = true;
		}
		else
		{
			cout << fileList[currentFile-1] << " = não foi possível abrir\n";
		}		
	}

	return (openFile ? 0 : 1);	// 0 significa que tem mais entrada para processar
}
