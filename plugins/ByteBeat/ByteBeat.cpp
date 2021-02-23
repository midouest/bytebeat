#include <SC_PlugIn.hpp>

#include <string>

#include "ByteBeat.hpp"
#include "parse.hpp"

static InterfaceTable *ft;

namespace ByteBeat
{
    ByteBeat::ByteBeat()
    {
        mCalcFunc = make_calc_function<ByteBeat, &ByteBeat::next>();

        // Initialize with no audio
        mExpression = new bb::Constant(0);
    }

    ByteBeat::~ByteBeat()
    {
        delete mExpression;
    }

    void ByteBeat::setExpression(const char *input)
    {
        string s = input;

        try
        {
            bb::Expression *prevExpr = mExpression;
            mExpression = bb::parse(s);
            delete prevExpr;
        }
        catch (invalid_argument &ex)
        {
            // TODO: Send back to client somehow. May not be possible without a
            //       more general sendResponse interface.
            // See: https://scsynth.org/t/scsynth-plugincmd-and-sending-responses/2638
        }
    }

    void ByteBeat::restart()
    {
        mTime = 0;
    }

    void ByteBeat::next(int nSamples)
    {
        float *outbuf = out(0);

        for (int i = 0; i < nSamples; ++i)
        {
            // TODO: Run expression at 8khz and resample to server samplerate
            uint8_t data = mExpression->evaluate(mTime);
            ++mTime;
            outbuf[i] = 2 * (float)data / 255 - 1;
        }
    }

    /**
     * Unit command callback for the /setexpr command. Expects args to contain
     * a single string argument representing the new bytebeat expression.
     */
    void setExprCmd(ByteBeat *unit, sc_msg_iter *args)
    {
        unit->setExpression(args->gets());
    }

    /**
     * Unit command callback for the /restart command. Resets the bytebeat time
     * counter to 0.
     */
    void restartCmd(ByteBeat *unit, sc_msg_iter *args)
    {
        unit->restart();
    }
}

PluginLoad(ByteBeat)
{
    ft = inTable;

    registerUnit<ByteBeat::ByteBeat>(ft, "ByteBeat", false);

    DefineUnitCmd("ByteBeat", "/setexpr", (UnitCmdFunc)ByteBeat::setExprCmd);
    DefineUnitCmd("ByteBeat", "/restart", (UnitCmdFunc)ByteBeat::restartCmd);
}
