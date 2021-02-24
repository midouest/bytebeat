#include <string>
#include <vector>
#include <stdexcept>

#include "tokenize.hpp"

using namespace std;

namespace bb
{
    bool is_whitespace(char c);
    bool is_terminal(char c);

    vector<string> tokenize(string &input)
    {
        vector<string> tokens;

        for (auto it = input.begin(); it != input.end(); ++it)
        {
            char c = *it;
            if (is_whitespace(c))
            {
                continue;
            }

            if (is_terminal(c))
            {
                tokens.push_back(string{c});
                continue;
            }

            if (c == '<' || c == '>')
            {
                string buffer{c};
                char n = *next(it);
                if (n == c || n == '=')
                {
                    buffer.push_back(n);
                    ++it;
                }
                tokens.push_back(buffer);
                continue;
            }

            if (c == '=' || c == '!')
            {
                char n = *next(it);
                if (n != '=')
                {
                    auto msg = "invalid character sequence: " + string{n, c};
                    throw invalid_argument(msg);
                }
                tokens.push_back(string{c, n});
                ++it;
                continue;
            }

            if (!isdigit(c))
            {
                auto msg = "invalid character: " + string{c};
                throw invalid_argument(msg);
            }

            string buffer{c};
            auto peek = next(it);
            while (peek != input.end())
            {
                c = *peek;
                if (!isdigit(c))
                {
                    break;
                }
                buffer.push_back(c);
                ++it;
                peek = next(it);
            }
            tokens.push_back(buffer);
        }

        return tokens;
    }

    bool is_whitespace(char c)
    {
        return c == ' ';
    }

    bool is_terminal(char c)
    {
        return c == 't' ||
               c == '(' ||
               c == ')' ||
               c == '+' ||
               c == '-' ||
               c == '*' ||
               c == '/' ||
               c == '%' ||
               c == '&' ||
               c == '|' ||
               c == '^';
    }
}
