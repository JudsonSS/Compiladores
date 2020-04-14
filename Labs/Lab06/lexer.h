#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

// cada token deve possuir uma tag (número a partir de 256)
// a tag dos caracteres individuais é seu código ASCII
enum Tag { NUM = 256, ID, TRUE, FALSE };

// classes para representar tokens
struct Token
{
	int tag;
	Token(int t) : tag(t) {}
};

struct Num : public Token
{
	int value;
	Num(int v) : Token(Tag::NUM), value(v) {}
};

struct Id : public Token
{
	string name;
	Id(): Token(0) {}
	Id(int t, string s) : Token(t), name(s) {}
};


// analisador léxico
class Lexer
{
private:
	int  line = 1;
	char peek = ' ';
	unordered_map<string, Id> id_table;

public:
	Lexer();
	
	Token Scan();
	void Start();
};
