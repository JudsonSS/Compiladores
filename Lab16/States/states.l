%{
/* 
 	Ilustra utilização de estados no Flex:
	%x - estados exclusivos
	%s - estados não-exclusivos  
*/

#include <iostream>
using std::cout;

int comments = 0, code = 0, whiteLine = 0;
%}

%option noyywrap
%x COMMENT

%%

^[ \t]*\n					whiteLine++;                // linha em branco
^[ \t]*"/*".*"*/"[ \t]*\n 	comments++;                 // linha apenas com comentário 
.+"/*".*"*/".*\n			code++;                     // linha com código e comentário  
.*"/*".*"*/".+\n			code++;                     // linha com comentário e código  
^[ \t]*"/*"					BEGIN COMMENT;              // abertura de comentário 
.+"/*".*\n					code++; BEGIN COMMENT;      // código e abertura de comentário 
\n							code++;						// linha de código
.							;                           // ignora qualquer outro caractere 

<COMMENT>.*"*/"[ \t]*\n		BEGIN INITIAL; comments++;  // fim de linha de comentário 
<COMMENT>"*/"				BEGIN INITIAL; 	            // fechamento de comentário
<COMMENT>.*\n			    comments++;                 // linha de comentário

%%

int main()
{
	yyFlexLexer lexer;
	lexer.yylex();
	
	cout << "code: " << code << ", comments: " << comments << ", blank: " << whiteLine << "\n";
}

