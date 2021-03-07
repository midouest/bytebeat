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
        cout << "    ./bytebeat [EXPRESSION] | head -c [BYTES] > [OUT].raw"
             << endl;
        cout << endl;
        cout << "  expression tokens:" << endl;
        cout << "    t" << endl;
        cout << "    1, -1, 0xf, \"foo\"" << endl;
        cout << "    ( ) + - * / %" << endl;
        cout << "    & | ^ << >> ~" << endl;
        cout << "    < > <= >= == != ! ? :" << endl;
        cout << "    [ ]" << endl;
        cout << endl;
        return 1;
    }

    string input{argv[1]};
    AstPtr expr;
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
        // TODO: check for undefined
        putchar(expr->eval(t++).to_int());
    }
}
