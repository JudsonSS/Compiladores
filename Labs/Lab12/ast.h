
enum NodeType { UNKNOWN, STMT, EXPR, CONSTANT, IDENTIFIER, LOG, REL, ARI, UNARY, 
                SEQ, ASSIGN, IF_STMT, WHILE_STMT, DOWHILE_STMT };

class Node
{
private:
    int type;

public:
    Node() : type(NodeType::UNKNOWN) {}
    Node(int t) : type(t) {}
};

class Stmt : public Node
{
public:
    Stmt() : Node(NodeType::STMT) {}
};

class Expr : public Node
{
private:
    Token * token = nullptr;

public:
    Expr(Token * t) : Node(NodeType::EXPR), token(t) {}
    Expr(int type, Token * t) : Node(type), token(t) {}
    ~Expr() { delete token; }
};

class Constant : public Expr
{
public:
    Constant(Token * t) : Expr(NodeType::CONSTANT, t) {}
};

class Identifier : public Expr
{
public:
    Identifier(Token * t) : Expr(NodeType::IDENTIFIER, t) {}
};

class Logical : public Expr
{
private:
    Expr * expr1;
    Expr * expr2;
    
public:
    Logical(Token * t, Expr * e1, Expr * e2) : Expr(NodeType::LOG, t), expr1(e1), expr2(e2) {}
};

class Relational : public Expr
{
private:
    Expr * expr1;
    Expr * expr2;

public:
    Relational(Token * t, Expr * e1, Expr * e2) : Expr(NodeType::REL, t), expr1(e1), expr2(e2) {}
};

class Arithmetic : public Expr
{
private:
    Expr * expr1;
    Expr * expr2;

public:
    Arithmetic(Token * t, Expr * e1, Expr * e2) : Expr(NodeType::ARI, t), expr1(e1), expr2(e2) {}
};

class UnaryExpr : public Expr
{
public:
    Expr * expr;

public:
    UnaryExpr(Token * t, Expr * e) : Expr(NodeType::UNARY, t), expr(e) {}
};

class Seq : public Stmt
{
private:
    Stmt * stmt;
    Stmt * stmts;

public:
    Seq(Stmt * s, Stmt * ss) {}
};

class Assign : public Stmt
{
private:
    Token * id;
    Expr * expr;

public:
    Assign(Token * i, Expr * e) { }
};

class If : public Stmt
{
private:
    Expr * expr;
    Stmt * stmt;

public:
    If(Expr * e, Stmt * s) { }
};

class While : public Stmt
{
private:
    Expr * expr;
    Stmt * stmt;

public:
    While(Expr * e, Stmt * s) { }
    
};

class DoWhile : public Stmt
{
private:
    Stmt * stmt;
    Expr * expr;    

public:
    DoWhile(Stmt * s, Expr * e) { }
    
};