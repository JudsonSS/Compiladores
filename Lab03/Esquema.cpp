// Tradução Dirigida por Sintaxe

// o código constrói manualmente a árvore de derivação de 9-5+2
// e utiliza uma busca em profundidade para traduzir a expressão
// em sua versão pós-fixada 

// O método utilizado é o do "Esquema de Tradução Dirigido por Sintaxe"
// que adiciona ações semânticas na árvore de derivação. Os nós do tipo 
// ACTION representam as ações semânticas

#include <iostream>
#include <vector>
using namespace std;

enum NodeTypes { EXPR, TERM, TOKEN, ACTION };

struct Node
{
	int type;
	char print;
	vector<Node*> children;
};

void DepthFirst(Node* node)
{
	if (node->type == EXPR || node->type == TERM)
	{
		for (Node* child : node->children)
		{
			DepthFirst(child);
		}
	}

	if (node->type == ACTION)
	{
		cout << node->print;
	}
}

int main()
{
	// ---- nivel 1 da árvore -----

	Node expr0{ EXPR };

	// ---- nivel 2 da árvore -----

	Node expr1{ EXPR };
	Node plus{ TOKEN };
	Node term1{ TERM };
	Node act1{ ACTION, '+' };

	expr0.children.push_back(&expr1);
	expr0.children.push_back(&plus);
	expr0.children.push_back(&term1);
	expr0.children.push_back(&act1);

	// ---- nivel 3 da árvore -----

	Node expr2{ EXPR };
	Node minus{ TOKEN };
	Node term2{ TERM };
	Node act2{ ACTION, '-' };

	expr1.children.push_back(&expr2);
	expr1.children.push_back(&minus);
	expr1.children.push_back(&term2);
	expr1.children.push_back(&act2);

	Node two{ TOKEN };
	Node act3{ ACTION, '2' };
	term1.children.push_back(&two);
	term1.children.push_back(&act3);

	// ---- nivel 4 da árvore -----

	Node term3{ TERM };
	Node five{ TOKEN };
	Node act4{ ACTION, '5' };

	expr2.children.push_back(&term3);
	term2.children.push_back(&five);
	term2.children.push_back(&act4);

	// ---- nivel 5 da árvore -----

	Node nine{ TOKEN };
	Node act5{ ACTION, '9' };
	term3.children.push_back(&nine);
	term3.children.push_back(&act5);

	DepthFirst(&expr0);
	cout << endl;
}