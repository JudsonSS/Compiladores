#include "lexer.h"
#include "symtable.h"

class Parser
{
private:
	Lexer scanner;
	SymTable * symtable;
	Token * lookahead;
	
	void Program();
	void Block();
	void Decls();
	void Decl();
	void Stmts();
	void Stmt();
	void Expr();
	void Rel();
	void Ari();
	void Term();
	void Factor();
	bool Match(int tag);

public:
	Parser();
	void Start();
};
