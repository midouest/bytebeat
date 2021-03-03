#include "lex.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace bb
{

TokenType get_terminal_type(char c);

vector<Token> lex(const string &input)
{
    vector<Token> tokens;

    for (int i = 0; i < input.length(); ++i)
    {
        char c = input[i];

        // Whitespace
        if (c == ' ')
        {
            continue;
        }

        // Integer constant
        if (isdigit(c))
        {
            string buffer{c};
            bool is_hex = false;
            while (i < input.length() - 1)
            {
                c = input[i + 1];
                if (!is_hex && !isdigit(c) || is_hex && !isxdigit(c))
                {
                    if (tolower(c) == 'x' && buffer.length() == 1 &&
                        buffer[0] == '0')
                    {
                        is_hex = true;
                    }
                    else
                    {
                        break;
                    }
                }
                buffer.push_back(c);
                ++i;
            }

            if (buffer.length() == 2 && buffer[1] == 'x')
            {
                throw invalid_argument("Invalid hexadecimal integer: 0x");
            }

            tokens.push_back({
                TokenType::Integer,
                buffer,
            });
            continue;
        }

        // Terminal tokens
        auto terminal_type = get_terminal_type(c);
        if (terminal_type != TokenType::Unknown)
        {
            tokens.push_back(Token{
                terminal_type,
                string{c},
            });
            continue;
        }

        // Less than, less than or equal, bitwise shift left
        if (c == '<')
        {
            TokenType type = TokenType::LessThan;
            string buffer{c};
            if (i < input.length())
            {
                c = input[i + 1];
                if (c == '<')
                {
                    type = TokenType::BitwiseShiftLeft;
                    buffer.push_back(c);
                    ++i;
                }
                else if (c == '=')
                {
                    type = TokenType::LessThanEqual;
                    buffer.push_back(c);
                    ++i;
                }
            }
            tokens.push_back({type, buffer});
            continue;
        }

        // Greater than, greater than or equal, bitwise shift right
        if (c == '>')
        {
            TokenType type = TokenType::GreaterThan;
            string buffer{c};
            if (i < input.length())
            {
                c = input[i + 1];
                if (c == '>')
                {
                    type = TokenType::BitwiseShiftRight;
                    buffer.push_back(c);
                    ++i;
                }
                else if (c == '=')
                {
                    type = TokenType::GreaterThanEqual;
                    buffer.push_back(c);
                    ++i;
                }
            }
            tokens.push_back({type, buffer});
            continue;
        }

        // Not, not equal
        if (c == '!')
        {
            TokenType type = TokenType::Not;
            string buffer{c};
            if (i < input.length())
            {
                c = input[i + 1];
                if (c == '=')
                {
                    type = TokenType::NotEqual;
                    buffer.push_back(c);
                    ++i;
                }
            }
            tokens.push_back({type, buffer});
            continue;
        }

        // Equal
        if (c == '=')
        {
            if (i == input.length() - 1)
            {
                throw invalid_argument("Invalid token: = (EOF)");
            }

            string buffer{c};
            c = input[i + 1];
            if (c != '=')
            {
                throw invalid_argument("Invalid token: =");
            }
            ++i;
            buffer.push_back(c);
            tokens.push_back({TokenType::Equal, buffer});
            continue;
        }

        // Bitwise and, and
        if (c == '&')
        {
            TokenType type = TokenType::BitwiseAnd;
            string buffer{c};
            if (i < input.length())
            {
                c = input[i + 1];
                if (c == '&')
                {
                    type = TokenType::And;
                    buffer.push_back(c);
                    ++i;
                }
            }
            tokens.push_back({type, buffer});
            continue;
        }

        // Bitwise or, or
        if (c == '|')
        {
            TokenType type = TokenType::BitwiseOr;
            string buffer{c};
            if (i < input.length())
            {
                c = input[i + 1];
                if (c == '|')
                {
                    type = TokenType::Or;
                    buffer.push_back(c);
                    ++i;
                }
            }
            tokens.push_back({type, buffer});
            continue;
        }

        // String literal
        if (c == '"')
        {
            if (i == input.length() - 1)
            {
                throw invalid_argument("Invalid string: \" (EOF)");
            }

            string buffer;
            ++i;
            while (i < input.length())
            {
                c = input[i];
                if (c == '"')
                {
                    break;
                }
                buffer.push_back(input[i]);
                ++i;
            }

            if (i == input.length())
            {
                throw invalid_argument("Invalid string: \"" + buffer);
            }

            tokens.push_back({TokenType::String, buffer});
            continue;
        }

        // Invalid token
        string msg = "Invalid token: ";
        msg.push_back(c);
        throw invalid_argument(msg);
    }

    return tokens;
}

TokenType get_terminal_type(char c)
{
    switch (c)
    {
    case 't':
        return TokenType::Identifier;
    case '+':
        return TokenType::Plus;
    case '-':
        return TokenType::Minus;
    case '*':
        return TokenType::Multiply;
    case '/':
        return TokenType::Divide;
    case '%':
        return TokenType::Modulo;
    case '^':
        return TokenType::BitwiseXor;
    case '~':
        return TokenType::BitwiseComplement;
    case '?':
        return TokenType::TernaryIf;
    case ':':
        return TokenType::TernaryElse;
    case '(':
        return TokenType::LeftParen;
    case ')':
        return TokenType::RightParen;
    case '[':
        return TokenType::LeftBracket;
    case ']':
        return TokenType::RightBracket;
    default:
        return TokenType::Unknown;
    }
}

} // namespace bb
