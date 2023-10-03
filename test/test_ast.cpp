#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch_all.hpp>

#include "ast.hpp"

using namespace std;
using namespace bb;

TEST_CASE("ast", "[ast]")
{
    SECTION("array subscript")
    {
        AstPtr l = AstPtr(new String("foo"));
        AstPtr r = AstPtr(new Identifier());
        AstPtr ast = AstPtr(new Subscript(move(l), move(r)));
        Value val = ast->eval(0);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 102);
    }

    SECTION("array subscript above bounds")
    {
        AstPtr l = AstPtr(new String("foo"));
        AstPtr r = AstPtr(new Identifier());
        AstPtr ast = AstPtr(new Subscript(move(l), move(r)));
        Value val = ast->eval(3);
        REQUIRE(val.is_undefined());
    }

    SECTION("array subscript below bounds")
    {
        AstPtr l = AstPtr(new String("foo"));
        AstPtr r = AstPtr(new Identifier());
        AstPtr ast = AstPtr(new Subscript(move(l), move(r)));
        Value val = ast->eval(-1);
        REQUIRE(val.is_undefined());
    }

    SECTION("add")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(1));
        AstPtr ast = AstPtr(new Add(move(l), move(r)));
        Value val = ast->eval(2);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 3);
    }

    SECTION("subtract")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(1));
        AstPtr ast = AstPtr(new Subtract(move(l), move(r)));
        Value val = ast->eval(2);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 1);
    }

    SECTION("multiply")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(2));
        AstPtr ast = AstPtr(new Multiply(move(l), move(r)));
        Value val = ast->eval(3);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 6);
    }

    SECTION("divide")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(2));
        AstPtr ast = AstPtr(new Divide(move(l), move(r)));
        Value val = ast->eval(4);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 2);
    }

    SECTION("divide by zero")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(0));
        AstPtr ast = AstPtr(new Divide(move(l), move(r)));
        Value val = ast->eval(1);
        REQUIRE(val.is_undefined());
    }

    SECTION("modulo")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(2));
        AstPtr ast = AstPtr(new Modulo(move(l), move(r)));
        Value val = ast->eval(3);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 1);
    }

    SECTION("modulo by zero")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(0));
        AstPtr ast = AstPtr(new Modulo(move(l), move(r)));
        Value val = ast->eval(1);
        REQUIRE(val.is_undefined());
    }

    SECTION("bitwise and")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(3));
        AstPtr ast = AstPtr(new BitwiseAnd(move(l), move(r)));
        Value val = ast->eval(5);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 1);
    }

    SECTION("bitwise or")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(2));
        AstPtr ast = AstPtr(new BitwiseOr(move(l), move(r)));
        Value val = ast->eval(1);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 3);
    }

    SECTION("bitwise xor")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(3));
        AstPtr ast = AstPtr(new BitwiseXor(move(l), move(r)));
        Value val = ast->eval(1);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 2);
    }

    SECTION("bitwise shift left")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(1));
        AstPtr ast = AstPtr(new BitwiseShiftLeft(move(l), move(r)));
        Value val = ast->eval(2);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 4);
    }

    SECTION("bitwise shift right")
    {
        AstPtr l = AstPtr(new Identifier());
        AstPtr r = AstPtr(new Integer(1));
        AstPtr ast = AstPtr(new BitwiseShiftRight(move(l), move(r)));
        Value val = ast->eval(4);
        REQUIRE(val.is_int());
        REQUIRE(val.to_int() == 2);
    }
}
