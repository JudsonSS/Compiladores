#include <iostream>
using namespace std;

void func(int && v)
{
	cout << "R-value: " << v << endl;
}

void func(int & v)
{
	cout << "L-value: " << v << endl;
}

int main()
{
	int val = 1;
	func(val);
	func(5);
}
