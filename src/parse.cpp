#include "parse.hpp"
#include "tokenize.hpp"

#include <iostream>

namespace bb
{
    using TokenIter = vector<string>::iterator;

    ExpressionPtr parse_expression(TokenIter &it, TokenIter &end);
    ExpressionPtr parse_expression_inner(TokenIter &it,
                                         TokenIter &end,
                                         ExpressionPtr lhs,
                                         int min_precedence);
    ExpressionPtr parse_primary(TokenIter &it, TokenIter &end);

    int get_precedence(string &token);
    ExpressionPtr make_binary_op(string &op, ExpressionPtr lhs, ExpressionPtr rhs);

    ExpressionPtr parse(string &input)
    {
        vector<string> tokens = tokenize(input);
        if (tokens.empty())
        {
            throw invalid_argument("no tokens to parse");
        }

        auto it = tokens.begin();
        auto end = tokens.end();
        ExpressionPtr expr = parse_expression(it, end);
        if (it != tokens.end())
        {
            throw invalid_argument("not all tokens consumed");
        }

        return expr;
    }

    ExpressionPtr parse_expression(TokenIter &it, TokenIter &end)
    {
        ExpressionPtr lhs = parse_primary(it, end);
        return parse_expression_inner(it, end, move(lhs), 0);
    }

    ExpressionPtr parse_expression_inner(TokenIter &it,
                                         TokenIter &end,
                                         ExpressionPtr lhs,
                                         int min_precedence)
    {
        if (it == end)
        {
            return lhs;
        }

        string lookahead = *it;
        int precedence = get_precedence(lookahead);

        while (precedence >= min_precedence)
        {
            string op = lookahead;
            ++it;

            ExpressionPtr rhs = parse_primary(it, end);
            if (it == end)
            {
                return make_binary_op(op, move(lhs), move(rhs));
            }

            lookahead = *it;
            int next_precedence = get_precedence(lookahead);

            while (next_precedence > precedence)
            {
                rhs = parse_expression_inner(it, end, move(rhs), next_precedence);
                if (it == end)
                {
                    next_precedence = -1;
                    break;
                }

                lookahead = *it;
                next_precedence = get_precedence(lookahead);
            }

            precedence = next_precedence;
            lhs = make_binary_op(op, move(lhs), move(rhs));
        }

        return lhs;
    }

    ExpressionPtr parse_primary(TokenIter &it, TokenIter &end)
    {
        if (it == end)
        {
            throw invalid_argument("expected token but got end of input");
        }

        string token = *it;

        if (token == "t")
        {
            ++it;
            return ExpressionPtr(new Variable());
        }

        if (token == "(")
        {
            ++it;
            auto expr = parse_expression(it, end);
            if (it == end || *it != ")")
            {
                throw invalid_argument("unbalanced parenthesis: ");
            }
            ++it;
            return expr;
        }

        try
        {
            int n = stoi(token);
            ++it;
            return ExpressionPtr(new Constant(n));
        }
        catch (invalid_argument &ex)
        {
            throw invalid_argument("unexpected primary token: " + token);
        }
    }

    int get_precedence(string &token)
    {
        if (token == "*" || token == "/" || token == "%")
        {
            return 7;
        }
        else if (token == "+" || token == "-")
        {
            return 6;
        }
        else if (token == "<<" || token == ">>")
        {
            return 5;
        }
        else if (token == "<" || token == "<=" || token == ">" || token == ">=")
        {
            return 4;
        }
        else if (token == "==" || token == "!=")
        {
            return 3;
        }
        else if (token == "&")
        {
            return 2;
        }
        else if (token == "^")
        {
            return 1;
        }
        else if (token == "|")
        {
            return 0;
        }

        return -1;
    }

    ExpressionPtr make_binary_op(std::string &op, ExpressionPtr lhs, ExpressionPtr rhs)
    {
        if (op == "+")
        {
            return ExpressionPtr(new Add(move(lhs), move(rhs)));
        }

        if (op == "-")
        {
            return ExpressionPtr(new Subtract(move(lhs), move(rhs)));
        }

        if (op == "*")
        {
            return ExpressionPtr(new Multiply(move(lhs), move(rhs)));
        }

        if (op == "/")
        {
            return ExpressionPtr(new Divide(move(lhs), move(rhs)));
        }

        if (op == "%")
        {
            return ExpressionPtr(new Modulo(move(lhs), move(rhs)));
        }

        if (op == "&")
        {
            return ExpressionPtr(new BitwiseAnd(move(lhs), move(rhs)));
        }

        if (op == "|")
        {
            return ExpressionPtr(new BitwiseOr(move(lhs), move(rhs)));
        }

        if (op == "^")
        {
            return ExpressionPtr(new BitwiseXor(move(lhs), move(rhs)));
        }

        if (op == "<<")
        {
            return ExpressionPtr(new BitwiseShiftLeft(move(lhs), move(rhs)));
        }

        if (op == ">>")
        {
            return ExpressionPtr(new BitwiseShiftRight(move(lhs), move(rhs)));
        }

        if (op == "<")
        {
            return ExpressionPtr(new LessThan(move(lhs), move(rhs)));
        }

        if (op == ">")
        {
            return ExpressionPtr(new GreaterThan(move(lhs), move(rhs)));
        }

        if (op == "<=")
        {
            return ExpressionPtr(new LessThanOrEqual(move(lhs), move(rhs)));
        }

        if (op == ">=")
        {
            return ExpressionPtr(new GreaterThanOrEqual(move(lhs), move(rhs)));
        }

        if (op == "==")
        {
            return ExpressionPtr(new Equal(move(lhs), move(rhs)));
        }

        if (op == "!=")
        {
            return ExpressionPtr(new NotEqual(move(lhs), move(rhs)));
        }

        throw invalid_argument("unrecognized operator");
    }
}
