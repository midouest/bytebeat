#pragma once

#include <SC_PlugIn.hpp>

#include "expression.hpp"

using namespace bb;

namespace ByteBeat
{
    class ByteBeat : public SCUnit
    {
    public:
        ByteBeat();
        ~ByteBeat();
        void set_expr(const char *input);

    private:
        void next(int nSamples);

        int t = 0;
        int gen = 0;
        Expression *exprs[2];
    };
}
