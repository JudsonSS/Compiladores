
enum NodeType
{
    UNKNOWN,
    STMT,
    EXPR,
    CONSTANT,
    IDENTIFIER,
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

struct Node
{
    int type;
    Node() : type(NodeType::UNKNOWN) {}
    Node(int t) : type(t) {}
};

struct Statement : public Node
{
    Statement() : Node(NodeType::STMT) {}
    Statement(int type) : Node(type) {}
};

struct Expression : public Node
{
    Token *token = nullptr;
    Expression(Token *t) : Node(NodeType::EXPR), token(t) {}
    Expression(int type, Token *t) : Node(type), token(t) {}
    ~Expression() { delete token; }
};

struct Constant : public Expression
{
    Constant(Token *t) : Expression(NodeType::CONSTANT, t) {}
};

struct Identifier : public Expression
{
    Identifier(Token *t) : Expression(NodeType::IDENTIFIER, t) {}
};

struct Logical : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Logical(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::LOG, t), expr1(e1), expr2(e2) {}
};

struct Relational : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Relational(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::REL, t), expr1(e1), expr2(e2) {}
};

struct Arithmetic : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Arithmetic(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::ARI, t), expr1(e1), expr2(e2) {}
};

struct UnaryExpr : public Expression
{
    Expression *expr;
    UnaryExpr(Token *t, Expression *e) : Expression(NodeType::UNARY, t), expr(e) {}
};

struct Seq : public Statement
{
    Statement *first;
    Statement *others;
    Seq(Statement *s, Statement *ss) : Statement(NodeType::SEQ), first(s), others(ss) {}
};

struct Assign : public Statement
{
    Token *id;
    Expression *expr;
    Assign(Token *i, Expression *e) : Statement(NodeType::ASSIGN), id(i), expr(e) {}
};

struct If : public Statement
{
    Expression *expr;
    Statement *stmt;
    If(Expression *e, Statement *s) : Statement(NodeType::IF_STMT), expr(e), stmt(s) {}
};

struct While : public Statement
{
    Expression *expr;
    Statement *stmt;
    While(Expression *e, Statement *s) : Statement(NodeType::WHILE_STMT), expr(e), stmt(s) {}
};

struct DoWhile : public Statement
{
    Statement *stmt;
    Expression *expr;
    DoWhile(Statement *s, Expression *e) : Statement(NodeType::DOWHILE_STMT), stmt(s), expr(e) {}
};