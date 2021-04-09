
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
    DOWHILE_STMT,
    TEMP
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
    static unsigned labels;

    Node();
    Node(int t);
    virtual string ToString();
    unsigned NewLabel();
};

struct Statement : public Node
{
    Statement();
    Statement(int type);
    virtual void Gen();
};

struct Expression : public Node
{
    int type;
    Token * token;
        
    Expression(Token *t);
    Expression(int ntype, int etype, Token *t);
    virtual string ToString();
    string Type();
};

struct Temp : public Expression
{
    static int count;
    int number;
    Temp(int etype);
    string ToString();
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
    string ToString();
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
    void Gen();
};

struct Assign : public Statement
{
    Expression *id;
    Expression *expr;
    Assign(Expression *i, Expression *e);
    void Gen();
};

struct If : public Statement
{
    unsigned after;
    Expression *expr;
    Statement *stmt;
    If(Expression *e, Statement *s);
    void Gen();
};

struct While : public Statement
{
    Expression *expr;
    Statement *stmt;
    While(Expression *e, Statement *s);
    void Gen();
};

struct DoWhile : public Statement
{
    unsigned before;
    Statement *stmt;
    Expression *expr;
    DoWhile(Statement *s, Expression *e);
    void Gen();
};

#endif