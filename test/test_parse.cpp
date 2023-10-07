// #define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include "lex.hpp"
#include "parse.hpp"

using namespace std;
using namespace bb;

TEST_CASE("parse", "[parse]")
{
    SECTION("simple expression")
    {
        string in = "t+1";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(t+1)");
        REQUIRE(ast->eval(2).to_int() == 3);
    }

    SECTION("simple expression w/o parentheses")
    {
        string in = "t+2*t";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(t+(2*t))");
        REQUIRE(ast->eval(2).to_int() == 6);
    }

    SECTION("simple expression w/ parentheses")
    {
        string in = "(t+1)*t";
        auto ast = parse(in);
        REQUIRE((string)*ast == "((t+1)*t)");
        REQUIRE(ast->eval(2).to_int() == 6);
    }

    SECTION("unbalanced parentheses")
    {
        string in = "(t+1";
        REQUIRE_THROWS_AS(parse(in), invalid_argument);
    }

    SECTION("incomplete expression")
    {
        string in = "t+";
        REQUIRE_THROWS_AS(parse(in), invalid_argument);
    }

    SECTION("extra parentheses")
    {
        string in = "t+1)))";
        REQUIRE_THROWS_AS(parse(in), invalid_argument);
    }

    SECTION("parse empty in")
    {
        string in = "";
        REQUIRE_THROWS_AS(parse(in), invalid_argument);
    }

    SECTION("atomic expression")
    {
        string in = "t";
        auto ast = parse(in);
        REQUIRE((string)*ast == "t");
        REQUIRE(ast->eval(42).to_int() == 42);
    }

    SECTION("multiple expressions")
    {
        string in = "(t+3)|(t*2)";
        auto ast = parse(in);
        REQUIRE((string)*ast == "((t+3)|(t*2))");
        REQUIRE(ast->eval(1).to_int() == 6);
    }

    SECTION("precedence")
    {
        string in = "t*2+t*3";
        auto ast = parse(in);
        REQUIRE((string)*ast == "((t*2)+(t*3))");
        REQUIRE(ast->eval(1).to_int() == 5);
    }

    SECTION("deep nesting")
    {
        string in = "(((t*t)))";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(t*t)");
        REQUIRE(ast->eval(2).to_int() == 4);
    }

    SECTION("unary")
    {
        string in = "1+-t";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(1+(-t))");
        REQUIRE(ast->eval(1).to_int() == 0);
    }

    SECTION("brackets")
    {
        string in = "\"foo\"[t+1]+1";
        auto ast = parse(in);
        REQUIRE((string)*ast == "((\"foo\"[(t+1)])+1)");
        REQUIRE((char)ast->eval(1).to_int() == 'p');
    }

    SECTION("ternary if")
    {
        string in = "t%2==0?(t*10):(t*100)+1";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(((t%2)==0)?(t*10):((t*100)+1))");
        REQUIRE(ast->eval(2).to_int() == 20);
        REQUIRE(ast->eval(3).to_int() == 301);
    }

    SECTION("nested if")
    {
        string in = "t > 10 ? t > 20 ? 1 : 0 : -1";
        auto ast = parse(in);
        REQUIRE((string)*ast == "((t>10)?((t>20)?1:0):(-1))");
        REQUIRE(ast->eval(10).to_int() == -1);
        REQUIRE(ast->eval(20).to_int() == 0);
        REQUIRE(ast->eval(30).to_int() == 1);
    }

    SECTION("if string subscript")
    {
        string in = "(t==1?\"foo\":\"bar\")[t]";
        auto ast = parse(in);
        REQUIRE((string)*ast == "(((t==1)?\"foo\":\"bar\")[t])");
        REQUIRE((char)ast->eval(0).to_int() == 'b');
        REQUIRE((char)ast->eval(1).to_int() == 'o');
        REQUIRE((char)ast->eval(2).to_int() == 'r');
    }

    SECTION("benchmarks")
    {
        string in = "((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7";

        BENCHMARK("parse crowd") { return parse(in); };

        auto crowd = parse(in);
        int t = 0;

        BENCHMARK("eval crowd") { return crowd->eval(t++); };
    }
}
