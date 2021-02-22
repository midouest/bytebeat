#include <catch2/catch.hpp>

#include "tokenize.hpp"

using namespace std;
using namespace bb;

TEST_CASE("tokenize empty input", "[tokenize]")
{
    string input = "";
    REQUIRE(tokenize(input).empty());
}

TEST_CASE("single-char tokens", "[tokenize]")
{
    string input = "t()+-*/%&|^";
    vector<string> expected = {"t", "(", ")", "+", "-", "*", "/", "%", "&", "|", "^"};
    REQUIRE(tokenize(input) == expected);
}

TEST_CASE("whitespace", "[tokenize]")
{
    string input = " ( t ) ";
    vector<string> expected = {"(", "t", ")"};
    REQUIRE(tokenize(input) == expected);
}

TEST_CASE("integers", "[tokenize]")
{
    string input = "(t+1)";
    vector<string> expected = {"(", "t", "+", "1", ")"};
    REQUIRE(tokenize(input) == expected);
}

TEST_CASE("multi-digit integers", "[tokenize]")
{
    string input = "t&63";
    vector<string> expected = {"t", "&", "63"};
    REQUIRE(tokenize(input) == expected);
}

TEST_CASE("multi-char tokens", "[tokenize]")
{
    string input = "t>>1";
    vector<string> expected = {"t", ">>", "1"};
    REQUIRE(tokenize(input) == expected);
}

TEST_CASE("tokenize crowd", "[tokenize]")
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
    REQUIRE(bb::tokenize(input) == expected);
}
