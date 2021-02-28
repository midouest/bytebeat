#pragma once

#include <string>
#include <memory>

using namespace std;

namespace bb
{
    class Expression
    {
    public:
        virtual ~Expression(){};
        virtual int evaluate(int t) const = 0;
        virtual string to_string() const = 0;
    };

    using ExpressionPtr = unique_ptr<Expression>;

    class Variable : public Expression
    {
    public:
        int evaluate(int t) const
        {
            return t;
        };

        string to_string() const
        {
            return "t";
        }
    };

    class Constant : public Expression
    {
    public:
        Constant(int c) : c(c){};

        int evaluate(int t) const
        {
            return c;
        };

        string to_string() const
        {
            return std::to_string(c);
        }

    private:
        int c;
    };

    class BinaryOperator : public Expression
    {
    public:
        explicit BinaryOperator(ExpressionPtr left, ExpressionPtr right) : left(move(left)), right(move(right)){};

        string to_string() const
        {
            return "(" + left->to_string() + operand() + right->to_string() + ")";
        }

    protected:
        virtual string operand() const = 0;

        ExpressionPtr left;
        ExpressionPtr right;
    };

    class Add : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) + right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "+";
        }
    };

    class Subtract : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) - right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "-";
        }
    };

    class Multiply : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) * right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "*";
        }
    };

    class Divide : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            int divisor = right->evaluate(t);
            // Dividing by zero will crash the SuperCollider server
            if (divisor == 0)
            {
                return 0;
            }
            return left->evaluate(t) / divisor;
        };

    protected:
        string operand() const
        {
            return "/";
        }
    };

    class Modulo : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {

            int divisor = right->evaluate(t);
            // Dividing by zero will crash the SuperCollider server
            if (divisor == 0)
            {
                return 0;
            }
            return left->evaluate(t) % divisor;
        };

    protected:
        string operand() const
        {
            return "%";
        }
    };

    class BitwiseAnd : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) & right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "&";
        }
    };

    class BitwiseOr : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) | right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "|";
        }
    };

    class BitwiseXor : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) ^ right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "^";
        }
    };

    class BitwiseShiftLeft : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) << right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "<<";
        }
    };

    class BitwiseShiftRight : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) >> right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return ">>";
        }
    };

    class LessThan : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) < right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "<";
        }
    };

    class GreaterThan : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) > right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return ">";
        }
    };

    class LessThanOrEqual : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) <= right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "<=";
        }
    };

    class GreaterThanOrEqual : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) >= right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return ">=";
        }
    };

    class Equal : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) == right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "==";
        }
    };

    class NotEqual : public BinaryOperator
    {
    public:
        using BinaryOperator::BinaryOperator;

        int evaluate(int t) const
        {
            return left->evaluate(t) != right->evaluate(t);
        };

    protected:
        string operand() const
        {
            return "!=";
        }
    };
}
