#include <string>
using std::string;

// código numérico dos Tokens
enum Tag { NUM = 256, ID, LOG, BIT };

// analisador léxico
class Lexer
{
private:
	int  line = 1;
	char peek = ' ';

public:
	Lexer();
	
	int Scan();
	void Start();
};
