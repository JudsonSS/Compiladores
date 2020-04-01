%{
// Contagem de caracteres, palavras e linhas de vários arquivos
#include <iostream>
#include <fstream>
using namespace std;

char **fileList;
unsigned currentFile = 0;
unsigned nFiles;
unsigned long charCount = 0, wordCount = 0, lineCount = 0;
unsigned long totalCC = 0, totalWC = 0, totalLC = 0;
ifstream fin;
%}

word [^ \t\n]+
eol  \n

%%

{word}	{ wordCount++; charCount += yyleng; }
{eol}	{ charCount++; lineCount++; }
.	    { charCount++; }

%%

int main(int argc, char ** argv)
{	
	yyFlexLexer lexer;	// usa entrada padrão (teclado)

    fileList = argv+1;  // salta nome do programa
    nFiles = argc-1;	// desconsidera nome do programa

    // apenas um arquivo para processar
	if (nFiles == 1) 
    {
		/*
		 * o caso de um arquivo único é tratado de forma diferente
	     * do caso de múltiplos aquivos porque para apenas um arquivo
		 * não precisamos mostrar uma linha de "totais"
		 */
		fin.open(fileList[0]);
		if (!fin.is_open()) 
       	{
			cout << "Arquivo " << fileList[0] << " não pode ser aberto!";
			exit(EXIT_FAILURE);
		}
		else
		{
			// altera entrada para arquivo
			lexer.switch_streams(&fin);
		}
	}

	// mais de um arquivo para processar
    if (nFiles > 1)
	{
		lexer.switch_streams(&fin);		// altera entrada para arquivo
		lexer.yywrap();					// abre primeiro arquivo
	}

	lexer.yylex();

	// executado somente após processar todos os arquivos    
    if (nFiles > 1) 
    {
		// exibe as estatísticas do último arquivo processado e totais para todos os arquivos
		cout << fileList[currentFile-1] << ": " << lineCount << " " << wordCount << " " << charCount << "\n";
		totalCC += charCount;
		totalWC += wordCount;
		totalLC += lineCount;
		cout << "Totais: " << totalLC << " " << totalWC << " " << totalCC << "\n";
	} 
    else
	{
		// exibe as estatísticas para o caso de um único arquivo
		cout << "Chars: " << charCount << "\n" 
			 << "Words: " << wordCount << "\n" 
             << "Lines: " << lineCount << "\n";
	}
	
	fin.close();
	return 0;
}

/*
 * o analisador léxico chama yywrap para tratar EOF, 
 * permitindo, por exemplo, a abertura de outros aquivos,
 * exatamente o que é feito aqui neste exemplo
 */

int yyFlexLexer::yywrap()
{
	bool no_file = true;  // nenhum arquivo aberto

	// se não for o primeiro arquivo de uma sequência válida
	if ((currentFile != 0) && (nFiles > 1) && (currentFile < nFiles)) 
    {
		// exibe as estatísticas para o último arquivo processado 
		cout << fileList[currentFile-1] << ": " << lineCount << " " << wordCount << " " << charCount << "\n";
		totalCC += charCount;
		totalWC += wordCount;
		totalLC += lineCount;
		charCount = wordCount = lineCount = 0;
		fin.close();
	}

	// enquanto existir arquivos para processar, tentar abrir o próximo
	while (no_file & (fileList[currentFile] != nullptr)) 
    {
		fin.open(fileList[currentFile++]);
		if (fin.is_open()) 
        {
			no_file = false;
		}
		else
		{
			cout << "Não foi possível abrir arquivo " << fileList[currentFile-1] << "\n";
		}		
	}

	return (no_file ? 1 : 0);	/* 0 significa que tem mais entrada para processar */
}

