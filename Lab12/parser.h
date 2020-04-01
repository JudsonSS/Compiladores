#include "lexer.h"
#include "symtable.h"
#include "ast.h"

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
	Expr * Local();
	Expr * Bool();
	Expr * Join();
	Expr * Equality();
	Expr * Rel();
	Expr * Ari();
	Expr * Term();
	Expr * Unary();
	Expr * Factor();
	bool Match(int tag);

public:
	Parser();
	void Start();
};
