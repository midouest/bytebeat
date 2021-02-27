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

    class BinaryExpression : public Expression
    {
    public:
        explicit BinaryExpression(ExpressionPtr left, ExpressionPtr right) : left(move(left)), right(move(right)){};

        string to_string() const
        {
            return "(" + left->to_string() + operand() + right->to_string() + ")";
        }

    protected:
        virtual string operand() const = 0;

        ExpressionPtr left;
        ExpressionPtr right;
    };

    class Add : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class Subtract : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class Multiply : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class Divide : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class Modulo : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class BitwiseAnd : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class BitwiseOr : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class BitwiseXor : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class BitwiseShiftLeft : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class BitwiseShiftRight : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class LessThan : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class GreaterThan : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class LessThanOrEqual : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class GreaterThanOrEqual : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class Equal : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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

    class NotEqual : public BinaryExpression
    {
    public:
        using BinaryExpression::BinaryExpression;

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
