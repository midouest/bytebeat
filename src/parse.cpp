#include "parse.hpp"
#include "tokenize.hpp"

#include <iostream>

namespace bb
{
    using TokenIter = vector<string>::iterator;

    Expression *parse_expression(TokenIter &it, TokenIter &end);
    Expression *parse_expression_inner(TokenIter &it,
                                       TokenIter &end,
                                       Expression *lhs,
                                       int min_precedence);
    Expression *parse_primary(TokenIter &it, TokenIter &end);

    int get_precedence(string &token);
    Expression *make_binary_op(string &op, Expression *lhs, Expression *rhs);

    Expression *parse(string &input)
    {
        vector<string> tokens = tokenize(input);
        if (tokens.empty())
        {
            throw invalid_argument("no tokens to parse");
        }

        auto it = tokens.begin();
        auto end = tokens.end();
        Expression *expr = parse_expression(it, end);
        if (it != tokens.end())
        {
            throw invalid_argument("not all tokens consumed");
        }

        return expr;
    }

    Expression *parse_expression(TokenIter &it, TokenIter &end)
    {
        Expression *lhs = parse_primary(it, end);
        return parse_expression_inner(it, end, lhs, 0);
    }

    Expression *parse_expression_inner(TokenIter &it,
                                       TokenIter &end,
                                       Expression *lhs,
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

            Expression *rhs = parse_primary(it, end);
            if (it == end)
            {
                return make_binary_op(op, lhs, rhs);
            }

            lookahead = *it;
            int next_precedence = get_precedence(lookahead);

            while (next_precedence > precedence)
            {
                rhs = parse_expression_inner(it, end, rhs, next_precedence);
                if (it == end)
                {
                    next_precedence = -1;
                    break;
                }

                lookahead = *it;
                next_precedence = get_precedence(lookahead);
            }

            precedence = next_precedence;
            lhs = make_binary_op(op, lhs, rhs);
        }

        return lhs;
    }

    Expression *parse_primary(TokenIter &it, TokenIter &end)
    {
        if (it == end)
        {
            throw invalid_argument("expected token but got end of input");
        }

        string token = *it;

        if (token == "t")
        {
            ++it;
            return new Variable();
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
            return new Constant(n);
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

    Expression *make_binary_op(std::string &op, Expression *lhs, Expression *rhs)
    {
        if (op == "+")
        {
            return new Add(lhs, rhs);
        }

        if (op == "-")
        {
            return new Subtract(lhs, rhs);
        }

        if (op == "*")
        {
            return new Multiply(lhs, rhs);
        }

        if (op == "/")
        {
            return new Divide(lhs, rhs);
        }

        if (op == "%")
        {
            return new Modulo(lhs, rhs);
        }

        if (op == "&")
        {
            return new BitwiseAnd(lhs, rhs);
        }

        if (op == "|")
        {
            return new BitwiseOr(lhs, rhs);
        }

        if (op == "^")
        {
            return new BitwiseXor(lhs, rhs);
        }

        if (op == "<<")
        {
            return new BitwiseShiftLeft(lhs, rhs);
        }

        if (op == ">>")
        {
            return new BitwiseShiftRight(lhs, rhs);
        }

        if (op == "<")
        {
            return new LessThan(lhs, rhs);
        }

        if (op == ">")
        {
            return new GreaterThan(lhs, rhs);
        }

        if (op == "<=")
        {
            return new LessThanOrEqual(lhs, rhs);
        }

        if (op == ">=")
        {
            return new GreaterThanOrEqual(lhs, rhs);
        }

        if (op == "==")
        {
            return new Equal(lhs, rhs);
        }

        if (op == "!=")
        {
            return new NotEqual(lhs, rhs);
        }

        throw invalid_argument("unrecognized operator");
    }
}
