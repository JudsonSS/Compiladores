#include <unordered_map>
#include <string>
#include <sstream>
using std::stringstream;
using std::unordered_map;
using std::string;

// cada token possui uma tag (número a partir de 256)
// a tag de caracteres individuais é seu código ASCII
enum Tag { NUM = 256, ID, TRUE, FALSE };

// classes para representar tokens
struct Token
{
	int tag;
	Token() : tag(0) {}
	Token(int t) : tag(t) {}
	virtual string toString() { stringstream ss; ss << char(tag); return ss.str(); }
};

struct Num : public Token
{
	int value;
	Num(): Token(Tag::NUM), value(0) {}
	Num(int v) : Token(Tag::NUM), value(v) {}
	virtual string toString() { stringstream ss; ss << value; return ss.str(); }
};

struct Id : public Token
{
	string name;
	Id(): Token(Tag::ID) {}
	Id(int t, string s) : Token(t), name(s) {}
	virtual string toString() { return name; }
};

// analisador léxico
class Lexer
{
private:
	// guarda último token retornado
	struct
	{
		Token t;
		Num n;
		Id i;
	} 
	token;
	
	int line = 1;
	char peek;
	unordered_map<string, Id> id_table;

public:
	Lexer();
	int Lineno();
	Token * Scan();
};
