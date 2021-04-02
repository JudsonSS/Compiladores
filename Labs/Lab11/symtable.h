#ifndef COMPILER_SYMTABLE
#define COMPILER_SYMTABLE

#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;


// modelo para símbolos
struct Symbol
{
	string var;
	string type;
};


// tabela de símbolos
class SymTable
{
private: 
   	unordered_map<string,Symbol> table;    
   	SymTable * prev;   

public:
	SymTable();
	SymTable(SymTable * t);
	
	bool Insert(string s, Symbol symb);
	Symbol * Find(string s); 
};

#endif