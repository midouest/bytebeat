#include "parse.hpp"
#include "lex.hpp"

namespace bb
{

int get_precedence(TokenType type);
AstPtr make_unary_op(TokenType type, AstPtr inner);
AstPtr make_binary_op(TokenType type, AstPtr left, AstPtr right);

using TokenIter = vector<Token>::iterator;

AstPtr parse_expression(TokenIter &it, TokenIter &end);
AstPtr parse_expression_inner(TokenIter &it, TokenIter &end, AstPtr lhs,
                              int min_precedence);
AstPtr parse_primary(TokenIter &it, TokenIter &end);

AstPtr parse(const string &input)
{
    auto tokens = lex(input);
    if (tokens.empty())
    {
        throw invalid_argument("No tokens to parse");
    }

    TokenIter it = tokens.begin();
    TokenIter end = tokens.end();
    AstPtr expr = parse_expression(it, end);

    if (it != tokens.end())
    {
        throw invalid_argument("Not all tokens consumed");
    }

    return expr;
}

/**
 * An operator-precedence parser
 *
 * See:
 * - https://en.wikipedia.org/wiki/Operator-precedence_parser
 * - https://en.cppreference.com/w/c/language/operator_precedence
 * - https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
 */
AstPtr parse_expression(TokenIter &it, TokenIter &end)
{
    AstPtr lhs = parse_primary(it, end);
    return parse_expression_inner(it, end, move(lhs), 0);
}

AstPtr parse_expression_inner(TokenIter &it, TokenIter &end, AstPtr lhs,
                              int min_precedence)
{
    if (it == end)
    {
        return lhs;
    }

    TokenType lookahead = it->type;
    int precedence = get_precedence(lookahead);

    while (precedence >= min_precedence)
    {
        TokenType op = lookahead;
        ++it;

        AstPtr rhs;
        if (op == TokenType::LeftBracket)
        {
            rhs = parse_expression(it, end);
            if (it->type != TokenType::RightBracket)
            {
                throw invalid_argument("Unbalanced brackets");
            }
            ++it;
        }
        else if (op == TokenType::TernaryIf)
        {
            auto pass = parse_expression(it, end);
            if (it->type != TokenType::TernaryElse)
            {
                throw invalid_argument("Missing ternary else");
            }
            ++it;
            auto fail = parse_expression(it, end);
            return AstPtr(new TernaryIf(move(lhs), move(pass), move(fail)));
        }
        else
        {
            rhs = parse_primary(it, end);
        }

        if (it == end)
        {
            return make_binary_op(op, move(lhs), move(rhs));
        }

        lookahead = it->type;
        int next_precedence = get_precedence(lookahead);

        while (next_precedence > precedence)
        {
            rhs = parse_expression_inner(it, end, move(rhs), next_precedence);
            if (it == end)
            {
                next_precedence = -1;
                break;
            }

            lookahead = it->type;
            next_precedence = get_precedence(lookahead);
        }

        precedence = next_precedence;
        lhs = make_binary_op(op, move(lhs), move(rhs));
    }

    return lhs;
}

AstPtr parse_primary(TokenIter &it, TokenIter &end)
{
    if (it == end)
    {
        throw invalid_argument("Expected primary token but got end of input");
    }

    TokenType type = it->type;

    if (type == TokenType::Identifier)
    {
        ++it;
        return AstPtr(new Identifier());
    }

    if (type == TokenType::Integer)
    {
        int n = stoi(it->value, 0, 0); // automatically detect base
        ++it;
        return AstPtr(new Integer(n));
    }

    if (type == TokenType::String)
    {
        string s = it->value;
        ++it;
        return AstPtr(new String(s));
    }

    if (type == TokenType::LeftParen)
    {
        ++it;
        AstPtr inner = parse_expression(it, end);
        if (it == end || it->type != TokenType::RightParen)
        {
            throw invalid_argument("Unbalanced parentheses");
        }
        ++it;
        return inner;
    }

    if (type == TokenType::Minus || type == TokenType::BitwiseComplement ||
        type == TokenType::Not)
    {
        ++it;
        if (it == end)
        {
            throw invalid_argument("Expected primary token for unary prefix "
                                   "operator but got end-of-input");
        }
        AstPtr inner = parse_primary(it, end);
        return make_unary_op(type, move(inner));
    }

    throw invalid_argument("Unexpected primary token");
}

int get_precedence(TokenType type)
{
    if (type == TokenType::LeftBracket)
    {
        return 11;
    }

    if (type == TokenType::Multiply || type == TokenType::Divide ||
        type == TokenType::Modulo)
    {
        return 10;
    }

    if (type == TokenType::Plus || type == TokenType::Minus)
    {
        return 9;
    }

    if (type == TokenType::BitwiseShiftLeft ||
        type == TokenType::BitwiseShiftRight)
    {
        return 8;
    }

    if (type == TokenType::LessThan || type == TokenType::LessThanEqual ||
        type == TokenType::GreaterThan || type == TokenType::GreaterThanEqual)
    {
        return 7;
    }

    if (type == TokenType::Equal || type == TokenType::NotEqual)
    {
        return 6;
    }

    if (type == TokenType::BitwiseAnd)
    {
        return 5;
    }

    if (type == TokenType::BitwiseXor)
    {
        return 4;
    }

    if (type == TokenType::BitwiseOr)
    {
        return 3;
    }

    if (type == TokenType::And)
    {
        return 2;
    }

    if (type == TokenType::Or)
    {
        return 1;
    }

    if (type == TokenType::TernaryIf)
    {
        return 0;
    }

    return -1;
}

AstPtr make_unary_op(TokenType type, AstPtr inner)
{
    if (type == TokenType::Minus)
    {
        return AstPtr(new Negate(move(inner)));
    }

    if (type == TokenType::BitwiseComplement)
    {
        return AstPtr(new BitwiseComplement(move(inner)));
    }

    if (type == TokenType::Not)
    {
        return AstPtr(new Not(move(inner)));
    }

    throw invalid_argument("Unrecognized unary operator");
}

AstPtr make_binary_op(TokenType type, AstPtr left, AstPtr right)
{
    if (type == TokenType::LeftBracket)
    {
        return AstPtr(new Subscript(move(left), move(right)));
    }

    if (type == TokenType::Plus)
    {
        return AstPtr(new Add(move(left), move(right)));
    }

    if (type == TokenType::Minus)
    {
        return AstPtr(new Subtract(move(left), move(right)));
    }

    if (type == TokenType::Multiply)
    {
        return AstPtr(new Multiply(move(left), move(right)));
    }

    if (type == TokenType::Divide)
    {
        return AstPtr(new Divide(move(left), move(right)));
    }

    if (type == TokenType::Modulo)
    {
        return AstPtr(new Modulo(move(left), move(right)));
    }

    if (type == TokenType::BitwiseAnd)
    {
        return AstPtr(new BitwiseAnd(move(left), move(right)));
    }

    if (type == TokenType::BitwiseOr)
    {
        return AstPtr(new BitwiseOr(move(left), move(right)));
    }

    if (type == TokenType::BitwiseXor)
    {
        return AstPtr(new BitwiseXor(move(left), move(right)));
    }

    if (type == TokenType::BitwiseShiftLeft)
    {
        return AstPtr(new BitwiseShiftLeft(move(left), move(right)));
    }

    if (type == TokenType::BitwiseShiftRight)
    {
        return AstPtr(new BitwiseShiftRight(move(left), move(right)));
    }

    if (type == TokenType::LessThan)
    {
        return AstPtr(new LessThan(move(left), move(right)));
    }

    if (type == TokenType::GreaterThan)
    {
        return AstPtr(new GreaterThan(move(left), move(right)));
    }

    if (type == TokenType::LessThanEqual)
    {
        return AstPtr(new LessThanEqual(move(left), move(right)));
    }

    if (type == TokenType::GreaterThanEqual)
    {
        return AstPtr(new GreaterThanEqual(move(left), move(right)));
    }

    if (type == TokenType::Equal)
    {
        return AstPtr(new Equal(move(left), move(right)));
    }

    if (type == TokenType::NotEqual)
    {
        return AstPtr(new NotEqual(move(left), move(right)));
    }

    throw invalid_argument("Unrecognized operator");
}

} // namespace bb
