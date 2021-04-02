#include <iostream>
#include <fstream>
#include "lexer.h"
#include "ast.h"
using namespace std;

extern ifstream fin;

void TestLexer()
{
    Lexer scanner;
    Token *t = nullptr;
    while ((t = scanner.Scan()) && (t->tag != EOF))
    {
        switch (t->tag)
        {
        case ID:
            cout << "<ID," << t->lexeme << "> ";
            break;
        case INTEGER:
            cout << "<INTEGER," << t->lexeme << "> ";
            break;
        case REAL:
            cout << "<REAL," << t->lexeme << "> ";
            break;
        case TYPE:
            cout << "<TYPE," << t->lexeme << "> ";
            break;
        case TRUE:
            cout << "<TRUE," << t->lexeme << "> ";
            break;
        case FALSE:
            cout << "<FALSE," << t->lexeme << "> ";
            break;
        case MAIN:
            cout << "<MAIN," << t->lexeme << "> ";
            break;
        case IF:
            cout << "<IF," << t->lexeme << "> ";
            break;
        case WHILE:
            cout << "<WHILE," << t->lexeme << "> ";
            break;
        case DO:
            cout << "<DO," << t->lexeme << "> ";
            break;
        case OR:
            cout << "<OR," << t->lexeme << "> ";
            break;
        case AND:
            cout << "<AND," << t->lexeme << "> ";
            break;
        case EQ:
            cout << "<EQ," << t->lexeme << "> ";
            break;
        case NEQ:
            cout << "<NEQ," << t->lexeme << "> ";
            break;
        case LTE:
            cout << "<LTE," << t->lexeme << "> ";
            break;
        case GTE:
            cout << "<GTE," << t->lexeme << "> ";
            break;
        default:
            cout << "<" << t->lexeme << "> ";
            break;
        }
    }

    fin.clear();
    fin.seekg(0, ios::beg);
    cout << endl << endl;
}

void Traverse(Node *n)
{
    if (n)
    {
        switch (n->node_type)
        {
        case SEQ:
        {
            Seq *s = (Seq *)n;
            cout << "<SEQ> ";
            Traverse(s->stmt);
            cout << "\n";
            Traverse(s->stmts);
            cout << "</SEQ> ";
            break;
        }
        case ASSIGN:
        {
            Assign *a = (Assign *)n;
            cout << "<ASSIGN> ";
            Traverse(a->id);
            cout << "= ";
            Traverse(a->expr);
            cout << "</ASSIGN> ";
            break;
        }
        case REL:
        {
            Relational *r = (Relational *)n;
            cout << "<REL> ";
            Traverse(r->expr1);
            cout << r->Name() << " ";
            Traverse(r->expr2);
            cout << "</REL> ";
            break;
        }
        case LOG:
        {
            Logical *l = (Logical *)n;
            cout << "<LOG> ";
            Traverse(l->expr1);
            cout << l->Name() << " ";
            Traverse(l->expr2);
            cout << "</LOG> ";
            break;
        }
        case ARI:
        {
            Arithmetic *a = (Arithmetic *)n;
            cout << "<ARI> ";
            Traverse(a->expr1);
            cout << a->Name() << " ";
            Traverse(a->expr2);
            cout << "</ARI> ";
            break;
        }
        case UNARY:
        {
            UnaryExpr *u = (UnaryExpr *)n;
            cout << "<UNARY> ";
            cout << u->Name() << " ";
            Traverse(u->expr);
            cout << "</UNARY> ";
            break;
        }
        case CONSTANT:
        {
            Constant *c = (Constant *)n;
            cout << c->Name() << " ";
            break;
        }
        case IDENTIFIER:
        {
            Identifier *i = (Identifier *)n;
            cout << i->Name() << " ";
            break;
        }
        case ACCESS:
        {
            Access *a = (Access*) n;
            Traverse(a->id);
            cout << "[ ";
            Traverse(a->expr);
            cout << "] ";
            break;
        }
        case IF_STMT:
        {
            If *i = (If *)n;
            cout << "<IF> ";
            Traverse(i->expr);
            cout << "\n";
            Traverse(i->stmt);
            cout << "</IF> ";
            break;
        }
        case WHILE_STMT:
        {
            While *w = (While *)n;
            cout << "<WHILE> ";
            Traverse(w->expr);
            cout << "\n";
            Traverse(w->stmt);
            cout << "</WHILE> ";
            break;
        }
        case DOWHILE_STMT:
        {
            DoWhile *dw = (DoWhile *)n;
            cout << "<DOWHILE> ";
            Traverse(dw->stmt);
            Traverse(dw->expr);
            cout << "</DOWHILE> ";
            break;
        }
        }
    }
}

void TestParser(Node *n)
{
    Traverse(n);   
    cout << endl; 
}
