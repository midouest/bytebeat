#include <SC_PlugIn.hpp>

#include <string>

#include "ByteBeat.hpp"
#include "parse.hpp"

static InterfaceTable *ft;

namespace ByteBeat
{
    ByteBeat::ByteBeat()
    {
        Print("ByteBeat Ctor\n");

        mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();

        string s{"((t<<1)^((t<<1)+(t>>7)&t>>12))|t>>(4-(1^7&(t>>19)))|t>>7"};
        exprs[0] = bb::parse(s);
        exprs[1] = nullptr;
    }

    ByteBeat::~ByteBeat()
    {
        if (exprs[0])
        {
            delete exprs[0];
        }

        if (exprs[1])
        {
            delete exprs[1];
        }
    }

    void ByteBeat::set_expr(const char *input)
    {
        string s{input};

        try
        {
            Expression *expr = bb::parse(s);
            int next_gen = 1 - gen;
            if (exprs[next_gen])
            {
                delete exprs[next_gen];
            }
            exprs[next_gen] = expr;
            gen = next_gen;
        }
        catch (invalid_argument &ex)
        {
            // todo: send back to client, somehow
        }
    }

    void ByteBeat::next(int nSamples)
    {
        float *outbuf = out(0);

        if (!exprs[gen])
        {
            for (int i = 0; i < nSamples; ++i)
            {
                outbuf[i] = 0;
            }
        }
        else
        {
            for (int i = 0; i < nSamples; ++i)
            {
                // todo: interpolate server sample rate and 8khz
                uint8_t data = exprs[gen]->evaluate(t);
                ++t;
                outbuf[i] = 2 * (float)data / 255 - 1;
            }
        }
    }

    void set_expr(ByteBeat *unit, sc_msg_iter *args)
    {
        Print("ByteBeat::set_expr u_cmd\n");
        // unit->set_expr(args->gets());
    }
}

PluginLoad(ByteBeatUGens)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);

    DefineUnitCmd("ByteBeat", "/set_expr", (UnitCmdFunc)ByteBeat::set_expr);

    Print("ByteBeat plugin loaded!\n");
}
