#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;

void Parser::Program()
{
    // program -> main block
    if (!Match(Tag::MAIN))
        throw SyntaxError(scanner.Lineno(), "\'main\' esperado");
    Block();
}

void Parser::Block()
{
    // block -> { decls stmts } 
    if (!Match('{'))
        throw SyntaxError(scanner.Lineno(), "\'{\' esperado");

    // nova tabela de símbolos para o bloco
    // ------------------------------------
    SymTable * saved = symtable;
    symtable = new SymTable(symtable);
    // ------------------------------------

    Decls();
    Stmts();

    if (!Match('}'))
        throw SyntaxError(scanner.Lineno(), "\'}\' esperado");

    // tabela do escopo envolvente volta a ser a tabela ativa
    // ------------------------------------------------------ 
    delete symtable;
    symtable = saved;
    // ------------------------------------------------------
}

void Parser::Decls()
{
    // decls -> decl decls
    //        | empty
    // decl  -> type id;

    while (lookahead->tag == Tag::TYPE)
    {
        // captura nome do tipo
        string type {lookahead->toString()};
        Match(Tag::TYPE);
        
        // verifica se é um tipo válido
        if (type != "int" && type != "float")
        {
            stringstream ss;
            ss << "tipo " << type << " inválido";
            throw SyntaxError { scanner.Lineno(), ss.str() };
        }

        // captura nome do identificador
        string name {lookahead->toString()};
        Match(Tag::ID);

        // cria símbolo
        Symbol s { name, type };
        
        // insere variável na tabela de símbolos
        if(!symtable->Insert(name, s))
        {
            // a inserção falha quando a variável já está na tabela
            stringstream ss;
            ss << "variável \"" << name << "\" já definida";
            throw SyntaxError(scanner.Lineno(), ss.str());    
        }

        // verifica se é uma declaração de arranjo
        if (Match('['))
        {
            if (!Match(Tag::INTEGER))
            {
                stringstream ss;
                ss << "o índice de um arranjo deve ser um valor inteiro";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
            if (!Match(']'))
            {
                stringstream ss;
                ss << "esperado ] no lugar de  \'" << lookahead->toString() << "\'";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
        }

        // verififica ponto e vírgula 
        if(!Match(';'))
        {
            stringstream ss;
            ss << "encontrado \'" << lookahead->toString() << "\' no lugar de ';'";
            throw SyntaxError { scanner.Lineno(), "esperado ;" };
        }

    }
}

void Parser::Stmts()
{
    // stmts -> stmt stmts
    //        | empty

    while(true)
    {

        switch(lookahead->tag)
        {
            // stmts -> stmt stmts
            case Tag::ID:
            case Tag::IF:
            case Tag::WHILE:
            case Tag::DO:
            case '{':
                Stmt();
                break;
            // stmts -> empty
            default:
                return;
        }
    }    
}

void Parser::Stmt()
{
    
    // stmt  -> local = bool;
    //        | if (bool) stmt
    //        | while (bool) stmt
    //        | do stmt while (bool);
    //        | block
    
    while(true)
    {
        switch(lookahead->tag)
        {
            // stmt -> local = bool; 
            case Tag::ID:
            { 
                Local();
                if(!Match('='))
                {
                    stringstream ss;
                    ss << "esperado = no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Bool();
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "esperado ; no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                // sai da função Stmt()
                return;
            }

            // stmt -> if (bool) stmt
            case Tag::IF:
            {
                Match(Tag::IF);
                if (!Match('('))
                {
                    stringstream ss;
                    ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Bool();
                if (!Match(')'))
                {
                    stringstream ss;
                    ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                // se mantém na função Stmt()
                break;
            }

            // stmt -> while (bool) stmt
            case Tag::WHILE:
            {
                Match(Tag::WHILE);
                if (!Match('('))
                {
                    stringstream ss;
                    ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Bool();
                if (!Match(')'))
                {
                    stringstream ss;
                    ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                // se mantém na função Stmt()
                break;
            }
            case Tag::DO:
            {
                Match(Tag::DO);
                Stmt();
                if (!Match(Tag::WHILE))
                {
                    stringstream ss;
                    ss << "esperado \'while\' no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                if (!Match('('))
                {
                    stringstream ss;
                    ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Bool();
                if (!Match(')'))
                {
                    stringstream ss;
                    ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                if(!Match(';'))
                {
                    stringstream ss;
                    ss << "esperado ; no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                } 
                // sai da função Stmt()
                return;
            }  
            // stmt -> block
            case '{':
            { 
                Block(); 
                return;
            }
            default:
            {
                stringstream ss;
                ss << "\'" << lookahead->toString() << "\' não inicia uma instrução válida";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
        }
    }   
}

Expr * Parser::Local()
{
    // local -> local[bool]
    //        | id

    Expr * expr = nullptr;

    switch(lookahead->tag)
    {
        case Tag::ID:
        {
            // verifica tipo da variável na tabela de símbolos
            Symbol * s = symtable->Find(lookahead->toString());
            if (!s)
            {
                stringstream ss;
                ss << "variável \"" << lookahead->toString() << "\" não declarada";
                throw SyntaxError{ scanner.Lineno(), ss.str() };
            }
            
            expr = new Identifier(new Token{*lookahead});
            Match(Tag::ID);

            if (Match('['))
            {
                Bool();
                if(!Match(']'))
                {
                    stringstream ss;
                    ss << "esperado ] no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
            }
            break;
        }
        default:
        {
            stringstream ss;
            ss << "esperado um local de armazenamento (variável ou arranjo)";
            throw SyntaxError { scanner.Lineno(), ss.str() };
        }
    }

    return expr;
}

Expr * Parser::Bool()
{
    // bool -> join lor
    // lor  -> || join lor
    //       | empty

    Expr * expr = Join();

    // função Lor() 
    while (true)
    {
        Token t = *lookahead;

        if (Match(Tag::OR))
        {
            Expr * expr2 = Join();
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

Expr * Parser::Join()
{
    // join -> equality land
    // land -> && equality land
    //       | empty

    Expr * expr = Equality();

    // função Land() 
    while (true)
    {
        Token t = *lookahead;
        if (Match(Tag::AND))
        {
            Expr * expr2 = Equality();
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

Expr * Parser::Equality()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty

    Expr * expr = Rel();

    // função Eqdif() 
    while (true)
    {
        Token t = *lookahead;

        if (lookahead->tag == Tag::EQ)
        {
            Match(Tag::EQ);
            Expr * expr2 = Rel();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if(lookahead->tag == Tag::NEQ)
        {
            Match(Tag::NEQ);
            Expr * expr2 = Rel();
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

Expr * Parser::Rel()
{
    // rel  -> ari comp
    // comp -> < ari comp
    //       | <= ari comp
    //       | > ari comp
    //       | >= ari comp
    //       | empty

    Expr * expr = Ari();

    // função Comp() 
    while (true)
    {
        Token t = *lookahead;

        if(lookahead->tag == '<')
        {
            Match('<');
            Expr * expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if(lookahead->tag == Tag::LTE)
        {
            Match(Tag::LTE);
            Expr * expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if(lookahead->tag == '>')
        {
            Match('>');
            Expr * expr2 = Ari();
            expr = new Relational(new Token{t}, expr, expr2);
        }
        else if(lookahead->tag == Tag::GTE)
        {
            Match(Tag::GTE);
            Expr * expr2 = Ari();
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

Expr * Parser::Ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Expr * expr = Term();

    // função Oper()
    while (true)
    {
        Token t = *lookahead;

        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Expr * expr2 = Term();
            expr = new Arithmetic(new Token{t}, expr, expr2);            
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Expr * expr2 = Term();
            expr = new Arithmetic(new Token{t}, expr, expr2); 
        }
        // oper -> empty
        else break; 
    }

    return expr;
}

Expr * Parser::Term()
{
    // term -> unary calc
    // calc -> * unary calc
    //       | / unary calc
    //       | empty

    Expr * expr = Unary();

    // função Calc()
    while (true)
    {
        Token t = *lookahead;

        // calc -> * unary calc
        if (lookahead->tag == '*')
        {
            Match('*');
            Expr * expr2 = Unary();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        } 
        // calc -> / unary calc
        else if (lookahead->tag == '/')
        {
            Match('/');
            Expr * expr2 = Unary();
            expr = new Arithmetic(new Token{t}, expr, expr2);
        }
        // calc -> empty
        else break;
    }

    return expr;
}

Expr * Parser::Unary()
{
    // unary -> !unary
    //        | -unary
    //        | factor

    Expr * unary = nullptr;

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


Expr * Parser::Factor()
{
    // factor -> (bool)
    //         | local
    //         | integer
    //         | floating
    //         | true
    //         | false

    Expr * expr = nullptr;

    switch (lookahead->tag)
    {
        // factor -> (bool)
        case '(':   
        {  
            Match('(');
            expr = Bool();
            if(!Match(')'))
            {
                stringstream ss;
                ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                throw SyntaxError { scanner.Lineno(), ss.str() };
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
        case Tag::INTEGER:
        {
            expr = new Constant(new Token{*lookahead});
            Match(Tag::INTEGER);
            break;
        }

        // factor -> floating
        case Tag::FLOATING:
        {
            expr = new Constant(new Token{*lookahead});
            Match(Tag::FLOATING);
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
            throw SyntaxError { scanner.Lineno(), ss.str() };
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
    Program();
}