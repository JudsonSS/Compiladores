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
    if (!Match(Tag::PROGRAM))
        throw SyntaxError(scanner.Lineno(), "\'program\' esperado");
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
        string type {lookahead->toString()};
        Match(Tag::TYPE);
        
        string name {lookahead->toString()};
        Match(Tag::ID);
        
        Symbol s { name, type };
        
        // insere variável na tabela de símbolos
        if(!symtable->Insert(name, s))
        {
            // a inserção falha quando a variável já está na tabela
            stringstream ss;
            ss << "variável \"" << name << "\" já definida";
            throw SyntaxError(scanner.Lineno(), ss.str());    
        }

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
            case Tag::INTEGRAL:
            case Tag::FLOATING:
            case '(':
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
    
    // stmt  -> expr;
    //        | if (expr) stmt
    //        | while (expr) stmt
    //        | do stmt while (expr);
    //        | block
    
    while(true)
    {
        switch(lookahead->tag)
        {
            // stmt -> expr; 
            case Tag::ID:
            case Tag::INTEGRAL:
            case Tag::FLOATING:
            case '(':
            { 
                Expr();

                if (!Match(';'))
                {
                    stringstream ss;
                    ss << "esperado ; no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }

                // sai da função Stmt()
                return;
            }

            // stmt -> if (expr) stmt
            case Tag::IF:
            {
                Match(Tag::IF);
                if (!Match('('))
                {
                    stringstream ss;
                    ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Expr();
                if (!Match(')'))
                {
                    stringstream ss;
                    ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                // se mantém na função Stmt()
                break;
            }

            // stmt -> while (expr) stmt
            case Tag::WHILE:
            {
                Match(Tag::WHILE);
                if (!Match('('))
                {
                    stringstream ss;
                    ss << "esperado ( no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                Expr();
                if (!Match(')'))
                {
                    stringstream ss;
                    ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                    throw SyntaxError { scanner.Lineno(), ss.str() };
                }
                // se mantém na função Stmt()
                break;
            }

            //  stmt -> do stmt while (expr);
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
                Expr();
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

                // sai da função Stmt() 
                return;
            }

            // erro
            default:
            {
                stringstream ss;
                ss << "\'" << lookahead->toString() << "\' não inicia uma instrução válida";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
        }
    }   
}

void Parser::Expr()
{
    // expr -> rel = expr
    //       | rel
    
    while(true)
    {
        Rel();

        // se for seguido por atribuição
        if (lookahead->tag == '=')
        {
            Match('=');
        }
        else
        {
            break;
        }
    }
}


void Parser::Rel()
{
    // rel  -> ari comp
    // comp -> < ari comp
    //       | <= ari comp
    //       | > ari comp
    //       | >= ari comp
    //       | empty

    Ari();

    // função Comp() 
    while (true)
    {
        switch(lookahead->tag) 
        {
            case '<':
            {
                Match('<');
                Ari();
                break;
            }
            case Tag::LTE:
            {
                Match(Tag::LTE);
                Ari();
                break;
            }
            case '>':
            {
                Match('>');
                Ari();
                break;
            }
            case Tag::GTE:
            {
                Match(Tag::GTE);
                Ari();
                break;
            }
            default:
            {
                // empty
                return;
            }   
        }
    }
}

void Parser::Ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Term();

    // função Oper()
    while (true)
    {
        // oper -> + term oper
        if (lookahead->tag == '+')
        {
            Match('+');
            Term();
        }
        // oper -> - term oper
        else if (lookahead->tag == '-')
        {
            Match('-');
            Term();
        }
        // oper -> empty
        else return; 
    }
}

void Parser::Term()
{
    // term -> factor oper
    // oper -> * factor oper
    //       | / factor oper
    //       | empty

    Factor();

    // função Oper()
    while (true)
    {
        // oper -> * factor oper
        if (lookahead->tag == '*')
        {
            Match('*');
            Factor();
        } 
        // oper -> / factor oper
        else if (lookahead->tag == '/')
        {
            Match('/');
            Factor();
        }
        // oper -> empty
        else return;
    }
}

void Parser::Factor()
{
    // factor -> (expr)
    //         | id
    //         | integral
    //         | floating

    switch (lookahead->tag)
    {
        // factor -> (expr)
        case '(':   
        {  
            Match('(');
            Expr();
            if(!Match(')'))
            {
                stringstream ss;
                ss << "esperado ) no lugar de  \'" << lookahead->toString() << "\'";
                throw SyntaxError { scanner.Lineno(), ss.str() };
            }
            break;
        }

        // factor -> id
        case Tag::ID:
        {
            Match(Tag::ID);
            break;
        }

        // factor -> integeral
        case Tag::INTEGRAL:
        {
            Match(Tag::INTEGRAL);
            break;
        }

        // factor -> floating
        case Tag::FLOATING:
        {
            Match(Tag::FLOATING);
            break;
        }

        // erro
        default:
        {
            stringstream ss;
            ss << "uma expressão é esperada no lugar de  \'" << lookahead->toString() << "\'";
            throw SyntaxError { scanner.Lineno(), ss.str() };
            break;
        }
    }
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