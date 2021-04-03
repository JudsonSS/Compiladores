#include <iostream>
#include <sstream>
#include "ast.h"
#include "error.h"
#include "generator.h"
using std::stringstream;
using std::cout;
using std::endl;

extern Lexer * scanner;

// ----
// Node
// ----

unsigned Node::labels = 0;
Node::Node() : node_type(NodeType::UNKNOWN) {}
Node::Node(int t) : node_type(t) {}
void Node::Gen() {}
string Node::ToString() { return ""; }
unsigned Node::NewLabel() { return ++labels; }
void Node::PrintLabel(unsigned i) { cout << 'L' << i << ':'; }
void Node::Print(string s) { cout << "\t" << s << endl; }

// ---------
// Statement
// ---------

Statement::Statement() : Node(NodeType::STMT) {}
Statement::Statement(int type) : Node(type) {}

// ----------
// Expression
// ----------

Expression::Expression(Token *t) : Node(NodeType::EXPR), type(ExprType::VOID), token(t) {}
Expression::Expression(int ntype, int etype, Token *t) : Node(ntype), type(etype), token(t) {}

string Expression::ToString()
{
    return token->lexeme;
}

string Expression::Type()
{
    switch (type)
    {
    case ExprType::INT:
        return "int";
        break;
    case ExprType::FLOAT:
        return "float";
        break;
    case ExprType::BOOL:
        return "bool";
        break;
    default:
        return "void";
    }
}

// ----
// Temp
// ----

int Temp::count = 0;

Temp::Temp(int etype) : Expression(NodeType::TEMP, etype, nullptr), number(++count)
{
}

string Temp::ToString()
{
    stringstream ss;
    ss << "t" << number;
    return ss.str();
}


// --------
// Constant
// --------

Constant::Constant(int etype, Token *t) : Expression(NodeType::CONSTANT, etype, t) {}

// ----------
// Identifier
// ----------

Identifier::Identifier(int etype, Token *t) : Expression(NodeType::IDENTIFIER, etype, t) {}

// ------
// Access
// ------

Access::Access(int etype, Token * t, Expression * i, Expression * e): Expression(NodeType::ACCESS, etype, t), id(i), expr(e) {}

string Access::ToString()
{
    stringstream ss;
    ss << id->ToString() << "[" << expr->ToString() << "]";
    return ss.str();
}

// -------
// Logical
// -------

Logical::Logical(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::LOG, ExprType::BOOL, t), expr1(e1), expr2(e2)
{
    // verificação de tipos
    if (expr1->type != ExprType::BOOL || expr2->type != ExprType::BOOL)
    {
        stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos não booleanos ("
           << expr1->ToString() << ":" << expr1->Type() << ") ("
           << expr2->ToString() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Relational
// ----------

Relational::Relational(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::REL, ExprType::BOOL, t), expr1(e1), expr2(e2)
{
    // verificação de tipos
    if (expr1->type != expr2->type)
    {
        stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos de tipos diferentes ("
           << expr1->ToString() << ":" << expr1->Type() << ") ("
           << expr2->ToString() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----------
// Arithmetic
// ----------

Arithmetic::Arithmetic(int etype, Token *t, Expression *e1, Expression *e2) : Expression(NodeType::ARI, etype, t), expr1(e1), expr2(e2)
{
    // verificação de tipos
    if (expr1->type != expr2->type)
    {
        stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operandos de tipos diferentes ("
           << expr1->ToString() << ":" << expr1->Type() << ") ("
           << expr2->ToString() << ":" << expr2->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ---------
// UnaryExpr
// ---------

UnaryExpr::UnaryExpr(int etype, Token *t, Expression *e) : Expression(NodeType::UNARY, etype, t), expr(e)
{
    // verificação de tipos
    if (expr->type != ExprType::BOOL)
    {
        stringstream ss;
        ss << "\'" << token->lexeme << "\' usado com operando não booleano ("
           << expr->ToString() << ":" << expr->Type() << ")";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

// ----
// Seq
// ----

Seq::Seq(Statement *s, Statement *ss) : Statement(NodeType::SEQ), stmt(s), stmts(ss) {}

void Seq::Gen()
{
    stmt->Gen();

    if (stmts)
        stmts->Gen();
}

// ------
// Assign
// ------

Assign::Assign(Expression *i, Expression *e) : Statement(NodeType::ASSIGN), id(i), expr(e)
{
    // verificação de tipos
    if (id->type != expr->type)
    {
        stringstream ss;
        ss << "\'=\' usado com operandos de tipos diferentes ("
           << id->ToString() << ":" << id->Type() << ") ("
           << expr->ToString() << ":" << expr->Type() << ") ";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

void Assign::Gen()
{
    Expression * left = Lvalue(id);
    Expression * right = Rvalue(expr);
    cout << left->ToString() << " = " << right->ToString() << endl;
}

// ----
// If
// ----

If::If(Expression *e, Statement *s) : Statement(NodeType::IF_STMT), expr(e), stmt(s) 
{
    // verificação de tipos
    if (expr->type != ExprType::BOOL)
    {
        stringstream ss;
        ss << "expressão condicional \'" << expr->ToString() << "\' não booleana";
        throw SyntaxError{scanner->Lineno(), ss.str()};   
    }    
}

void If::Gen()
{
    Expression * n = Rvalue(expr);
    cout << "ifFalse " << n->ToString() << " goto " << "after\n";
    stmt->Gen();
    cout << "after" << ":" << endl;
}

// -----
// While
// -----

While::While(Expression *e, Statement *s) : Statement(NodeType::WHILE_STMT), expr(e), stmt(s) {}

// --------
// Do-While
// --------

DoWhile::DoWhile(Statement *s, Expression *e) : Statement(NodeType::DOWHILE_STMT), stmt(s), expr(e) {}