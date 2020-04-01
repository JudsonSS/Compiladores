#include <FlexLexer.h>

class Parser
{
private:
	yyFlexLexer scanner;
	int lookahead;
	
public:
	void Start();
};
