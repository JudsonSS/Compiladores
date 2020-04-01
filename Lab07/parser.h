#include "lexer.h"

class Parser
{
private:
	Lexer scanner;
	Token * lookahead;
	
	void Expr();
	void Term();
	void Fact();
	bool Match(int tag);

public:
	Parser();
	void Start();
};
