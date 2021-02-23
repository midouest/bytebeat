#include <catch2/catch.hpp>
#include "parse.hpp"

using namespace std;
using namespace bb;

TEST_CASE("simple expression", "[parse]")
{
    string input = "t+1";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "(t+1)");
    REQUIRE(expr->evaluate(2) == 3);
}

TEST_CASE("simple expression w/o parentheses", "[parse]")
{
    string input = "t+2*t";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "(t+(2*t))");
    REQUIRE(expr->evaluate(2) == 6);
}

TEST_CASE("simple expression w/ parentheses", "[parse]")
{
    string input = "(t+1)*t";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "((t+1)*t)");
    REQUIRE(expr->evaluate(2) == 6);
}

TEST_CASE("unbalanced parentheses", "[parse]")
{
    string input = "(t+1";
    REQUIRE_THROWS_AS(parse(input), invalid_argument);
}

TEST_CASE("incomplete expression", "[parse]")
{
    string input = "t+";
    REQUIRE_THROWS_AS(parse(input), invalid_argument);
}

TEST_CASE("extra parentheses", "[parse]")
{
    string input = "t+1)))";
    REQUIRE_THROWS_AS(parse(input), invalid_argument);
}

TEST_CASE("parse empty input", "[parse]")
{
    string input = "";
    REQUIRE_THROWS_AS(parse(input), invalid_argument);
}

TEST_CASE("atomic expression", "[parse]")
{
    string input = "t";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "t");
    REQUIRE(expr->evaluate(42) == 42);
}

TEST_CASE("multiple expressions", "[parse]")
{
    string input = "(t+3)|(t*2)";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "((t+3)|(t*2))");
    REQUIRE(expr->evaluate(1) == 6);
}

TEST_CASE("precedence", "[parse]")
{
    string input = "t*2+t*3";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "((t*2)+(t*3))");
    REQUIRE(expr->evaluate(1) == 5);
}

TEST_CASE("deep nesting", "[parse]")
{
    string input = "(((t*t)))";
    Expression *expr = parse(input);
    REQUIRE(expr->to_string() == "(t*t)");
    REQUIRE(expr->evaluate(2) == 4);
}

TEST_CASE("parse crowd", "[parse]")
{
    string input = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";
    Expression *expr = parse(input);
    REQUIRE(expr);
}
