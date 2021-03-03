#pragma once

#include <string>
#include <vector>

using namespace std;

namespace bb
{

/** Set of token types that can be parsed */
enum class TokenType
{
    Unknown,
    Identifier,
    Integer,
    String,
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    BitwiseShiftLeft,
    BitwiseShiftRight,
    BitwiseComplement,
    LessThan,
    GreaterThan,
    LessThanEqual,
    GreaterThanEqual,
    Equal,
    NotEqual,
    Not,
    And,
    Or,
    TernaryIf,
    TernaryElse,
};

inline ostream &operator<<(std::ostream &os, const TokenType &type)
{

    if (type == TokenType::Identifier)
    {
        os << string{"Identifier"};
    }
    else if (type == TokenType::Integer)
    {
        os << string{"Integer"};
    }
    else if (type == TokenType::String)
    {
        os << string{"String"};
    }
    else if (type == TokenType::LeftParen)
    {
        os << string{"LeftParen"};
    }
    else if (type == TokenType::RightParen)
    {
        os << string{"RightParen"};
    }
    else if (type == TokenType::LeftBracket)
    {
        os << string{"LeftBracket"};
    }
    else if (type == TokenType::RightBracket)
    {
        os << string{"RightBracket"};
    }
    else if (type == TokenType::Plus)
    {
        os << string{"Plus"};
    }
    else if (type == TokenType::Minus)
    {
        os << string{"Minus"};
    }
    else if (type == TokenType::Multiply)
    {
        os << string{"Multiply"};
    }
    else if (type == TokenType::Divide)
    {
        os << string{"Divide"};
    }
    else if (type == TokenType::Modulo)
    {
        os << string{"Modulo"};
    }
    else if (type == TokenType::BitwiseAnd)
    {
        os << string{"BitwiseAnd"};
    }
    else if (type == TokenType::BitwiseOr)
    {
        os << string{"BitwiseOr"};
    }
    else if (type == TokenType::BitwiseXor)
    {
        os << string{"BitwiseXor"};
    }
    else if (type == TokenType::BitwiseShiftLeft)
    {
        os << string{"BitwiseShiftLeft"};
    }
    else if (type == TokenType::BitwiseShiftRight)
    {
        os << string{"BitwiseShiftRight"};
    }
    else if (type == TokenType::BitwiseComplement)
    {
        os << string{"BitwiseComplement"};
    }
    else if (type == TokenType::LessThan)
    {
        os << string{"LessThan"};
    }
    else if (type == TokenType::GreaterThan)
    {
        os << string{"GreaterThan"};
    }
    else if (type == TokenType::LessThanEqual)
    {
        os << string{"LessThanEqual"};
    }
    else if (type == TokenType::GreaterThanEqual)
    {
        os << string{"GreaterThanEqual"};
    }
    else if (type == TokenType::Equal)
    {
        os << string{"Equal"};
    }
    else if (type == TokenType::NotEqual)
    {
        os << string{"NotEqual"};
    }
    else if (type == TokenType::Not)
    {
        os << string{"Not"};
    }
    else if (type == TokenType::And)
    {
        os << string{"And"};
    }
    else if (type == TokenType::Or)
    {
        os << string{"Or"};
    }
    else if (type == TokenType::TernaryIf)
    {
        os << string{"TernaryIf"};
    }
    else if (type == TokenType::TernaryElse)
    {
        os << string{"TernaryElse"};
    }
    else
    {
        os << string{"Unknown"};
    }
    return os;
}

/** A token and the category that it belongs to */
struct Token
{
    TokenType type;
    string value;
};

inline bool operator==(const Token &t1, const Token &t2)
{
    return t1.type == t2.type && t1.value == t2.value;
}

inline ostream &operator<<(std::ostream &os, const Token &token)
{
    os << string{"Token{"};
    os << token.type;
    os << string{", "};
    os << token.value;
    os << string{"}"};
    return os;
}

/** Split a string into a list of component tokens */
vector<Token> lex(const string &input);

} // namespace bb
