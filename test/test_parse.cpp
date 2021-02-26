#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include "parse.hpp"

using namespace std;
using namespace bb;

TEST_CASE("parse", "[parse]")
{

    SECTION("simple expression")
    {
        string input = "t+1";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "(t+1)");
        REQUIRE(expr->evaluate(2) == 3);
    }

    SECTION("simple expression w/o parentheses")
    {
        string input = "t+2*t";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "(t+(2*t))");
        REQUIRE(expr->evaluate(2) == 6);
    }

    SECTION("simple expression w/ parentheses")
    {
        string input = "(t+1)*t";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "((t+1)*t)");
        REQUIRE(expr->evaluate(2) == 6);
    }

    SECTION("unbalanced parentheses")
    {
        string input = "(t+1";
        REQUIRE_THROWS_AS(parse(input), invalid_argument);
    }

    SECTION("incomplete expression")
    {
        string input = "t+";
        REQUIRE_THROWS_AS(parse(input), invalid_argument);
    }

    SECTION("extra parentheses")
    {
        string input = "t+1)))";
        REQUIRE_THROWS_AS(parse(input), invalid_argument);
    }

    SECTION("parse empty input")
    {
        string input = "";
        REQUIRE_THROWS_AS(parse(input), invalid_argument);
    }

    SECTION("atomic expression")
    {
        string input = "t";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "t");
        REQUIRE(expr->evaluate(42) == 42);
    }

    SECTION("multiple expressions")
    {
        string input = "(t+3)|(t*2)";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "((t+3)|(t*2))");
        REQUIRE(expr->evaluate(1) == 6);
    }

    SECTION("precedence")
    {
        string input = "t*2+t*3";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "((t*2)+(t*3))");
        REQUIRE(expr->evaluate(1) == 5);
    }

    SECTION("deep nesting")
    {
        string input = "(((t*t)))";
        auto expr = parse(input);
        REQUIRE(expr->to_string() == "(t*t)");
        REQUIRE(expr->evaluate(2) == 4);
    }

    SECTION("parse crowd")
    {
        string input = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";
        auto expr = parse(input);
        REQUIRE(expr);
    }

    SECTION("benchmarks")
    {
        string input = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";

        BENCHMARK("parse crowd")
        {
            return parse(input);
        };

        auto crowd = parse(input);
        CHECK(crowd);
        int t = 0;

        BENCHMARK("eval crowd")
        {
            return crowd->evaluate(t++);
        };
    }
}
