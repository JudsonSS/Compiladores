#include <iostream>
#include <string>
#include <stack>
using namespace std;

int Eval(string input)
{
    stack<int> numbers;

    for (char token : input)
    {
        switch (token)
        {
        case '+':
        {
            int arg2 = numbers.top(); numbers.pop();
            int arg1 = numbers.top(); numbers.pop();
            int result = arg1 + arg2;
            numbers.push(result);
            break;
        }
        case '*':
        {
            int arg2 = numbers.top(); numbers.pop();
            int arg1 = numbers.top(); numbers.pop();
            int result = arg1 * arg2;
            numbers.push(result);
            break;
        }
        default:
            numbers.push(token - '0');        
        }
    }

    return numbers.top();
}

int main()
{
    cout << Eval("123*+") << endl;
    cout << Eval("12+3*") << endl;
}