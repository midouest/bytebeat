#include <iostream>

#include "parse.hpp"

using namespace std;
using namespace bb;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << endl;
        cout << "  usage:" << endl;
        cout << "    ./bytebeat [EXPRESSION] | head -c [BYTES] > [OUT].raw" << endl;
        cout << endl;
        cout << "  expression tokens:" << endl;
        cout << "    t" << endl;
        cout << "    ( ) + - * / %" << endl;
        cout << "    & | ^ << >>" << endl;
        cout << "    < > <= >= == !=" << endl;
        cout << endl;
        return 1;
    }

    string input{argv[1]};
    Expression *expr;
    try
    {
        expr = parse(input);
    }
    catch (invalid_argument &ex)
    {
        cerr << "failed to parse expression. " << ex.what() << endl;
        return 1;
    }

    int t = 0;
    while (true)
    {
        putchar(expr->evaluate(t++));
    }
}
