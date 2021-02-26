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
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(99));
        REQUIRE(Add(move(left), move(right)).evaluate(1) == 100);
    }

    SECTION("subtract")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(99));
        REQUIRE(Subtract(move(left), move(right)).evaluate(100) == 1);
    }

    SECTION("multiply")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        REQUIRE(Multiply(move(left), move(right)).evaluate(2) == 4);
    }

    SECTION("divide")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        REQUIRE(Divide(move(left), move(right)).evaluate(3) == 1);
    }

    SECTION("modulo")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        REQUIRE(Modulo(move(left), move(right)).evaluate(3) == 1);
    }

    SECTION("bitwise and")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        REQUIRE(BitwiseAnd(move(left), move(right)).evaluate(1) == 0);
    }

    SECTION("bitwise or")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        REQUIRE(BitwiseOr(move(left), move(right)).evaluate(1) == 3);
    }

    SECTION("bitwise xor")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        REQUIRE(BitwiseXor(move(left), move(right)).evaluate(1) == 0);
    }

    SECTION("bitwise shift left")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        REQUIRE(BitwiseShiftLeft(move(left), move(right)).evaluate(1) == 2);
    }

    SECTION("bitwise shift right")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        REQUIRE(BitwiseShiftRight(move(left), move(right)).evaluate(2) == 1);
    }

    SECTION("less than")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(2));
        LessThan lt(move(left), move(right));
        REQUIRE(lt.evaluate(1) == 1);
        REQUIRE(lt.evaluate(2) == 0);
        REQUIRE(lt.evaluate(3) == 0);
    }

    SECTION("greater than")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        GreaterThan gt(move(left), move(right));
        REQUIRE(gt.evaluate(2) == 1);
        REQUIRE(gt.evaluate(1) == 0);
        REQUIRE(gt.evaluate(0) == 0);
    }

    SECTION("less than or equal")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        LessThanOrEqual lte(move(left), move(right));
        REQUIRE(lte.evaluate(0) == 1);
        REQUIRE(lte.evaluate(1) == 1);
        REQUIRE(lte.evaluate(2) == 0);
    }

    SECTION("greater than or equal")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        GreaterThanOrEqual gte(move(left), move(right));
        REQUIRE(gte.evaluate(2) == 1);
        REQUIRE(gte.evaluate(1) == 1);
        REQUIRE(gte.evaluate(0) == 0);
    }

    SECTION("equal")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        Equal eq(move(left), move(right));
        REQUIRE(eq.evaluate(1) == 1);
        REQUIRE(eq.evaluate(2) == 0);
    }

    SECTION("not equal")
    {
        ExpressionPtr left = ExpressionPtr(new Variable());
        ExpressionPtr right = ExpressionPtr(new Constant(1));
        NotEqual neq(move(left), move(right));
        REQUIRE(neq.evaluate(1) == 0);
        REQUIRE(neq.evaluate(2) == 1);
    }
}
