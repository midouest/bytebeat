#include <catch2/catch.hpp>
#include "lex.hpp"

using namespace std;
using namespace bb;

TEST_CASE("lex empty input", "[lex]")
{
    string input = "";
    REQUIRE(lex(input).empty());
}

TEST_CASE("single-char tokens", "[lex]")
{
    string input = "t()+-*/%&|^";
    vector<string> expected = {"t", "(", ")", "+", "-", "*", "/", "%", "&", "|", "^"};
    REQUIRE(lex(input) == expected);
}

TEST_CASE("whitespace", "[lex]")
{
    string input = " ( t ) ";
    vector<string> expected = {"(", "t", ")"};
    REQUIRE(lex(input) == expected);
}

TEST_CASE("integers", "[lex]")
{
    string input = "(t+1)";
    vector<string> expected = {"(", "t", "+", "1", ")"};
    REQUIRE(lex(input) == expected);
}

TEST_CASE("multi-digit integers", "[lex]")
{
    string input = "t&63";
    vector<string> expected = {"t", "&", "63"};
    REQUIRE(lex(input) == expected);
}

TEST_CASE("multi-char tokens", "[lex]")
{
    string input = "t>>1";
    vector<string> expected = {"t", ">>", "1"};
    REQUIRE(lex(input) == expected);
}

TEST_CASE("lex crowd", "[lex]")
{
    std::string input = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";
    std::vector<std::string> expected = {
        "(",
        "(",
        "t",
        "<<",
        "1",
        ")",
        "^",
        "(",
        "(",
        "t",
        "<<",
        "1",
        ")",
        "+",
        "(",
        "t",
        ">>",
        "7",
        ")",
        "&",
        "t",
        ">>",
        "12",
        ")",
        ")",
        "|",
        "t",
        ">>",
        "(",
        "4",
        "-",
        "(",
        "1",
        "^",
        "7",
        "&",
        "(",
        "t",
        ">>",
        "19",
        ")",
        ")",
        ")",
        "|",
        "t",
        ">>",
        "7",
    };
    REQUIRE(bb::lex(input) == expected);
}
