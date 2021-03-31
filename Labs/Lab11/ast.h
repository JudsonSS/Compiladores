
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
    Node() : node_type(NodeType::UNKNOWN) {}
    Node(int t) : node_type(t) {}
};

struct Statement : public Node
{
    Statement() : Node(NodeType::STMT) {}
    Statement(int type) : Node(type) {}
};

struct Expression : public Node
{
    int type;
    Token * token = nullptr;
    
    Expression(Token *t) : Node(NodeType::EXPR), type(ExprType::VOID), token(t) {}
    Expression(int ntype, int etype, Token * t) : Node(ntype), type(etype), token(t) {}
    
    string TypeName() 
    { 
        switch(type)
        {
            case ExprType::INT: return "int"; break;
            case ExprType::FLOAT: return "float"; break;
            case ExprType::BOOL: return "bool"; break;
            default: return "void";
        }        
    }
};

struct Constant : public Expression
{
    Constant(int etype, Token * t) : Expression(NodeType::CONSTANT, etype, t) {}
};

struct Identifier : public Expression
{
    Identifier(int etype, Token * t) : Expression(NodeType::IDENTIFIER, etype, t) {}
};

struct Access : public Expression
{
    Expression * id;
    Expression * expr;
    Access(int etype, Token * t, Expression * i, Expression * e) : Expression(NodeType::ACCESS, etype, t), id(i), expr(e) {}
};

struct Logical : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Logical(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::LOG, ExprType::BOOL, t), expr1(e1), expr2(e2) {}
};

struct Relational : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Relational(Token *t, Expression *e1, Expression *e2) : Expression(NodeType::REL, ExprType::BOOL, t), expr1(e1), expr2(e2) {}
};

struct Arithmetic : public Expression
{
    Expression *expr1;
    Expression *expr2;
    Arithmetic(int etype, Token *t, Expression *e1, Expression *e2) : Expression(NodeType::ARI, etype, t), expr1(e1), expr2(e2) {}
};

struct UnaryExpr : public Expression
{
    Expression *expr;
    UnaryExpr(int etype, Token *t, Expression *e) : Expression(NodeType::UNARY, etype, t), expr(e) {}
};

struct Seq : public Statement
{
    Statement *stmt;
    Statement *stmts;
    Seq(Statement *s, Statement *ss) : Statement(NodeType::SEQ), stmt(s), stmts(ss) {}
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