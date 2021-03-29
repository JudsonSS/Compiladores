#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

Statement * Parser::Program()
{
    // program -> main block
    if (!Match(Tag::MAIN))
        throw SyntaxError(scanner.Lineno(), "\'main\' esperado");
    return Block();
}

Statement * Parser::Block()
{
    // block -> { decls stmts }
    if (!Match('{'))
        throw SyntaxError(scanner.Lineno(), "\'{\' esperado");

    // nova tabela de símbolos para o bloco
    // ------------------------------------
    SymTable *saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    Decls();
    Statement *sts = Stmts();

    if (!Match('}'))
        throw SyntaxError(scanner.Lineno(), "\'}\' esperado");

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
        string type{lookahead->toString()};
        Match(Tag::TYPE);

        // verifica se é um tipo válido
        if (type != "int" && type != "float")
        {
            stringstream ss;
            ss << "tipo " << type << " inválido";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }

        // captura nome do identificador
        string name{lookahead->toString()};
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
                ss << "esperado ] no lugar de  \'" << lookahead->toString() << "\'";
                throw SyntaxError{scanner.Lineno(), ss.str()};
            }
        }

        // verififica ponto e vírgula
        if (!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
            throw SyntaxError{scanner.Lineno(), "esperado ;"};
        }
    }
}

Statement *Parser::Stmts()
{
    // stmts -> stmt stmts
    //        | empty

    Statement * seq = nullptr;

    switch (lookahead->tag)
    {
    // stmts -> stmt stmts
    case Tag::ID:
    case Tag::IF:
    case Tag::WHILE:
    case Tag::DO:
    case '{':
    {
        Statement * st = Stmt();
        Statement * sts = Stmts();
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
            ss << "esperado = no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *right = Bool();
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        // sai da função Stmt()
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
            ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Statement *inst = Stmt();

        // sai da função Stmt()
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
            ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Statement *inst = Stmt();

        // sai da função Stmt()
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
            ss << "esperado \'while\' no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        if (!Match('('))
        {
            stringstream ss;
            ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        Expression *cond = Bool();
        if (!Match(')'))
        {
            stringstream ss;
            ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }
        if (!Match(';'))
        {
            stringstream ss;
            ss << "esperado ; no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }

        // sai da função Stmt()
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
        ss << "\'" << lookahead->toString() << "\' não inicia uma instrução válida";
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
        Symbol *s = symtable->Find(lookahead->toString());
        if (!s)
        {
            stringstream ss;
            ss << "variável \"" << lookahead->toString() << "\" não declarada";
            throw SyntaxError{scanner.Lineno(), ss.str()};
        }

        expr = new Identifier(new Token{*lookahead});
        Match(Tag::ID);

        if (Match('['))
        {
            Bool();
            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->toString() << "\'";
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

    Expression *expr = Join();

    // função Lor()
    while (true)
    {
        Token t = *lookahead;

        if (Match(Tag::OR))
        {
            Expression *expr2 = Join();
            expr = new Logical(new Token{t}, expr, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr;
}

Expression *Parser::Join()
{
    // join -> equality land
    // land -> && equality land
    //       | empty

    Expression *expr = Equality();

    // função Land()
    while (true)
    {
        Token t = *lookahead;
        if (Match(Tag::AND))
        {
            Expression *expr2 = Equality();
            expr = new Logical(new Token{t}, expr, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr;
}

Expression *Parser::Equality()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty

    Expression *expr = Rel();

    // função Eqdif()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == Tag::EQ)
        {
            Match(Tag::EQ);
            Expression *expr2 = Rel();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if (lookahead->tag == Tag::NEQ)
        {
            Match(Tag::NEQ);
            Expression *expr2 = Rel();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr;
}

Expression *Parser::Rel()
{
    // rel  -> ari comp
    // comp -> < ari comp
    //       | <= ari comp
    //       | > ari comp
    //       | >= ari comp
    //       | empty

    Expression *expr = Ari();

    // função Comp()
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == '<')
        {
            Match('<');
            Expression *expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if (lookahead->tag == Tag::LTE)
        {
            Match(Tag::LTE);
            Expression *expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if (lookahead->tag == '>')
        {
            Match('>');
            Expression *expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if (lookahead->tag == Tag::GTE)
        {
            Match(Tag::GTE);
            Expression *expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else
        {
            // empty
            break;
        }
    }

    return expr;
}

Expression *Parser::Ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Expression *expr = Term();

    // função Oper()
    while (true)
    {
        Token t = *lookahead;

        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Expression *expr2 = Term();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Expression *expr2 = Term();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        }
        // oper -> empty
        else
            break;
    }

    return expr;
}

Expression *Parser::Term()
{
    // term -> unary calc
    // calc -> * unary calc
    //       | / unary calc
    //       | empty

    Expression *expr = Unary();

    // função Calc()
    while (true)
    {
        Token t = *lookahead;

        // calc -> * unary calc
        if (lookahead->tag == '*')
        {
            Match('*');
            Expression *expr2 = Unary();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        }
        // calc -> / unary calc
        else if (lookahead->tag == '/')
        {
            Match('/');
            Expression *expr2 = Unary();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        }
        // calc -> empty
        else
            break;
    }

    return expr;
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
        unary = new UnaryExpr(new Token{t}, Unary());
    }
    // unary -> -unary
    else if (lookahead->tag == '-')
    {
        Token t = *lookahead;
        Match('-');
        unary = new UnaryExpr(new Token{t}, Unary());
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
            ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
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
        expr = new Constant(new Token{*lookahead});
        Match(Tag::NUM_INT);
        break;
    }

    // factor -> floating
    case Tag::NUM_FLOAT:
    {
        expr = new Constant(new Token{*lookahead});
        Match(Tag::NUM_FLOAT);
        break;
    }

    // factor -> true
    case Tag::TRUE:
    {
        expr = new Constant(new Token{*lookahead});
        Match(Tag::TRUE);
        break;
    }

    // factor -> false
    case Tag::FALSE:
    {
        expr = new Constant(new Token{*lookahead});
        Match(Tag::FALSE);
        break;
    }

    default:
    {
        stringstream ss;
        ss << "uma expressão é esperada no lugar de  \'" << lookahead->toString() << "\'";
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
    Statement * tree = Program();
}