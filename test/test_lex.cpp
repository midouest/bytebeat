#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include "lex.hpp"

using namespace std;
using namespace bb;

TEST_CASE("lex", "[lex]")
{
    SECTION("skip whitespace")
    {
        string in = "t + 1";
        vector<Token> out = {
            Token{TokenType::Identifier, "t"},
            Token{TokenType::Plus, "+"},
            Token{TokenType::Integer, "1"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("terminal tokens")
    {
        string in = "t+-*/%^~?:()[]";
        vector<Token> out = {
            Token{TokenType::Identifier, "t"},
            Token{TokenType::Plus, "+"},
            Token{TokenType::Minus, "-"},
            Token{TokenType::Multiply, "*"},
            Token{TokenType::Divide, "/"},
            Token{TokenType::Modulo, "%"},
            Token{TokenType::BitwiseXor, "^"},
            Token{TokenType::BitwiseComplement, "~"},
            Token{TokenType::TernaryIf, "?"},
            Token{TokenType::TernaryElse, ":"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::LeftBracket, "["},
            Token{TokenType::RightBracket, "]"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("multi-digit numbers")
    {
        string in = "1234";
        vector<Token> out = {Token{TokenType::Integer, "1234"}};
        REQUIRE(lex(in) == out);
    }

    SECTION("hex numbers")
    {
        string in = "0xfff";
        vector<Token> out = {Token{TokenType::Integer, "0xfff"}};
        REQUIRE(lex(in) == out);
    }

    SECTION("invalid hex numbers")
    {
        string in = "0x";
        REQUIRE_THROWS(lex(in));

        in = "0x0x1";
        REQUIRE_THROWS(lex(in));

        in = "0xx1";
        REQUIRE_THROWS(lex(in));

        in = "1x1";
        REQUIRE_THROWS(lex(in));

        in = "1ff";
        REQUIRE_THROWS(lex(in));
    }

    SECTION("left arrow tokens")
    {
        string in = "< << <= <";
        vector<Token> out = {
            Token{TokenType::LessThan, "<"},
            Token{TokenType::BitwiseShiftLeft, "<<"},
            Token{TokenType::LessThanEqual, "<="},
            Token{TokenType::LessThan, "<"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("right arrow tokens")
    {
        string in = "> >> >= >";
        vector<Token> out = {
            Token{TokenType::GreaterThan, ">"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::GreaterThanEqual, ">="},
            Token{TokenType::GreaterThan, ">"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("not tokens")
    {
        string in = "! != !";
        vector<Token> out = {
            Token{TokenType::Not, "!"},
            Token{TokenType::NotEqual, "!="},
            Token{TokenType::Not, "!"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("equal")
    {
        string in = "==";
        vector<Token> out = {Token{TokenType::Equal, "=="}};
        REQUIRE(lex(in) == out);
    }

    SECTION("invalid equal")
    {
        string in = "=";
        REQUIRE_THROWS(lex(in));

        in = "=+";
        REQUIRE_THROWS(lex(in));
    }

    SECTION("and tokens")
    {
        string in = "& && &";
        vector<Token> out = {
            Token{TokenType::BitwiseAnd, "&"},
            Token{TokenType::And, "&&"},
            Token{TokenType::BitwiseAnd, "&"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("or tokens")
    {
        string in = "| || |";
        vector<Token> out = {
            Token{TokenType::BitwiseOr, "|"},
            Token{TokenType::Or, "||"},
            Token{TokenType::BitwiseOr, "|"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("strings")
    {
        string in = "\"foo\"";
        vector<Token> out = {
            Token{TokenType::String, "foo"},
        };
        REQUIRE(lex(in) == out);
    }

    SECTION("invalid strings")
    {
        string in = "\"";
        REQUIRE_THROWS(lex(in));

        in = "\"foo";
        REQUIRE_THROWS(lex(in));
    }

    SECTION("crowd")
    {
        string in = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";
        vector<Token> out = {
            Token{TokenType::LeftParen, "("},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftLeft, "<<"},
            Token{TokenType::Integer, "1"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::BitwiseXor, "^"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftLeft, "<<"},
            Token{TokenType::Integer, "1"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::Plus, "+"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::Integer, "7"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::BitwiseAnd, "&"},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::Integer, "12"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::BitwiseOr, "|"},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Integer, "4"},
            Token{TokenType::Minus, "-"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Integer, "1"},
            Token{TokenType::BitwiseXor, "^"},
            Token{TokenType::Integer, "7"},
            Token{TokenType::BitwiseAnd, "&"},
            Token{TokenType::LeftParen, "("},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::Integer, "19"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::RightParen, ")"},
            Token{TokenType::BitwiseOr, "|"},
            Token{TokenType::Identifier, "t"},
            Token{TokenType::BitwiseShiftRight, ">>"},
            Token{TokenType::Integer, "7"},
        };
        REQUIRE(lex(in) == out);

        BENCHMARK("lex crowd") { return lex(in); };
    }
}
