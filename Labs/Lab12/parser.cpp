#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

Node *Parser::Program()
{
    // program -> int main() block
    if (!Match(Tag::TYPE))
        throw SyntaxError(scanner.Lineno(), "\'int\' esperado");

    if (!Match(Tag::MAIN))
        throw SyntaxError(scanner.Lineno(), "\'main\' esperado");

    if (!Match('('))
        throw SyntaxError(scanner.Lineno(), "\'(\' esperado");

    if (!Match(')'))
        throw SyntaxError(scanner.Lineno(), "\')\' esperado");

    return Block();
}

Statement *Parser::Block()
{
    // block -> { decls stmts }
    if (!Match('{'))
        throw SyntaxError(scanner.Lineno(), "\'{\' esperado");

    // ------------------------------------
    // nova tabela de símbolos para o bloco
    // ------------------------------------
    SymTable *saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    Decls();
    Statement *sts = Stmts();

    if (!Match('}'))
        throw SyntaxError(scanner.Lineno(), "\'}\' esperado");

    // ------------------------------------------------------
    // tabela do escopo envolvente volta a ser a tabela ativa
    // ------------------------------------------------------
    delete symtable;
    symtable = saved;
    // ------------------------------------------------------

    return sts;
}

void Parser::Decls()
{
    // decls -> decl decls
    //        | empty
    // decl  -> type id;

    while (lookahead->tag == Tag::TYPE)
    {
        // captura nome do tipo
        string type{lookahead->ToString()};
        Match(Tag::TYPE);

        // captura nome do identificador
        string name{lookahead->ToString()};
        Match(Tag::ID);

        // cria símbolo
        Symbol s{name, type};

        // insere variável na tabela de símbolos
        if (!symtable->Insert(name, s))
        {
            // a inserção falha quando a variável já está na tabela
            stringstream ss;
            ss << "variável \"" << name << "\" já definida";
            throw SyntaxError(scanner.Lineno(), ss.str());
        }

        // verifica se é uma declaração de arranjo
        if (Match('['))
        {
            if (!Match(Tag::NUM_INT))
            {
                stringstream ss;
                ss << "o índice de um arranjo deve ser um valor inteiro";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->ToString() << "\'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
        }

        // verififica ponto e vírgula
        if (!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->ToString() << "\' no lugar de ';'";
            throw SyntaxError{scanner.Lineno(), "esperado ;"};
        }
    }
}

Statement *Parser::Stmts()
{
    // stmts -> stmt stmts
    //        | empty

    Statement *seq = nullptr;

    switch (lookahead->tag)
    {
    // stmts -> stmt stmts
    case Tag::ID:
    case Tag::IF:
    case Tag::WHILE:
    case Tag::DO:
    case '{':
    {
        Statement *st = Stmt();
        Statement *sts = Stmts();
        seq = new Seq(st, sts);
    }
    }

    // stmts -> empty
    return seq;
}

Statement *Parser::Stmt()
{
    // stmt  -> local = bool;
    //        | if (bool) stmt
    //        | while (bool) stmt
    //        | do stmt while (bool);
    //        | block

    Statement *stmt = nullptr;

    switch (lookahead->tag)
    {
    // stmt -> local = bool;
    case Tag::ID:
    {
        Expression *left = Local();
        if (!Match('='))
        {
            stringstream ss;
            ss << "esperado = no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *right = Bool();
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }

        // -----------------------------------------
        // Verificação de Tipos
        // -----------------------------------------
        if (left->type != right->type )
        {
            stringstream ss;
            ss << "\'=\' usado com operandos de tipos diferentes ("
               << left->token->ToString()
               << ":" << left->TypeName() << ") ("
               << right->token->ToString()
               << ":" << right->TypeName() << ") ";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        // -----------------------------------------

        stmt = new Assign(left->token, right);
        return stmt;
    }

    // stmt -> if (bool) stmt
    case Tag::IF:
    {
        Match(Tag::IF);
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Statement *inst = Stmt();
        stmt = new If(cond, inst);
        return stmt;
    }

    // stmt -> while (bool) stmt
    case Tag::WHILE:
    {
        Match(Tag::WHILE);
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Statement *inst = Stmt();
        stmt = new While(cond, inst);
        return stmt;
    }

    // stmt -> do stmt while (bool);
    case Tag::DO:
    {
        Match(Tag::DO);
        Statement *inst = Stmt();
        if (!Match(Tag::WHILE))
        {
            stringstream ss;
            ss << "esperado \'while\' no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        stmt = new DoWhile(inst, cond);
        return stmt;
    }
    // stmt -> block
    case '{':
    {
        stmt = Block();
        return stmt;
    }
    default:
    {
        stringstream ss;
        ss << "\'" << lookahead->ToString() << "\' não inicia uma instrução válida";
        throw SyntaxError{scanner.Lineno(), ss.str()};
    }
    }
}

Expression *Parser::Local()
{
    // local -> local[bool]
    //        | id

    Expression *expr = nullptr;

    switch (lookahead->tag)
    {
    case Tag::ID:
    {
        // verifica tipo da variável na tabela de símbolos
        Symbol *s = symtable->Find(lookahead->ToString());
        if (!s)
        {
            stringstream ss;
            ss << "variável \"" << lookahead->ToString() << "\" não declarada";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }

        // identifica o tipo da expressão
        int etype = ExprType::VOID;
        if (s->type == "int")
            etype = ExprType::INT;
        else if (s->type == "float")
            etype = ExprType::INT;
        else if (s->type == "bool")
            etype = ExprType::BOOL;


        expr = new Identifier(etype, new Token{*lookahead});
        Match(Tag::ID);

        if (Match('['))
        {
            Bool();
            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->ToString() << "\'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
        }
        break;
    }
    default:
    {
        stringstream ss;
        ss << "esperado um local de armazenamento (variável ou arranjo)";
        throw SyntaxError{scanner.Lineno(), ss.str()};
    }
    }

    return expr;
}

Expression *Parser::Bool()
{
    // bool -> join lor
    // lor  -> || join lor
    //       | empty

    Expression *expr1 = Join();

    // função Lor()
    while (true)
    {
        Token t = *lookahead;

        if (Match(Tag::OR))
        {
            Expression *expr2 = Join();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != ExprType::BOOL || expr2->type != ExprType::BOOL)
            {
                stringstream ss;
                ss << "\'||\' usado com operandos não booleanos ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Logical(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Join()
{
    // join -> equality land
    // land -> && equality land
    //       | empty

    Expression *expr1 = Equality();

    // função Land()
    while (true)
    {
        Token t = *lookahead;
        if (Match(Tag::AND))
        {
            Expression *expr2 = Equality();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != ExprType::BOOL || expr2->type != ExprType::BOOL)
            {
                stringstream ss;
                ss << "\'&&\' usado com operandos não booleanos ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Logical(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Equality()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty

    Expression *expr1 = Rel();

    // função Eqdif()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == Tag::EQ)
        {
            Match(Tag::EQ);
            Expression *expr2 = Rel();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'==\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::NEQ)
        {
            Match(Tag::NEQ);
            Expression *expr2 = Rel();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'!=\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Rel()
{
    // rel  -> ari comp
    // comp -> < ari comp
    //       | <= ari comp
    //       | > ari comp
    //       | >= ari comp
    //       | empty

    Expression *expr1 = Ari();

    // função Comp()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == '<')
        {
            Match('<');
            Expression *expr2 = Ari();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'<\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::LTE)
        {
            Match(Tag::LTE);
            Expression *expr2 = Ari();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'<=\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == '>')
        {
            Match('>');
            Expression *expr2 = Ari();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'>\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else if (lookahead->tag == Tag::GTE)
        {
            Match(Tag::GTE);
            Expression *expr2 = Ari();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'>=\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Relational(new Token{t}, expr1, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr1;
}

Expression *Parser::Ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Expression *expr1 = Term();

    // função Oper()
    while (true)
    {
        Token t = *lookahead;

        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Expression *expr2 = Term();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'+\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Expression *expr2 = Term();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'-\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // oper -> empty
        else
            break;
    }

    return expr1;
}

Expression *Parser::Term()
{
    // term -> unary calc
    // calc -> * unary calc
    //       | / unary calc
    //       | empty

    Expression *expr1 = Unary();

    // função Calc()
    while (true)
    {
        Token t = *lookahead;

        // calc -> * unary calc
        if (lookahead->tag == '*')
        {
            Match('*');
            Expression *expr2 = Unary();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'*\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // calc -> / unary calc
        else if (lookahead->tag == '/')
        {
            Match('/');
            Expression *expr2 = Unary();

            // -----------------------------------------
            // Verificação de Tipos
            // -----------------------------------------
            if (expr1->type != expr2->type)
            {
                stringstream ss;
                ss << "\'-\' usado com operandos de tipos diferentes ("
                   << expr1->token->ToString()
                   << ":" << expr1->TypeName() << ") ("
                   << expr2->token->ToString()
                   << ":" << expr2->TypeName() << ") ";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
            // -----------------------------------------

            expr1 = new Arithmetic(expr1->type, new Token{t}, expr1, expr2);
        }
        // calc -> empty
        else
            break;
    }

    return expr1;
}

Expression *Parser::Unary()
{
    // unary -> !unary
    //        | -unary
    //        | factor

    Expression *unary = nullptr;

    // unary -> !unary
    if (lookahead->tag == '!')
    {
        Token t = *lookahead;
        Match('!');
        Expression *expr = Unary();

        // -----------------------------------------
        // Verificação de Tipos
        // -----------------------------------------
        if (expr->type != ExprType::BOOL)
        {
            stringstream ss;
            ss << "\'!\' usado com operando não booleano ("
               << expr->token->ToString()
               << ":" << expr->TypeName() << ")";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        // -----------------------------------------

        unary = new UnaryExpr(ExprType::BOOL, new Token{t}, expr);
    }
    // unary -> -unary
    else if (lookahead->tag == '-')
    {
        Token t = *lookahead;
        Match('-');
        Expression *expr = Unary();

        // -----------------------------------------
        // Verificação de Tipos
        // -----------------------------------------
        if (expr->type != ExprType::INT && expr->type != ExprType::FLOAT)
        {
            stringstream ss;
            ss << "\'-unário\' usado com operando não numérico ("
               << expr->token->ToString()
               << ":" << expr->TypeName() << ")";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        // -----------------------------------------

        unary = new UnaryExpr(expr->type, new Token{t}, expr);
    }
    else
    {
        unary = Factor();
    }

    return unary;
}

Expression *Parser::Factor()
{
    // factor -> (bool)
    //         | local
    //         | integer
    //         | floating
    //         | true
    //         | false

    Expression *expr = nullptr;

    switch (lookahead->tag)
    {
    // factor -> (bool)
    case '(':
    {
        Match('(');
        expr = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->ToString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        break;
    }

    // factor -> local
    case Tag::ID:
    {
        expr = Local();
        break;
    }

    // factor -> integer
    case Tag::NUM_INT:
    {
        expr = new Constant(ExprType::INT, new Token{*lookahead});
        Match(Tag::NUM_INT);
        break;
    }

    // factor -> floating
    case Tag::NUM_FLOAT:
    {
        expr = new Constant(ExprType::FLOAT, new Token{*lookahead});
        Match(Tag::NUM_FLOAT);
        break;
    }

    // factor -> true
    case Tag::TRUE:
    {
        expr = new Constant(ExprType::BOOL, new Token{*lookahead});
        Match(Tag::TRUE);
        break;
    }

    // factor -> false
    case Tag::FALSE:
    {
        expr = new Constant(ExprType::BOOL, new Token{*lookahead});
        Match(Tag::FALSE);
        break;
    }

    default:
    {
        stringstream ss;
        ss << "uma expressão é esperada no lugar de  \'" << lookahead->ToString() << "\'";
        throw SyntaxError{scanner.Lineno(), ss.str()};
        break;
    }
    }

    return expr;
}

bool Parser::Match(int tag)
{
    if (tag == lookahead->tag)
    {
        lookahead = scanner.Scan();
        return true;
    }

    return false;
}

Parser::Parser()
{
    lookahead = scanner.Scan();
    symtable = nullptr;
}

void Parser::Start()
{
    Traverse(Program());
    cout << endl;
}

void Pad(int n)
{
    for (int i = 0; i < n; ++i)
        cout << ' ';
}

void Parser::Traverse(Node *n)
{
    static int padding = 0;

    if (n)
    {
        switch (n->node_type)
        {
        case SEQ:
        {
            Seq *s = (Seq *)n;
            Pad(padding);
            cout << "<SEQ>\n";
            padding += 2;
            Traverse(s->stmt);
            Traverse(s->stmts);
            padding -= 2;
            Pad(padding);
            cout << "</SEQ>\n";
            break;
        }
        case ASSIGN:
        {
            Assign *a = (Assign *)n;
            Pad(padding);
            cout << "<ASSIGN>\n";
            padding += 2;
            Pad(padding);
            cout << a->id->ToString() << endl;
            Traverse(a->expr);
            padding -= 2;
            Pad(padding);
            cout << "</ASSIGN>\n";
            break;
        }
        case REL:
        {
            Relational *r = (Relational *)n;
            Pad(padding);
            cout << "<REL>\n";
            padding += 2;
            Traverse(r->expr1);
            Pad(padding);
            cout << r->token->ToString() << endl;
            Traverse(r->expr2);
            padding -= 2;
            Pad(padding);
            cout << "</REL>\n";
            break;
        }
        case LOG:
        {
            Logical *l = (Logical *)n;
            Pad(padding);
            cout << "<LOG>\n";
            padding += 2;
            Traverse(l->expr1);
            Pad(padding);
            cout << l->token->ToString() << endl;
            Traverse(l->expr2);
            padding -= 2;
            Pad(padding);
            cout << "</LOG>\n";
            break;
        }
        case ARI:
        {
            Arithmetic *a = (Arithmetic *)n;
            Pad(padding);
            cout << "<ARI>\n";
            padding += 2;
            Traverse(a->expr1);
            Pad(padding);
            cout << a->token->ToString() << endl;
            Traverse(a->expr2);
            padding -= 2;
            Pad(padding);
            cout << "</ARI>\n";
            break;
        }
        case CONSTANT:
        {
            Constant *c = (Constant *)n;
            Pad(padding);
            cout << c->token->ToString() << endl;
            break;
        }
        case IDENTIFIER:
        {
            Identifier *i = (Identifier *)n;
            Pad(padding);
            cout << i->token->ToString() << endl;
            break;
        }
        case IF_STMT:
        {
            If *i = (If *)n;
            Pad(padding);
            cout << "<IF>\n";
            padding += 2;
            Traverse(i->expr);
            Traverse(i->stmt);
            padding -= 2;
            break;
        }
        case WHILE_STMT:
        {
            While *w = (While *)n;
            Pad(padding);
            cout << "<WHILE>\n";
            padding += 2;
            Traverse(w->expr);
            Traverse(w->stmt);
            padding -= 2;
            break;
        }
        case DOWHILE_STMT:
        {
            DoWhile *dw = (DoWhile *)n;
            Pad(padding);
            cout << "<DOWHILE>\n";
            padding += 2;
            Traverse(dw->stmt);
            Traverse(dw->expr);
            padding -= 2;
            break;
        }
        }
    }
    else
    {
        Pad(padding);
        cout << "NULL\n";
    }
}
