
#ifndef COMPILER_AST
#define COMPILER_AST

#include "lexer.h"

enum NodeType
{
    UNKNOWN,
    STMT,
    EXPR,
    CONSTANT,
    IDENTIFIER,
    ACCESS,
    LOG,
    REL,
    ARI,
    UNARY,
    SEQ,
    ASSIGN,
    IF_STMT,
    WHILE_STMT,
    DOWHILE_STMT
};

enum ExprType
{
    VOID,
    INT,
    FLOAT,
    BOOL
};

struct Node
{
    int node_type;
    Node();
    Node(int t);
};

struct Statement : public Node
{
    Statement();
    Statement(int type);
};

struct Expression : public Node
{
    int type;
    Token *token;
    Expression(Token *t);
    Expression(int ntype, int etype, Token *t);
    string Name();
    string Type();
};

struct Constant : public Expression
{
    Constant(int etype, Token *t);
};

struct Identifier : public Expression
{
    Identifier(int etype, Token *t);
};

struct Access : public Expression
{
    Expression * id;
    Expression * expr;
    Access(int etype, Token * t, Expression * i, Expression * e);
};

struct Logical : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Logical(Token *t, Expression *e1, Expression *e2);
};

struct Relational : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Relational(Token *t, Expression *e1, Expression *e2);
};

struct Arithmetic : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Arithmetic(int etype, Token *t, Expression *e1, Expression *e2);
};

struct UnaryExpr : public Expression
{
    Expression *expr;
    UnaryExpr(int etype, Token *t, Expression *e);
};

struct Seq : public Statement
{
    Statement *stmt;
    Statement *stmts;
    Seq(Statement *s, Statement *ss);
};

struct Assign : public Statement
{
    Expression *id;
    Expression *expr;
    Assign(Expression *i, Expression *e);
};

struct If : public Statement
{
    Expression *expr;
    Statement *stmt;
    If(Expression *e, Statement *s);
};

struct While : public Statement
{
    Expression *expr;
    Statement *stmt;
    While(Expression *e, Statement *s);
};

struct DoWhile : public Statement
{
    Statement *stmt;
    Expression *expr;
    DoWhile(Statement *s, Expression *e);
};

#endif