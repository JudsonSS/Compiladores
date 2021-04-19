%{
 
// ilustra utilização de estados no Flex:
// %x - estados exclusivos
// %s - estados não-exclusivos  

#include <iostream>
using std::cout;

int comments = 0, code = 0, whiteLine = 0;
%}

%option noyywrap
%x COMMENT

%%
 /* código ou comentário em linha única */
^[ \t]*\n					whiteLine++;                // linha em branco
^[ \t]*"/*".*"*/"[ \t]*\n 	comments++;                 // linha apenas com comentário 
.+"/*".*"*/".*\n			code++;                     // linha com código e comentário  
.*"/*".*"*/".+\n			code++;                     // linha com comentário e código  

 /* abertura de comentário em múltiplas linhas */
^[ \t]*"/*"					BEGIN COMMENT;              // abertura de comentário 
.+"/*".*\n					code++; BEGIN COMMENT;      // código e abertura de comentário 

 /* código em linhas sem comentário */
\n							code++;						// linha de código
.							;                           // ignora qualquer outro caractere 

 /* fechamento de comentário em múltiplas linhas */
<COMMENT>.*"*/"[ \t]*\n		BEGIN INITIAL; comments++;  // fim de linha de comentário 
<COMMENT>.*\n			    comments++;                 // linha de comentário

%%

int main()
{
	yyFlexLexer lexer;
	lexer.yylex();
	
	cout << "code: " << code << ", comments: " << comments << ", blank: " << whiteLine << "\n";
}

