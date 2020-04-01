#include <unordered_map>
#include <string>
#include <sstream>
using std::stringstream;
using std::unordered_map;
using std::string;

// cada token possui uma tag (número a partir de 256)
// a tag de caracteres individuais é seu código ASCII
enum Tag { PROGRAM = 256, IF, WHILE, DO, TYPE, INTEGRAL, FLOATING, ID, EQ, NEQ, LTE, GTE, TRUE, FALSE };

// classes para representar tokens
struct Token
{
	int tag;
	Token() : tag(0) {}
	Token(int t) : tag(t) {}
	virtual string toString() { stringstream ss; ss << char(tag); return ss.str(); }
};

struct Integral : public Token
{
	int value;
	Integral(): Token(Tag::INTEGRAL), value(0) {}
	Integral(int v) : Token(Tag::INTEGRAL), value(v) {}
	virtual string toString() { stringstream ss; ss << value; return ss.str(); }
};

struct Floating : public Token
{
	float value;
	Floating(): Token(Tag::FLOATING), value(0.0f) {}
	Floating(float v) : Token(Tag::FLOATING), value(v) {}
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
	// tipos de token da linguagem
	struct
	{
		Token t;
		Integral i;
		Floating f;
		Id id;
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
