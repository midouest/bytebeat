#include <catch2/catch.hpp>

#include "expression.hpp"

using namespace std;
using namespace bb;

TEST_CASE("expression", "[expression]")
{
    SECTION("constant")
    {
        REQUIRE(Constant(99).evaluate(42) == 99);
    }

    SECTION("variable")
    {
        REQUIRE(Variable().evaluate(42) == 42);
    }

    SECTION("add")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(99);
        REQUIRE(Add(left, right).evaluate(1) == 100);
    }

    SECTION("subtract")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(99);
        REQUIRE(Subtract(left, right).evaluate(100) == 1);
    }

    SECTION("multiply")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        REQUIRE(Multiply(left, right).evaluate(2) == 4);
    }

    SECTION("divide")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        REQUIRE(Divide(left, right).evaluate(3) == 1);
    }

    SECTION("modulo")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        REQUIRE(Modulo(left, right).evaluate(3) == 1);
    }

    SECTION("bitwise and")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        REQUIRE(BitwiseAnd(left, right).evaluate(1) == 0);
    }

    SECTION("bitwise or")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        REQUIRE(BitwiseOr(left, right).evaluate(1) == 3);
    }

    SECTION("bitwise xor")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        REQUIRE(BitwiseXor(left, right).evaluate(1) == 0);
    }

    SECTION("bitwise shift left")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        REQUIRE(BitwiseShiftLeft(left, right).evaluate(1) == 2);
    }

    SECTION("bitwise shift right")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        REQUIRE(BitwiseShiftRight(left, right).evaluate(2) == 1);
    }

    SECTION("less than")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(2);
        LessThan lt(left, right);
        REQUIRE(lt.evaluate(1) == 1);
        REQUIRE(lt.evaluate(2) == 0);
        REQUIRE(lt.evaluate(3) == 0);
    }

    SECTION("greater than")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        GreaterThan gt(left, right);
        REQUIRE(gt.evaluate(2) == 1);
        REQUIRE(gt.evaluate(1) == 0);
        REQUIRE(gt.evaluate(0) == 0);
    }

    SECTION("less than or equal")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        LessThanOrEqual lte(left, right);
        REQUIRE(lte.evaluate(0) == 1);
        REQUIRE(lte.evaluate(1) == 1);
        REQUIRE(lte.evaluate(2) == 0);
    }

    SECTION("greater than or equal")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        GreaterThanOrEqual gte(left, right);
        REQUIRE(gte.evaluate(2) == 1);
        REQUIRE(gte.evaluate(1) == 1);
        REQUIRE(gte.evaluate(0) == 0);
    }

    SECTION("equal")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        Equal eq(left, right);
        REQUIRE(eq.evaluate(1) == 1);
        REQUIRE(eq.evaluate(2) == 0);
    }

    SECTION("not equal")
    {
        Expression *left = new Variable();
        Expression *right = new Constant(1);
        NotEqual neq(left, right);
        REQUIRE(neq.evaluate(1) == 0);
        REQUIRE(neq.evaluate(2) == 1);
    }
}
