#pragma once

#include <memory>
#include <string>

using namespace std;

namespace bb
{

enum class ValueType
{
    Undefined,
    Integer,
    String,
};

class Value
{
public:
    Value() : type(ValueType::Undefined) {}
    Value(const Value &value) : type(value.type)
    {
        if (type == ValueType::Integer)
        {
            i = value.i;
        }
        else if (type == ValueType::String)
        {
            s = value.s;
        }
    }
    Value(const Value &&value) : type(move(value.type))
    {
        if (type == ValueType::Integer)
        {
            i = value.i;
        }
        else if (type == ValueType::String)
        {
            s = move(value.s);
        }
    }
    Value(int i) : type(ValueType::Integer), i(i) {}
    Value(const string &s) : type(ValueType::String), s(s) {}
    ~Value() {}

    bool is_undefined() const { return type == ValueType::Undefined; }
    bool is_int() const { return type == ValueType::Integer; }
    bool is_str() const { return type == ValueType::String; }

    int to_int() const { return i; }
    const string &to_str() const { return s; }

private:
    ValueType type;
    const union
    {
        int i;
        string s;
    };
};

class Ast
{
public:
    virtual ~Ast(){};
    virtual Value eval(int t) const = 0;
    virtual operator string() const = 0;
};

using AstPtr = unique_ptr<Ast>;

class Undefined : public Ast
{
public:
    Value eval(int t) const { return Value(); }
    operator string() const { return "UNDEFINED"; }
};

class Identifier : public Ast
{
public:
    Value eval(int t) const { return t; }
    operator string() const { return "t"; }
};

class Integer : public Ast
{
public:
    Integer(int value) : value(value) {}
    Value eval(int t) const { return value; }
    operator string() const { return to_string(value); }

private:
    const int value;
};

class String : public Ast
{
public:
    String(const string &value) : value(value) {}
    Value eval(int t) const { return value; }
    operator string() const { return "\"" + value + "\""; }

private:
    const string value;
};

class UnaryOperator : public Ast
{
public:
    UnaryOperator(AstPtr &&inner) : inner(move(inner)) {}

    operator string() const
    {
        return "(" + operand() + inner->operator string() + ")";
    }

protected:
    virtual string operand() const = 0;

    AstPtr inner;
};

class Negate : public UnaryOperator
{
public:
    using UnaryOperator::UnaryOperator;

    Value eval(int t) const
    {
        Value val = inner->eval(t);
        if (val.is_int())
        {
            return -val.to_int();
        }
        return Value();
    }

protected:
    string operand() const { return "-"; }
};

class BitwiseComplement : public UnaryOperator
{
public:
    using UnaryOperator::UnaryOperator;

    Value eval(int t) const
    {
        Value val = inner->eval(t);
        if (val.is_int())
        {
            return ~val.to_int();
        }
        return Value();
    }

protected:
    string operand() const { return "-"; }
};

class Not : public UnaryOperator
{
public:
    using UnaryOperator::UnaryOperator;

    Value eval(int t) const
    {
        Value val = inner->eval(t);
        if (!val.is_int())
        {
            return Value();
        }
        return !val.to_int();
    }

protected:
    string operand() const { return "-"; }
};

class BinaryOperator : public Ast
{
public:
    explicit BinaryOperator(AstPtr left, AstPtr right)
        : left(move(left)), right(move(right))
    {
    }

    operator string() const
    {
        return "(" + left->operator string() + operand() +
               right->operator string() + ")";
    }

protected:
    virtual string operand() const = 0;

    AstPtr left;
    AstPtr right;
};

class Subscript : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value s_val = left->eval(t);
        if (!s_val.is_str())
        {
            return Value();
        }

        Value i_val = right->eval(t);
        if (!i_val.is_int())
        {
            return Value();
        }

        string s = s_val.to_str();
        int i = i_val.to_int();
        if (i < 0 || i >= s.length())
        {
            return Value();
        }

        return s[i];
    }

    operator string() const
    {
        return "(" + left->operator string() + "[" + right->operator string() +
               "])";
    }

protected:
    string operand() const { return "["; }
};

class Add : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() + b.to_int();
    }

protected:
    string operand() const { return "+"; }
};

class Subtract : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() - b.to_int();
    }

protected:
    string operand() const { return "-"; }
};

class Multiply : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() * b.to_int();
    }

protected:
    string operand() const { return "*"; }
};

class Divide : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        int b_val = b.to_int();
        if (b_val == 0)
        {
            return Value();
        }

        return a.to_int() / b.to_int();
    }

protected:
    string operand() const { return "/"; }
};

class Modulo : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        int b_val = b.to_int();
        if (b_val == 0)
        {
            return Value();
        }

        return a.to_int() % b.to_int();
    }

protected:
    string operand() const { return "%"; }
};

class BitwiseAnd : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() & b.to_int();
    }

protected:
    string operand() const { return "&"; }
};

class BitwiseOr : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() | b.to_int();
    }

protected:
    string operand() const { return "|"; }
};

class BitwiseXor : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() ^ b.to_int();
    }

protected:
    string operand() const { return "^"; }
};

class BitwiseShiftLeft : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() << b.to_int();
    }

protected:
    string operand() const { return "<<"; }
};

class BitwiseShiftRight : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() >> b.to_int();
    }

protected:
    string operand() const { return ">>"; }
};

class LessThan : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() < b.to_int();
    }

protected:
    string operand() const { return "<"; }
};

class LessThanEqual : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() <= b.to_int();
    }

protected:
    string operand() const { return "<="; }
};

class GreaterThan : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() > b.to_int();
    }

protected:
    string operand() const { return ">"; }
};

class GreaterThanEqual : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() >= b.to_int();
    }

protected:
    string operand() const { return ">="; }
};

class Equal : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() == b.to_int();
    }

protected:
    string operand() const { return "=="; }
};

class NotEqual : public BinaryOperator
{
public:
    using BinaryOperator::BinaryOperator;

    Value eval(int t) const
    {
        Value a = left->eval(t);
        if (!a.is_int())
        {
            return Value();
        }

        Value b = right->eval(t);
        if (!b.is_int())
        {
            return Value();
        }

        return a.to_int() != b.to_int();
    }

protected:
    string operand() const { return "!="; }
};

class TernaryIf : public Ast
{
public:
    TernaryIf(AstPtr pred, AstPtr pass, AstPtr fail)
        : pred(move(pred)), pass(move(pass)), fail(move(fail))
    {
    }

    Value eval(int t) const
    {
        Value p = pred->eval(t);
        if (!p.is_int())
        {
            return Value();
        }

        return p.to_int() ? pass->eval(t) : fail->eval(t);
    }

    operator string() const
    {
        return "(" + pred->operator string() + "?" + pass->operator string() +
               ":" + fail->operator string() + ")";
    }

private:
    AstPtr pred;
    AstPtr pass;
    AstPtr fail;
};

} // namespace bb
