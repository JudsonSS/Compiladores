#include "lexer.h"
#include "symtable.h"
#include "ast.h"

class Parser
{
private:
	Lexer scanner;
	SymTable * symtable;
	Token * lookahead;
	
	Node * Program();
	Statement * Block();
	void Decls();
	void Decl();
	Statement * Stmts();
	Statement * Stmt();
	Expression * Local();
	Expression * Bool();
	Expression * Join();
	Expression * Equality();
	Expression * Rel();
	Expression * Ari();
	Expression * Term();
	Expression * Unary();
	Expression * Factor();
	bool Match(int tag);
	void Traverse(Node * n);

public:
	Parser();
	void Start();
};
